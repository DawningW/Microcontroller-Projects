#include "hmc5883.h"
#include <stdio.h>
#include <math.h>
#include "delay.h"
#include "i2c.h"

#define MAGNETC_DECLINATION 7.73 // 所在地磁偏角
#define CALIBRATE_THRESHOLD 0
#define M_PI 3.14159265
#define abs(x) ((x) < 0 ? -(x) : (x))

int offsetX, offsetY, offsetZ;
float gainX, gainY, gainZ;

static bool hmc5883_self_test(void)
{
    /* TODO hmc5883自检
    可用下列方式来判断芯片是否正常
    判断标准在空旷的环境下两次读数据寄存器的值
    其结果表现状况：绝对值大致相同，符号相反
1.	0x20 = 40,  0x21=1,  0x0B=1,   0x09=1D
2.	Wait 10ms
3.	0x09=1C    ( enter standby mode)
4.	0x0B=0      (turn off set/reset)
5.	0x09=12    (self-test mode,  do set)
6.	Wait 10ms and check whether 0x09=10 (wait until selftest done)
7.	0x09=1D
8.	Read x/y/z data  ( data A )
9.	0x09=13     (self-test mode,  do reset)
10.	Wait 10ms  and check whether 0x09=10 (wait until selftest done)
11.	0x09=1D
12.	Read x/y/z data  ( data B )
    */
    return true;
}

bool hmc5883_init(void)
{
#if defined(HMC5883L)
    hmc5883_write(HMC_MODE_REGISTER, 0x00);
#elif defined(QMC5883L)
    hmc5883_write(HMC_SET_RESET_PERIOD_REGISTER, 0x01);
    hmc5883_write(0x20, 0x40);
    hmc5883_write(0x21, 0x01);
    // 模式为连续测量, 数据输出频率200Hz, 范围2Gauss, 采样比率512
    hmc5883_write(HMC_CONTROL_REGISTER, 0x0d);
#endif
    // 自检
    if (!hmc5883_self_test())
    {
        printf("HMC5883 could not pass self test.\r\n");
        return false;
    }
    return true;
}

bool hmc5883_write(uint8_t reg, uint8_t data)
{
    uint8_t dataArray[1] = { data };
    HAL_I2C_Mem_Write(&hi2c1, HMC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, dataArray, 1, HAL_MAX_DELAY);
    return true;
}

bool hmc5883_read(uint8_t reg, uint8_t *data, int length)
{
    HAL_I2C_Mem_Read(&hi2c1, HMC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
    return true;
}

bool hmc5883_get_raw_data(short *data)
{
    uint8_t buffer[6];
    if (!hmc5883_read(HMC_DATA_OUTPUT_REGISTER, buffer, 6))
        return false;
#if defined(HMC5883L)
    data[0] = buffer[0] << 8 | buffer[1];
    data[1] = buffer[4] << 8 | buffer[5];
    data[2] = buffer[2] << 8 | buffer[3];
#elif defined(QMC5883L)
    data[0] = buffer[1] << 8 | buffer[0];
    data[1] = buffer[3] << 8 | buffer[2];
    data[2] = buffer[5] << 8 | buffer[4];
#endif
    return true;
}

bool hmc5883_get_data(short *x, short *y, short *z, float *angle)
{
    short rawData[3];
    if (!hmc5883_get_raw_data(rawData))
        return false;
    short correctX = (rawData[0] - offsetX) * gainX;
    short correctY = (rawData[1] - offsetY) * gainY;
    short correctZ = (rawData[2] - offsetZ) * gainZ;
    *x = correctX / HMC_COUNTS_PER_UT;
    *y = correctY / HMC_COUNTS_PER_UT;
    *z = correctZ / HMC_COUNTS_PER_UT;
    if (angle)
    {
        *angle = atan2(correctY, correctX) * 180 / M_PI + 180; // + MAGNETC_DECLINATION;
        // float angleXZ = atan2(*z, *x) * 180 / M_PI + 180;
        // float angleYZ = atan2(*z, *y) * 180 / M_PI + 180;
        if (*angle > 360) *angle -= 360;
    }
    return true;
}

void hmc5883_calibrate(void)
{
    short data[3];
    short xMax, xMin, yMax, yMin, zMax, zMin;
    offsetX = offsetY = offsetZ = 0;
    printf("Start calibration......\r\n");
    printf("Please turn your device around in 10 seconds\r\n");
    hmc5883_get_raw_data(data);  
    xMax = xMin = data[0];
    yMax = yMin = data[1];
    zMax = zMin = data[2];
    for (uint8_t i = 0; i < 100; i++)
    {
        hmc5883_get_raw_data(data);
        if (data[0] > xMax) xMax = data[0];
        if (data[0] < xMin) xMin = data[0];
        if (data[1] > yMax) yMax = data[1];
        if (data[1] < yMin) yMin = data[1];
        if (data[2] > zMax) zMax = data[2];
        if (data[2] < zMin) zMin = data[2];
        delay_ms(100);
    }
    if (abs(xMax - xMin) > CALIBRATE_THRESHOLD)
        offsetX = (xMax + xMin) / 2;
    if (abs(yMax - yMin) > CALIBRATE_THRESHOLD)
        offsetY = (yMax + yMin) / 2;
    if (abs(zMax - zMin) > CALIBRATE_THRESHOLD)
        offsetZ = (zMax + zMin) / 2;
    gainX = 1.0f;
    gainY = (float) (xMax - xMin) / (yMax - yMin);
    gainZ = (float) (xMax - xMin) / (zMax - zMin);
    printf("offset: x%d, y%d, z%d\r\n", offsetX, offsetY, offsetZ);
    printf("gain: x%.3f, y%.3f, z%.3f\r\n", gainX, gainY, gainZ);
}
