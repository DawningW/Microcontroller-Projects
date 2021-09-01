#include "delay.h"
#include "tim.h"

void delay_us(uint32_t us)
{
    __HAL_TIM_SetCounter(&htim5, 0);
    HAL_TIM_Base_Start(&htim5);
    while (__HAL_TIM_GetCounter(&htim5) < us);
    HAL_TIM_Base_Stop(&htim5);
}

void delay_ms(uint16_t ms)
{
    delay_us(ms * 1000);
}

void delay_s(float s)
{
    uint32_t ms = (uint16_t) (s * 1000);
    uint8_t repeat = ms / UINT16_MAX;
    uint16_t remain = ms % UINT16_MAX;
    while (repeat)
    {
        delay_ms(UINT16_MAX);
        repeat--;
    }
    if (remain) delay_ms(remain);
}
