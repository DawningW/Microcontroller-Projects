#include "app_main.h"
#include "stm32f4xx.h"
#include "delay.h"

void app_init()
{
	
}

void app_loop()
{
	// kuhanshu
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9 | GPIO_PIN_9, GPIO_PIN_RESET);
	delay_ms(500);
	// weidai
	
	// jicunqi
	
}
