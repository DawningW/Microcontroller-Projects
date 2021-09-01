#ifndef __MPU9250_H
#define __MPU9250_H

#include "main.h"
#include "mltypes.h"
#include "inv_mpu.h"

// AD0 I2C地址 接地: 0x68 接+3.3V: 0x69

#define MPU_DEBUG 1
// #define USE_MPL
// #define USE_DMP
#define SKIP_SELF_TEST

/* Sampling rate. */
#define DEFAULT_MPU_HZ  (500)
#define TEMP_READ_MS    (1000)
#define COMPASS_READ_MS (100)

bool mpu_init_user(void);
int mpu_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
int mpu_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
#ifdef COMPASS_ENABLED
uint8_t mpu_get_data(float *accel, float *gyro, float *comp);
#else
uint8_t mpu_get_data(float *accel, float *gyro);
#endif
/**
 * 与常规坐标系有些区别, x正半轴指向飞机头
 * pitch(y): 俯仰角 精度: 0.1° 范围: -90.0°  ~ +90.0°
 * roll (x): 横滚角 精度: 0.1° 范围: -180.0° ~ +180.0°
 * yaw  (z): 偏航角 精度: 0.1° 范围: -180.0° ~ +180.0°
 */
#ifdef USE_MPL
// 单位uT
void mpl_build_external_compress(short x, short y, short z, inv_time_t timestamp);
bool mpl_get_data(float *pitch, float *roll, float *yaw);
#endif
#ifdef USE_DMP
bool dmp_get_data(float *pitch, float *roll, float *yaw);
#endif

#endif /* __MPU9250_H */
