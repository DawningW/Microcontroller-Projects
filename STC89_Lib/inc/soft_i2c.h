/**
 * @file soft_i2c.h
 * @author Chen Wu
 * @brief 使用软件实现的I2C接口(I2C, Inter-Integrated Circuit)
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "system.h"

#if COMPILE_SOFT_I2C == 1

// SCL(Serial Clock): 时钟信号线
#ifndef I2C_SCL
#error I2C_SCL is not defined!
#endif
// SDA(Serial Data): 数据信号线
#ifndef I2C_SDA
#error I2C_SDA is not defined!
#endif

/**
 * @brief 向某地址处的设备的某个寄存器写入指定长度的数据
 * 
 * @param address 设备地址
 * @param reg_addr 寄存器地址
 * @param dat 存放数据的数组首指针
 * @param len 数组长度
 */
void si2c_write_reg(uint8_t address, uint8_t reg_addr, uint8_t* dat, uint8_t len);
/**
 * @brief 从某地址处的设备的某个寄存器读取指定长度的数据
 * 
 * @param address 设备地址
 * @param reg_addr 寄存器地址
 * @param buffer 存放数据的数组首指针
 * @param len 数组长度
 */
void si2c_read_reg(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint8_t len);

#endif // COMPILE_SOFT_I2C

#endif // __SOFT_I2C_H__
