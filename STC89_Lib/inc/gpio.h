/**
 * @file gpio.h
 * @author Chen Wu
 * @brief GPIO控制, 由于51单片机的GPIO均为准双向口, 无需配置直接读写相应寄存器位即可, 因此不建议使用此模块
 * 
 * @copyright Copyright (c) 2020-2022
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include "system.h"

#if COMPILE_GPIO == 1

/**
 * @brief GPIO 端口号
 */
typedef enum
{
    GPIO_0 = 0x0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
} GPIO_PORT;

/**
 * @brief GPIO 引脚序号
 */
typedef enum
{
    GPIO_PIN_0 = BIT_MASK(0),
    GPIO_PIN_1 = BIT_MASK(1),
    GPIO_PIN_2 = BIT_MASK(2),
    GPIO_PIN_3 = BIT_MASK(3),
    GPIO_PIN_4 = BIT_MASK(4),
    GPIO_PIN_5 = BIT_MASK(5),
    GPIO_PIN_6 = BIT_MASK(6),
    GPIO_PIN_7 = BIT_MASK(7),
    GPIO_PIN_ALL = 0xFF,
} GPIO_PIN;

/**
 * @brief 读取某端口所有位的电平
 * 
 * @param port GPIO端口
 * @return 值
 */
uint8_t gpio_read(GPIO_PORT port);
/**
 * @brief 读取某端口某位的电平
 * 
 * @param port GPIO端口
 * @param pin GPIO引脚
 * @return 值
 */
bool gpio_read_bit(GPIO_PORT port, GPIO_PIN pin);
/**
 * @brief 写某端口所有位的电平
 * 
 * @param port GPIO端口
 * @param value 值
 */
void gpio_write(GPIO_PORT port, uint8_t value);
/**
 * @brief 写某端口某位的电平
 * 
 * @param port GPIO端口
 * @param pin GPIO引脚
 * @param value 值
 */
void gpio_write_bit(GPIO_PORT port, GPIO_PIN pin, bool value);
/**
 * @brief 将某端口的指定引脚设为高电平
 * 
 * @param port GPIO端口
 * @param pin GPIO引脚
 */
void gpio_set_bits(GPIO_PORT port, GPIO_PIN pin);
/**
 * @brief 将某端口的指定引脚设为低电平
 * 
 * @param port GPIO端口
 * @param pin GPIO引脚
 */
void gpio_reset_bits(GPIO_PORT port, GPIO_PIN pin);
/**
 * @brief 切换某端口的指定引脚的电平
 * 
 * @param port GPIO端口
 * @param pin GPIO引脚
 */
void gpio_toggle_bits(GPIO_PORT port, GPIO_PIN pin);

#endif // COMPILE_GPIO

#endif // __GPIO_H__
