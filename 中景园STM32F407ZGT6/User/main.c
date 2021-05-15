#include "system.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"

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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}

void timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 15 - 1; // 10000 - 1
    TIM_TimeBaseInitStructure.TIM_Prescaler = 35 - 1; // 8400 - 1
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3, ENABLE);
}

uint8_t lastkey = 0;
uint8_t brightness = 7;
uint16_t tick = 0;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    led_init();
    key_init();
    oled_init();
    uart_init(9600);
    timer_init();
    
    uart_println("Hello, I'm STM32F407ZGT6!");
    oled_disp_string(16, 2, "Hello World!");
    oled_disp_string(20, 6, "Author: WC");
    
    while (1)
    {
        char str_temp[8];
        sprintf(str_temp, "led: %02d", brightness);
        oled_disp_string(66, 4, str_temp);
        int temp = key_scan();
        if (lastkey != temp)
        {
            lastkey = temp;
            printf("Update key state: %d\n", lastkey);
            char str[8];
            sprintf(str, "Key: %d", lastkey);
            oled_disp_string(8, 4, str);
        }
        // if (lastkey) GPIO_ResetBits(GPIOF, GPIO_Pin_10);
        // else GPIO_SetBits(GPIOF, GPIO_Pin_10);
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
        // LED0 = !LED0;
        if (tick < brightness)
        {
            LED1 = 0;
        }
        else
        {
            LED1 = 1;
        }
        if (++tick > 15) tick = 0;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void EXTI0_IRQHandler(void)
{
    delay_ms(10);
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        // 唤醒
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI3_IRQHandler(void)
{
    delay_ms(10);
    if (KEY1 == 0) // 大
    {
        if (brightness < 15) ++brightness;
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
    delay_ms(10);
    if (KEY0 == 0) // 小
    {
        if (brightness > 0) --brightness;
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
}
