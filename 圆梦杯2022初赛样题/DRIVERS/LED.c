#include "LED.h"
#include "delay.h"

void LED_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);
}

void LED_Switch(u8 led)
{
	LED_RED = 0;
    LED_GREEN = 0;
    LED_YELLOW = 0;
	switch (led)
	{
	default:
	case 0:
		LED_RED = 1;
		break;
	case 1:
		LED_GREEN = 1;
		break;
	case 2:
		LED_YELLOW = 1;
		break;
	}
}
