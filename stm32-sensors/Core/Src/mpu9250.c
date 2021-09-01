#include "mpu9250.h"

#include "log.h"
#include "mpu.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "packet.h"
#include "i2c.h"

// long转float时的除数
#define q30 1073741824.0f
#define q16 65536.0f
#define log_i printf // MPL_LOGI
#define log_e printf // MPL_LOGE

#ifdef USE_MPL
unsigned char *mpl_key = (unsigned char*) "eMPL 5.1";

/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};
/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 }
};
#if defined MPU9150 || defined MPU9250
static struct platform_data_s compass_pdata = {
    .orientation = { 0, 1, 0,
                     1, 0, 0,
                     0, 0, -1 }
};
#define COMPASS_ENABLED 1
#elif defined AK8975_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0, 1, 0,
                     0, 0,-1 }
};
#define COMPASS_ENABLED 1
#elif defined AK8963_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0,-1, 0,
                     0, 0, 1 }
};
#define COMPASS_ENABLED 1
#endif
#endif

static bool run_self_test(void)
{
    int result;
    long gyro[3], accel[3];
#if defined (MPU6500) || defined (MPU9250)
    result = mpu_run_6500_self_test(gyro, accel, MPU_DEBUG);
#elif defined (MPU6050) || defined (MPU9150)
    result = mpu_run_self_test(gyro, accel);
#endif
    if (result == 0x7)
    {
        log_i("Self test passed!\r\n");
        log_i("accel: %7.4f %7.4f %7.4f\r\n",
                    accel[0] / 65536.f,
                    accel[1] / 65536.f,
                    accel[2] / 65536.f);
        log_i("gyro: %7.4f %7.4f %7.4f\r\n",
                    gyro[0] / 65536.f,
                    gyro[1] / 65536.f,
                    gyro[2] / 65536.f);
        /* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/
#ifdef USE_CAL_HW_REGISTERS
        /*
         * This portion of the code uses the HW offset registers that are in the MPUxxxx devices
         * instead of pushing the cal data to the MPL software library
         */
        unsigned char i = 0;
        for(i = 0; i<3; i++)
        {
        	gyro[i] = (long)(gyro[i] * 32.8f); //convert to +-1000dps
        	accel[i] *= 2048.f; //convert to +-16G
        	accel[i] = accel[i] >> 16;
        	gyro[i] = (long)(gyro[i] >> 16);
        }
        mpu_set_gyro_bias_reg(gyro);
#if defined (MPU6500) || defined (MPU9250)
        mpu_set_accel_bias_6500_reg(accel);
#elif defined (MPU6050) || defined (MPU9150)
        mpu_set_accel_bias_6050_reg(accel);
#endif
#else
        /* Push the calibrated data to the MPL library.
         *
         * MPL expects biases in hardware units << 16, but self test returns
		 * biases in g's << 16.
		 */
    	unsigned short accel_sens;
    	float gyro_sens;
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		inv_set_accel_bias(accel, 3);
		mpu_get_gyro_sens(&gyro_sens);
		gyro[0] = (long) (gyro[0] * gyro_sens);
		gyro[1] = (long) (gyro[1] * gyro_sens);
		gyro[2] = (long) (gyro[2] * gyro_sens);
		inv_set_gyro_bias(gyro, 3);
#endif
    }
    else
    {
        if (!(result & 0x1))
            log_e("Gyro test failed.\r\n");
        if (!(result & 0x2))
            log_e("Accel test failed.\r\n");
#ifdef COMPASS_ENABLED
        if (!(result & 0x4))
            log_e("Compass test failed.\r\n");
#endif
        return false;
    }
    return true;
}

