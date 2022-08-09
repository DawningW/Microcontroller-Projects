/**
 * @file wdt.h
 * @author Chen Wu
 * @brief 看门狗
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __WDT_H__
#define __WDT_H__

#include "system.h"

#if COMPILE_WDT == 1

/**
 * @brief 看门狗定时器预分频值
 */
typedef enum
{
    WDT_Prescale_2 = 0x0,
    WDT_Prescale_4,
    WDT_Prescale_8,
    WDT_Prescale_16,
    WDT_Prescale_32,
    WDT_Prescale_64,
    WDT_Prescale_128,
    WDT_Prescale_256,
} WDT_PRESCALE;

/**
 * @brief 看门狗开关
 * 
 * @param enable 是否启用看门狗
 */
void wdt_cmd(bool enable);
/**
 * @brief 看门狗定时器预分频值设置
 * 
 * @param prescale 预分频值
 */
void wdt_set_prescale(WDT_PRESCALE prescale);
/**
 * @brief 看门狗定时器是否在空闲模式下计数
 * 
 * @param enable 是否在空闲模式下计数
 */
void wdt_set_idle_count(bool enable);
/**
 * @brief 喂狗
 */
void wdt_feed();

#endif // COMPILE_WDT

#endif // __WDT_H__
