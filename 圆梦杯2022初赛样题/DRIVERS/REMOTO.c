#include "REMOTO.h"

#define T100US (65536 - Main_Fosc_KHZ / 100) // 1T模式
#define kServoPWMCountMax 2000

u16 ServoPWMCount = 0;
u16 ServoHighPulse = 50; // 50-250

void REMOTO_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_6;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
}

void REMOTO_Start(void)
{
	AUXR |= 0x40;
	TMOD = 0x00;
	TL1 = T100US;
	TH1 = T100US >> 8;
	TR1 = 1;
	ET1 = 1;
	EA = 1;
}

void REMOTO_Stop(void)
{
	TR1 = 0;
	ET1 = 0;
}

// 50us一次中断
void tm1_isr() interrupt 3
{
	ServoPWMCount++;
	if (ServoPWMCount >= kServoPWMCountMax)
		ServoPWMCount = 0;
	PWM_REMOTO = ServoPWMCount < ServoHighPulse;
}