bool mpu_init_user(void)
{
    inv_error_t result;
    uint16_t sample_rate;
    uint8_t accel_fsr;
    uint16_t gyro_fsr;
#ifdef COMPASS_ENABLED
    uint16_t compass_fsr;
#endif
    // 关闭中断
    __disable_irq();
    // 初始化MPU
    result = mpu_init(NULL);
    if (result)
    {
        log_e("Could not initialize MPU.\r\n");
        return false;
    }
    /* If you're not using an MPU9150 AND you're not using DMP features, this
     * function will place all slaves on the primary bus.
     * mpu_set_bypass(1);
     */
    // 初始化MPL
#ifdef USE_MPL
    result = inv_init_mpl();
    if (result)
    {
        log_e("Could not initialize MPL.\r\n");
        return false;
    }
    /* Compute 6-axis and 9-axis quaternions. */
    inv_enable_quaternion();
    inv_enable_9x_sensor_fusion();
    /* The MPL expects compass data at a constant rate (matching the rate
     * passed to inv_set_compass_sample_rate). If this is an issue for your
     * application, call this function, and the MPL will depend on the
     * timestamps passed to inv_build_compass instead.
     *
     * inv_9x_fusion_use_timestamps(1);
     */
    /* This function has been deprecated.
     * inv_enable_no_gyro_fusion();
     */
    /* Update gyro biases when not in motion.
     * WARNING: These algorithms are mutually exclusive.
     */
    inv_enable_fast_nomot();
    /* inv_enable_motion_no_motion(); */
    /* inv_set_no_motion_time(1000); */
    /* Update gyro biases when temperature changes. */
    inv_enable_gyro_tc();
    /* This algorithm updates the accel biases when in motion. A more accurate
     * bias measurement can be made when running the self-test (see case 't' in
     * handle_input), but this algorithm can be enabled if the self-test can't
     * be executed in your application.
     *
     * inv_enable_in_use_auto_calibration();
     */
#ifdef COMPASS_ENABLED
    /* Compass calibration algorithms. */
    inv_enable_vector_compass_cal();
    inv_enable_magnetic_disturbance();
#endif
    /* If you need to estimate your heading before the compass is calibrated,
     * enable this algorithm. It becomes useless after a good figure-eight is
     * detected, so we'll just leave it out to save memory.
     * inv_enable_heading_from_gyro();
     */
    /* Allows use of the MPL APIs in read_from_mpl. */
    inv_enable_eMPL_outputs();
    // 开启MPL
    result = inv_start_mpl();
    if (result)
    {
        log_e("Could not start MPL.\r\n");
        return false;
    }
#endif
    /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
#ifdef COMPASS_ENABLED
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
#else
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
#endif
    /* Push both gyro and accel data into the FIFO. */
    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    mpu_set_sample_rate(DEFAULT_MPU_HZ);
#ifdef COMPASS_ENABLED
    /* The compass sampling rate can be less than the gyro/accel sampling rate.
     * Use this function for proper power management.
     */
    mpu_set_compass_sample_rate(1000 / COMPASS_READ_MS);
#endif
    /* Read back configuration in case it was set improperly. */
    mpu_get_sample_rate(&sample_rate);
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);
#ifdef COMPASS_ENABLED
    mpu_get_compass_fsr(&compass_fsr);
#endif
#ifdef USE_MPL
    /* Sync driver configuration with MPL. */
    /* Sample rate expected in microseconds. */
    inv_set_gyro_sample_rate(1000000L / sample_rate);
    inv_set_accel_sample_rate(1000000L / sample_rate);
#ifdef COMPASS_ENABLED
    /* The compass rate is independent of the gyro and accel rates. As long as
     * inv_set_compass_sample_rate is called with the correct value, the 9-axis
     * fusion algorithm's compass correction gain will work properly.
     */
    inv_set_compass_sample_rate(COMPASS_READ_MS * 1000L);
#endif
    /* Set chip-to-body orientation matrix.
     * Set hardware units to dps/g's/degrees scaling factor.
     */
    inv_set_gyro_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_pdata.orientation), gyro_fsr << 15);
    inv_set_accel_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_pdata.orientation), accel_fsr << 15);
#ifdef COMPASS_ENABLED
    inv_set_compass_orientation_and_scale(
            inv_orientation_matrix_to_scalar(compass_pdata.orientation), compass_fsr << 15);
#endif
#endif
    // 加载DMP驱动
