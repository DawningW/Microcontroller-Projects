/**
 * @file nvic.h
 * @author Chen Wu
 * @brief 中断优先级控制
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __NVIC_H__
#define __NVIC_H__

#include "system.h"

/**
 * @brief 开启全局中断
 */
#define nvic_enable() do { EA = 1; } while (0)
/**
 * @brief 关闭全局中断
 */
#define nvic_disable() do { EA = 0; } while (0)

/**
 * @brief 中断优先级
 */
typedef enum
{
    NVIC_Priority_0 = 0x0, // 低
    NVIC_Priority_1,       // 高
#ifdef STC89
    NVIC_Priority_2,
    NVIC_Priority_3,
#endif
} NVIC_PRIORITY;

/**
 * @brief 中断优先级设置
 * 
 * @param channel 中断号
 * @param priority 中断优先级
 */
void nvic_set_priority(uint8_t channel, NVIC_PRIORITY priority);

#endif // __NVIC_H__
