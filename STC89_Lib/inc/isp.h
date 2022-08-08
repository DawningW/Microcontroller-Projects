/**
 * @file isp.c
 * @author Wu Chen
 * @brief 内部EEPROM读写
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __ISP_H__
#define __ISP_H__

#include "system.h"

#if COMPILE_ISP == 1

#define ISP_ADDR_START 0x2000
#define ISP_ADDR_END 0x2FFF
#if   (FOSC <=  1000000UL)
    #define ISP_WAIT_TIME 0x07
#elif (FOSC <=  2000000UL)
    #define ISP_WAIT_TIME 0x06
#elif (FOSC <=  3000000UL)
    #define ISP_WAIT_TIME 0x05
#elif (FOSC <=  6000000UL)
    #define ISP_WAIT_TIME 0x04
#elif (FOSC <= 12000000UL)
    #define ISP_WAIT_TIME 0x03
#elif (FOSC <= 20000000UL)
    #define ISP_WAIT_TIME 0x02
#elif (FOSC <= 24000000UL)
    #define ISP_WAIT_TIME 0x01
#else 
    #define ISP_WAIT_TIME 0x00
#endif

/**
 * @brief ISP命令
 */
typedef enum
{
    ISP_Command_Idle = 0x0, // 空闲
    ISP_Command_Read,       // 读
    ISP_Command_Write,      // 写
    ISP_Command_Erase,      // 擦除
} ISP_COMMAND;

/**
 * @brief 配置ISP
 */
void isp_config();
/**
 * @brief 开关ISP
 * 
 * @param enable 是否启用ISP
 */
void isp_cmd(bool enable);
/**
 * @brief ISP进入待机模式
 */
void isp_idle();
/**
 * @brief 从内部EEPROM某地址处读取一个字节
 * 
 * @param addr 要读取的地址
 * @return 读取的一字节数据
 */
uint8_t isp_read_byte(uint16_t addr);
/**
 * @brief 向内部EEPROM某地址处写入一个字节
 * 
 * @param addr 要写入的地址
 * @param dat 要写入的一字节数据
 * @return 是否写入成功
 */
bool isp_write_byte(uint16_t addr, uint8_t dat);
/**
 * @brief 擦除内部EEPROM的一个扇区
 * 
 * @param addr 扇区地址
 * @return 是否成功擦除
 */
bool isp_erase_sector(uint16_t addr);

#endif // COMPILE_ISP

#endif // __ISP_H__
