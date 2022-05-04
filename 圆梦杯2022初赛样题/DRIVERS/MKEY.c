#include "MKEY.h"
#include "delay.h"

void MKEY_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

char Mkey_Read(void)
{
	char val = -1;

	switch (P3)
	{
	case 0x11:
		val = 1;
		break;
	case 0x21:
		val = 2;
		break;
	case 0x41:
		val = 3;
		break;
	case 0x81:
		val = 100;
		break;

	case 0x12:
		val = 4;
		break;
	case 0x22:
		val = 5;
		break;
	case 0x42:
		val = 6;
		break;
	case 0x82:
		val = 101;
		break;

	case 0x14:
		val = 7;
		break;
	case 0x24:
		val = 8;
		break;
	case 0x44:
		val = 9;
		break;
	case 0x84:
		val = 102;
		break;

	case 0x18:
		val = 10;
		break;
	case 0x28:
		val = 0;
		break;
	case 0x48:
		val = 11;
		break;
	case 0x88:
		val = 103;
		break;
	
	default:
		break;
	}

	return val;
}