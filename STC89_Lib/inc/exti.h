/**
 * @file exti.h
 * @author Chen Wu
 * @brief 外部中断
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __EXTI_H__
#define __EXTI_H__

#include "system.h"

#if COMPILE_EXTI == 1

#include "nvic.h"

/**
 * @brief 外部中断序号
 */
typedef enum
{
    EXTI_0 = 0,
    EXTI_1,
#ifdef STC89
    EXTI_2,
    EXTI_3,
#endif
} EXTI_NUM;

/**
 * @brief 外部中断触发方式
 */
typedef enum
{
    EXTI_Trigger_Low = 0x0, // 低电平触发
    EXTI_Trigger_Falling    // 下降沿触发
} EXTI_TRIGGER_MODE;

/**
 * @brief 外部中断配置
 */
typedef struct
{
    EXTI_TRIGGER_MODE trigger;
    NVIC_PRIORITY     priority;
} EXTI_CONFIG;

/**
 * @brief 外部中断初始化
 * 
 * @param num 外部中断序号
 * @param exti 外部中断配置结构体
 */
void exti_init(EXTI_NUM num, EXTI_CONFIG *exti);
/**
 * @brief 外部中断开关
 * 
 * @param exti 外部中断序号
 * @param enable 是否启用外部中断
 */
void exti_cmd(EXTI_NUM exti, bool enable);

// 0003H
#define EXINT0 INTERRUPT(exint0, 0)
// 0013H
#define EXINT1 INTERRUPT(exint1, 2)
#ifdef STC89
// 0033H
#define EXINT2 INTERRUPT(exint2, 6)
// 003BH
#define EXINT3 INTERRUPT(exint3, 7)
#endif

#endif // COMPILE_EXTI

#endif // __EXTI_H__
