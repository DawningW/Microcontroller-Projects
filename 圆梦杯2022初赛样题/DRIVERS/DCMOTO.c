#include "DCMOTO.h"

#define T1MS (65536 - MAIN_Fosc / 1000)

void DCMOTO_GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_OUT_PP;
    GPIO_InitStructure.Pin = GPIO_Pin_6;
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

void Time0_16_Config_Init(void)
{
    AUXR |= 0x80;
    // AUXR &= 0x7f;
    TMOD = 0x00;
    TL0 = T1MS;
    TH0 = T1MS >> 8;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
}

void DCMoto_Sta(u8 sta, u8 dir)
{

    switch (sta)
    {
    case 0:
        TR0 = 0;
        IN1 = 0;
        IN2 = 0;
        break;
    case 1:
        TR0 = 1;
        switch (dir)
        {
        case 1:
            IN1 = 1;
            IN2 = 0;
            break;
        case 2:
            IN1 = 0;
            IN2 = 1;
            break;
        }
        break;
    }
}

// void tm0_isr() interrupt 1 using 1
// {
//     PWM_DC = !PWM_DC;
// }
