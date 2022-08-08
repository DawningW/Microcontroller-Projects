/**
 * @file timer.h
 * @author Wu Chen
 * @brief 定时器
 *
 * @copyright Copyright (c) 2020-2022
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "system.h"
#include "exti.h"

#if COMPILE_TIMER == 1

// 1ms timer count
#define T1MS (65536 - FOSC / CPI / 1000)

/**
 * @brief 定时器序号
 */
typedef enum
{
    TIM_0 = 0,
    TIM_1,
#ifdef STC89
    TIM_2,
#endif
} TIM_NUM;

/**
 * @brief 定时器模式
 */
typedef enum
{
    TIM_Mode_0 = 0x0, // 定时器0/1: 13位定时器/计数器, 定时器2: 16位自动重装
    TIM_Mode_1,       // 定时器0/1: 16位定时器/计数器, 定时器2: 16位捕获
    TIM_Mode_2,       // 定时器0/1: 8位自动重装载定时器, 定时器2: 波特率发生器
    TIM_Mode_3,       // 定时器0: TL0作为8位定时器/计数器, TH0作为8位定时器
} TIM_MODE;

/**
 * @brief 定时器功能
 */
typedef enum
{
    TIM_Function_Timer = 0x0, // 定时器模式
    TIM_Function_Count,       // 计数器模式
} TIM_FUNCTION;

/**
 * @brief 定时器配置
 */
typedef struct
{
    TIM_MODE      mode;
    TIM_FUNCTION  function;
    bool          enable_int;
    NVIC_PRIORITY priority;
    uint16_t      value;
    uint16_t      period; // 仅定时器0/1模式2和定时器2模式0可用
} TIM_CONFIG;

/**
 * @brief 定时器初始化
 * 
 * @param num 定时器序号
 * @param timer 定时器配置结构体
 */
void timer_init(TIM_NUM num, TIM_CONFIG *timer);
/**
 * @brief 定时器开关
 * 
 * @param tim 定时器序号
 * @param enable 是否开启定时器
 */
void timer_cmd(TIM_NUM tim, bool enable);
/**
 * @brief 定时器是否溢出
 * 
 * @param tim 定时器序号
 * @return 定时器是否溢出
 */
bool timer_is_overflow(TIM_NUM tim);
/**
 * @brief 获取定时器的计数值
 * 
 * @param tim 定时器序号
 * @return uint16_t 定时器计数值
 */
uint16_t timer_get_value(TIM_NUM tim);
/**
 * @brief 设置定时器的计数值
 * 
 * @param tim 定时器序号
 * @param value 定时器计数值
 */
void timer_set_value(TIM_NUM tim, uint16_t value);

// 000BH
#define TIMER0 INTERRUPT(timer0, 1)
// 001BH (作为串口波特率发生器)
#define TIMER1 INTERRUPT(timer1, 3)
#ifdef STC89
// 002BH
#define TIMER2 INTERRUPT(timer2, 5)
#endif

#endif // COMPILE_TIMER

#endif // __TIMER_H__
