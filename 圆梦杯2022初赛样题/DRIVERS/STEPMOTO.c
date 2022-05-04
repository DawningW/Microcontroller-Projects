#include "STEPMOTO.h"
#include "delay.h"

void _delay_moto(u8 t)
{
	u8 i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < 10; j++);
}

void STEP_MOTO_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_InitStructure.Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);
}

void STEP_MOTO_Run(u16 speed)
{
	INA = 1;
	INB = 0;
	INC = 0;
	IND = 0;
	_delay_moto(speed);
	INA = 1;
	INB = 1;
	INC = 0;
	IND = 0;
	_delay_moto(speed);
	INA = 0;
	INB = 1;
	INC = 0;
	IND = 0;
	_delay_moto(speed);
	INA = 0;
	INB = 1;
	INC = 1;
	IND = 0;
	_delay_moto(speed);
	INA = 0;
	INB = 0;
	INC = 1;
	IND = 0;
	_delay_moto(speed);
	INA = 0;
	INB = 0;
	INC = 1;
	IND = 1;
	_delay_moto(speed);
	INA = 0;
	INB = 0;
	INC = 0;
	IND = 1;
	_delay_moto(speed);
	INA = 1;
	INB = 0;
	INC = 0;
	IND = 1;
	_delay_moto(speed);
}
