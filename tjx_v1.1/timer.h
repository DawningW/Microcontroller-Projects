#ifndef __TIMER_H__
#define __TIMER_H__

#include "system.h"
#include "exti.h"

// Timer clock mode, comment this line is 12T mode, uncomment is 1T mode
// #define MODE1T

#ifdef MODE1T
#define T1MS (65536 - FOSC / 1000) // 1ms timer calculation method in 1T mode
#else
#define T1MS (65536 - FOSC / 12 / 1000) // 1ms timer calculation method in 12T mode
#endif

// 定时器号, C51只有0和1
typedef enum
{
    TIM_0 = 0,
    TIM_1,
    TIM_2
} TIM_NUM;

typedef enum
{
    TIM_Mode_0 = 0x0, // 定时器0/1: 13位定时器/计数器, 定时器2: 16位自动重装
    TIM_Mode_1,       // 定时器0/1: 16位定时器/计数器, 定时器2: 16位捕获
    TIM_Mode_2,       // 定时器0/1: 8位自动重装载定时器, 定时器2: 波特率发生器
    TIM_Mode_3        // 定时器0: TL0作为8位定时器/计数器, TH0作为8位定时器
} TIM_MODE;

typedef enum
{
    TIM_Function_Timer = 0x0, // 定时器模式
    TIM_Function_Count        // 计数器模式
} TIM_FUNCTION;

typedef struct
{
    TIM_MODE      mode;
    TIM_FUNCTION  function;
    bool          enable_int;
    NVIC_PRIORITY priority;
    uint16_t      value;
    uint16_t      period; // 仅定时器0/1模式2和定时器2模式0可用
} TIM_CONFIG;

void timer_init(TIM_NUM num, TIM_CONFIG *timer);
void timer_cmd(TIM_NUM tim, bool enable);
bool timer_is_overflow(TIM_NUM tim);
uint16_t timer_get_value(TIM_NUM tim);
void timer_set_value(TIM_NUM tim, uint16_t value);

void delay(WORD ms);
void delays(WORD s);

// 000BH
// void timer0() interrupt 1
// 001BH (作为串口波特率发生器)
// void timer1() interrupt 3
// 002BH
// void timer2() interrupt 5
    
#endif
