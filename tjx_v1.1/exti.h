#ifndef __EXTI_H__
#define __EXTI_H__

#include "system.h"

// 开启/关闭全局中断
#define nvic_enable() do { EA = 1; } while (0)
#define nvic_disable() do { EA = 0; } while (0)

// 中断优先级, C51只有0和1
typedef enum
{
    NVIC_Priority_0 = 0x0, // 低
    NVIC_Priority_1,       // 高
    NVIC_Priority_2,
    NVIC_Priority_3,
} NVIC_PRIORITY;

// 外部中断号, C51只有0和1
typedef enum
{
    EXTI_0 = 0,
    EXTI_1,
    EXTI_2,
    EXTI_3
} EXTI_NUM;

// 触发方式
typedef enum
{
    EXTI_Trigger_Low = 0x0, // 低电平触发
    EXTI_Trigger_Falling    // 下降沿触发
} EXTI_TRIGGER_MODE;

typedef struct
{
    EXTI_TRIGGER_MODE trigger;
    NVIC_PRIORITY     priority;
} EXTI_CONFIG;

void nvic_set_priority(uint8_t channel, NVIC_PRIORITY priority);
void exti_init(EXTI_NUM num, EXTI_CONFIG *exti);
void exti_cmd(EXTI_NUM exti, bool enable);

// 0003H (被矩阵键盘占用)
// void exint0() interrupt 0
// 0013H
// void exint1() interrupt 2
// 0033H
// void exint2() interrupt 6
// 003BH
// void exint3() interrupt 7

#endif
