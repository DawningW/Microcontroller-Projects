#include "timer.h"

void timer_init(TIM_NUM num, TIM_CONFIG *timer)
{
    if (num == TIM_0)
    {
        TR0 = 0;
        WRITE_REG(TMOD, 0x03, 0, timer->mode);
        WRITE_REG_BIT(TMOD, 2, timer->function);
        TL0 = timer->value;
        if (timer->mode == TIM_Mode_2)
            TH0 = timer->period;
        else
            TH0 = timer->value >> 8;
        if (timer->enable_int)
            nvic_set_priority(1, timer->priority);
        ET0 = timer->enable_int;
    }
    else if (num == TIM_1)
    {
        TR1 = 0;
        WRITE_REG(TMOD, 0x03, 4, timer->mode);
        WRITE_REG_BIT(TMOD, 6, timer->function);
        TL1 = timer->value;
        if (timer->mode == TIM_Mode_2)
            TH1 = timer->period;
        else
            TH1 = timer->value >> 8;
        if (timer->enable_int)
            nvic_set_priority(3, timer->priority);
        ET1 = timer->enable_int;
    }
    else if (num == TIM_2)
    {
        TR2 = 0;
        __CLEAR_BIT(T2CON, 0x31);
        if (timer->mode == TIM_Mode_2)
            __SET_BIT(T2CON, 0x30);
        else
            CP_RL2 = timer->mode;
        C_T2 = timer->function;
        if (timer->mode != TIM_Mode_1)
        {
            RCAP2L = timer->period;
            RCAP2H = timer->period >> 8;
        }
        TL2 = timer->value;
        TH2 = timer->value >> 8;
        if (timer->enable_int)
            nvic_set_priority(5, timer->priority);
        ET2 = timer->enable_int;
    }
}

void timer_cmd(TIM_NUM tim, bool enable)
{
    switch (tim)
    {
        case TIM_0: TR0 = enable; break;
        case TIM_1: TR1 = enable; break;
        case TIM_2: TR2 = enable; break;
        default: break;
    }
}

bool timer_is_overflow(TIM_NUM tim)
{
    switch (tim)
    {
        case TIM_0: return (bool) TF0;
        case TIM_1: return (bool) TF1;
        case TIM_2: return (bool) TF2;
        default: return false;
    }
}

uint16_t timer_get_value(TIM_NUM tim)
{
    switch (tim)
    {
        case TIM_0: return (TH0 << 8) | TL0;
        case TIM_1: return (TH1 << 8) | TL1;
        case TIM_2: return (TH2 << 8) | TL2;
        default: return 0x0000;
    }
}

void timer_set_value(TIM_NUM tim, uint16_t value)
{
    switch (tim)
    {
        case TIM_0:
            TL0 = value;
            TH0 = value >> 8;
            break;
        case TIM_1:
            TL1 = value;
            TH1 = value >> 8;
            break;
        case TIM_2:
            TL2 = value;
            TH2 = value >> 8;
            break;
        default: break;
    }
}

void delay(WORD ms)
{
    BYTE i;
    while (ms-- != 0)
        for (i = 0; i < 91; i++);
}

void delays(WORD s)
{
    BYTE i, j, k;
    while (s-- != 0)
    {
        i = 8, j = 1, k = 243;
        _nop_();
        do
        {
            do
            {
                while (--k);
            }
            while (--j);
        }
        while (--i);
    }
}
