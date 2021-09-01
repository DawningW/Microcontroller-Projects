#ifndef __HMC5883_H
#define __HMC5883_H

#include "main.h"

#define QMC5883L

#if defined(HMC5883L)
#define HMC_MAX_VALUE 2047.0f
#define HMC_MIN_VALUE -2048.0f
// 1090Counts/Gauss 1Gs=100uT
#define HMC_COUNTS_PER_UT 10.9f

#define HMC_ADDRESS 0x1e << 1 // 0x3c
// 模式寄存器
#define HMC_MODE_REGISTER 0x02
// 数据输出寄存器(XZY, 先高位后低位)
#define HMC_DATA_OUTPUT_REGISTER 0x03

#elif defined(QMC5883L)
#define HMC_MAX_VALUE 32767.0f
#define HMC_MIN_VALUE -32768.0f
// 12000Counts/Gauss 1Gs=100uT
#define HMC_COUNTS_PER_UT 120.0f

#define HMC_ADDRESS 0x0d << 1 // 0x1a
// 数据输出寄存器(XYZ, 先低位后高位)
#define HMC_DATA_OUTPUT_REGISTER 0x00
// 控制寄存器
#define HMC_CONTROL_REGISTER 0x09
// 设置清除时间寄存器
#define HMC_SET_RESET_PERIOD_REGISTER 0x0b

#else
#error You have not specified the model of hmc5883
#endif

bool hmc5883_init(void);
bool hmc5883_write(uint8_t reg, uint8_t data);
bool hmc5883_read(uint8_t reg, uint8_t *data, int length);
bool hmc5883_get_raw_data(short *data);
bool hmc5883_get_data(short *x, short *y, short *z, float *angle);
void hmc5883_calibrate(void);

#endif /* __HMC5883_H */
