#include "system.h"
#include "delay.h"
#include "usart.h"

#define LED0 PFout(9)
#define LED1 PFout(10)

#define KEY0 PEin(4)
#define KEY1 PEin(3)
#define WK_UP PAin(0)

uint8_t key_scan(void)
{
    int temp = 0;
    if (KEY0 == 0 || KEY1 == 0)
    {
        delay_ms(10); // 去抖动
        temp = !KEY0 | (!KEY1 << 1);
    }
    return temp;
}

void led_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
}

void key_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 8400 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3, ENABLE);
}

int main(void)
{
    led_init();
    key_init();
    uart_init(9600);
    delay_init();
    timer_init();
    uart_println("Hello, I'm STM32F407ZGT6!");
    while (1)
    {
        int temp = key_scan();
        if (temp)
        {
            GPIO_ResetBits(GPIOF, GPIO_Pin_10);
        }
        else
        {
            GPIO_SetBits(GPIOF, GPIO_Pin_10);
        }
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            delay_ms(10); // 去抖动
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
            {
                printf("WK_UP had been pressed. Key state: %d\n", temp);
                delay_ms(200);
            }
        }
        if (USART_RX_STA & 0x8000)
        {
            uint8_t len = USART_RX_STA & 0x3fff;
            USART_RX_BUF[len] = '\0';
            uart_println((char*) USART_RX_BUF);
            USART_RX_STA = 0;
        }
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        LED0 = !LED0;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