#ifdef USE_DMP
    /* To initialize the DMP:
     * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
     *    inv_mpu_dmp_motion_driver.h into the MPU memory.
     * 2. Push the gyro and accel orientation matrix to the DMP.
     * 3. Register gesture callbacks. Don't worry, these callbacks won't be
     *    executed unless the corresponding feature is enabled.
     * 4. Call dmp_enable_feature(mask) to enable different features.
     * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
     * 6. Call any feature-specific control functions.
     *
     * To enable the DMP, just call mpu_set_dmp_state(1). This function can
     * be called repeatedly to enable and disable the DMP at runtime.
     *
     * The following is a short summary of the features supported in the DMP
     * image provided in inv_mpu_dmp_motion_driver.c:
     * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
     * 200Hz. Integrating the gyro data at higher rates reduces numerical
     * errors (compared to integration on the MCU at a lower sampling rate).
     * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
     * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
     * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
     * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
     * an event at the four orientations where the screen should rotate.
     * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
     * no motion.
     * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
     * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
     * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
     * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
     */
    result = dmp_load_motion_driver_firmware();
    if (result)
    {
        log_e("Could not load DMP driver.\r\n");
    }
    dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_pdata.orientation));
    // dmp_register_tap_cb(tap_cb);
    // dmp_register_android_orient_cb(android_orient_cb);
    /*
     * Known Bug -
     * DMP when enabled will sample sensor data at 200Hz and output to FIFO at the rate
     * specified in the dmp_set_fifo_rate API. The DMP will then sent an interrupt once
     * a sample has been put into the FIFO. Therefore if the dmp_set_fifo_rate is at 25Hz
     * there will be a 25Hz interrupt from the MPU device.
     *
     * There is a known issue in which if you do not enable DMP_FEATURE_TAP
     * then the interrupts will be at 200Hz even if fifo rate
     * is set at a different rate. To avoid this issue include the DMP_FEATURE_TAP
     *
     * DMP sensor fusion works only with gyro at +-2000dps and accel +-2G
     */
    uint16_t dmp_features = DMP_FEATURE_6X_LP_QUAT |
        // DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | 
        DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL;
    dmp_enable_feature(dmp_features);
    dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    // 使能DMP
    result = mpu_set_dmp_state(1);
    if (result)
    {
        log_e("Could not enable DMP driver.\r\n");
        return false;
    }
#endif
    // 自检
#ifndef SKIP_SELF_TEST
    if (!run_self_test())
    {
        log_e("Could not pass self test.\r\n");
    }
#endif
    // 开启中断
    __enable_irq();
    return true;
}

