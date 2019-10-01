#include "1602A.h"
uchar code bell[] = {0x01, 0x1b, 0x1d, 0x19, 0x1d, 0x1b, 0x01, 0x00};

//@11.0592MHz
static void delay1s()
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
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

void main()
{
	lcd_init();
	lcd_write_cgram(0, bell);
	
	lcd_set_address(0, 0);
	lcd_print("LCD1602A TEST!");
	lcd_disp(1, 0, 0);
	lcd_set_address(1, 1);
	lcd_print(" Run on 89C52");
	
	delay1s();
	delay1s();
	delay1s();
	delay1s();
	delay1s();
	
	while (1)
	{
		lcd_write_cmd(LCD_CMD_CLR);
		delay1s();
		lcd_set_address(0, 3);
		lcd_print("HelloWorld!");
		delay1s();
	}
}
