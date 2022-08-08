/**
 * @file soft_spi.h
 * @author Chen Wu
 * @brief 使用软件实现的串行外设接口(SPI, Serial Periphral Interface)
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "system.h"

#if COMPILE_SOFT_SPI == 1

// SCLK/SCL/SCK(Serial Clock): 由主设备产生的时钟信号
#ifndef SPI_SCLK
#error SPI_SCLK is not defined!
#endif
// MOSI(Master Out Slave In): 主设备输出, 从设备输入
// 在主设备上可能被称为SDO/DO, 在从设备上可能被称为SDI/DI
#ifndef SPI_MOSI
#error SPI_MOSI is not defined!
#endif
// MISO(Master In Slave Out): 主设备输入, 从设备输出
// 在主设备上可能被称为SDI/DI, 在从设备上可能被称为SDO/DO
#ifndef SPI_MISO
#error SPI_MISO is not defined!
#endif
// CS(Chip Select)/SS(Slave Select): 片选, 通常为低电平有效
#ifndef SPI_CS
#error SPI_CS is not defined!
#endif
// SPI工作模式
// 模式0: CPOL=0, CPHA=0
// 模式1: CPOL=0, CPHA=1
// 模式2: CPOL=1, CPHA=0
// 模式3: CPOL=1, CPHA=1
#ifndef SPI_MODE
#error SPI_MODE is not defined!
#endif
// CPOL(Clock POLarity): 时钟极性, 用来表示时钟信号在空闲时是高电平还是低电平, 0为低电平, 1为高电平
#ifndef SPI_CPOL
#define SPI_CPOL GET_BIT(SPI_MODE, 1)
#endif
// CPHA(Clock PHAse): 时钟相位, 0为奇数缘, 1为偶数缘
#ifndef SPI_CPHA
#define SPI_CPHA GET_BIT(SPI_MODE, 0)
#endif

/**
 * @brief 初始化软件SPI
 */
void sspi_init();
/**
 * @brief 向从设备发送并接收一个字节的数据, 工作模式由CPOL和CPHA宏指定
 * 
 * @param write_dat 要发送的数据
 * @return uint8_t 接收到的数据
 */
uint8_t sspi_transfer(uint8_t write_dat);

#endif // COMPILE_SOFT_SPI

#endif // __SOFT_SPI_H__