int mpu_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data)
{
    return HAL_I2C_Mem_Write(&hi2c2, slave_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

int mpu_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data)
{
    return HAL_I2C_Mem_Read(&hi2c2, slave_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

#ifdef COMPASS_ENABLED
uint8_t mpu_get_data(float *accel, float *gyro, float *comp)
#else
uint8_t mpu_get_data(float *accel, float *gyro)
#endif
{
    uint8_t ret = 0;
    if (accel)
    {
        short accel_short[3];
        if (mpu_get_accel_reg(accel_short, NULL))
        {
            ret |= 0x01;
        }
        else
        {
            accel[0] = accel_short[0] / 16384.0f * 9.8f;
            accel[1] = accel_short[1] / 16384.0f * 9.8f;
            accel[2] = accel_short[2] / 16384.0f * 9.8f;
        }
    }
    if (gyro)
    {
        short gyro_short[3];
        if (mpu_get_gyro_reg(gyro_short, NULL))
        {
            ret |= 0x02;
        }
        else
        {
            gyro[0] = gyro_short[0] / 16.4f - 4.39f;
            gyro[1] = gyro_short[1] / 16.4f + 0.49f;
            gyro[2] = gyro_short[2] / 16.4f - 0.43f;
        }
    }
#ifdef COMPASS_ENABLED
    if (comp)
    {
        short comp_short[3];
        if (mpu_get_compass_reg(comp_short, NULL))
        {
            ret |= 0x04;
        }
        else
        {
            comp[0] = comp_short[0] * 0.6f;
            comp[1] = comp_short[0] * 0.6f;
            comp[2] = comp_short[0] * 0.6f;
        }
    }
#endif
    return ret;
}

#ifdef USE_MPL
void mpl_build_external_compress(short x, short y, short z, inv_time_t timestamp)
{
    long temp[3];
    temp[0] = x * q16;
    temp[1] = y * q16;
    temp[2] = z * q16;
    inv_build_compass(temp, INV_CALIBRATED | 0, timestamp);
}

bool mpl_get_data(float *pitch, float *roll, float *yaw)
{
    inv_time_t timestamp;
    short gyro[3], accel[3];
#ifdef COMPASS_ENABLED
    short comp[3];
#endif
    long temperature;
    uint8_t sensors, more;
    /* This function gets new data from the FIFO. The FIFO can contain
     * gyro, accel, both, or neither. The sensors parameter tells the
     * caller which data fields were actually populated with new data.
     * For example, if sensors == INV_XYZ_GYRO, then the FIFO isn't
     * being filled with accel data. The more parameter is non-zero if
     * there are leftover packets in the FIFO. The HAL can use this
     * information to increase the frequency at which this function is
     * called.
     */
    mpu_read_fifo(gyro, accel, &timestamp, &sensors, &more);
    if (!(sensors & INV_XYZ_ACCEL))
        mpu_get_accel_reg(accel, &timestamp);
    long accel_long[3];
    accel_long[0] = (long) accel[0];
    accel_long[1] = (long) accel[1];
    accel_long[2] = (long) accel[2];
    inv_build_accel(accel_long, 0, timestamp);
    if (!(sensors & INV_XYZ_GYRO))
        mpu_get_gyro_reg(gyro, &timestamp);
    inv_build_gyro(gyro, timestamp);
    inv_time_t prev_timestamp;
    inv_get_temp_set(&temperature, NULL, &prev_timestamp);
    get_tick_count(&timestamp);
    if (timestamp - prev_timestamp >= TEMP_READ_MS)
    {
        mpu_get_temperature(&temperature, &timestamp);
        inv_build_temp(temperature, timestamp);
    }
#ifdef COMPASS_ENABLED
    /* For any MPU device with an AKM on the auxiliary I2C bus, the raw
     * magnetometer registers are copied to special gyro registers.
     */
    if (!mpu_get_compass_reg(comp, &timestamp))
    {
        long temp[3];
        temp[0] = (long) comp[0];
        temp[1] = (long) comp[1];
        temp[2] = (long) comp[2];
        /* NOTE: If using a third-party compass calibration library,
         * pass in the compass data in uT * 2^16 and set the second
         * parameter to INV_CALIBRATED | acc, where acc is the
         * accuracy from 0 to 3.
         */
        inv_build_compass(compass, 0, timestamp);
    }
#endif
    // 开始计算
    inv_execute_on_data();
    // 报告结果
    long result[3];
    int8_t accuracy;
    inv_get_sensor_type_euler(result, &accuracy, &timestamp);
    // eMPL_send_data(PACKET_DATA_EULER, result);
    *roll  = result[0] / q16;
    *pitch = -result[1] / q16;
    *yaw   = -result[2] / q16;
    return true;
}
#endif

#ifdef USE_DMP
bool dmp_get_data(float *pitch, float *roll, float *yaw)
{
    inv_time_t timestamp;
    short gyro[3], accel[3];
    long quat[4];
    short sensors;
    uint8_t more;
    /* This function gets new data from the FIFO when the DMP is in
     * use. The FIFO can contain any combination of gyro, accel,
     * quaternion, and gesture data. The sensors parameter tells the
     * caller which data fields were actually populated with new data.
     * For example, if sensors == (INV_XYZ_GYRO | INV_WXYZ_QUAT), then
     * the FIFO isn't being filled with accel data.
     * The driver parses the gesture data to determine if a gesture
     * event has occurred; on an event, the application will be notified
     * via a callback (assuming that a callback function was properly
     * registered). The more parameter is non-zero if there are
     * leftover packets in the FIFO.
     */
    dmp_read_fifo(gyro, accel, quat, &timestamp, &sensors, &more);
    if (sensors & INV_WXYZ_QUAT)
    {
        inv_build_quat(quat, 0, timestamp);
        float q0 = quat[0] / q30;
        float q1 = quat[1] / q30;
        float q2 = quat[2] / q30;
        float q3 = quat[3] / q30;
        *pitch = asin(2 * (q0 * q2 - q1 * q3)) * 57.29578f;
        *roll  = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)) * 57.29578f;
        *yaw   = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.29578f;
        return true;
    }
    return false;
}
#endif
