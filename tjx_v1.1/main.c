#include <STC89C5xRC.H>
#include <intrins.h>
#include "keyboard.h"
#include "1602A.h"
#include "interrupt.h"
#include "timer.h"

uchar code hex[][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

uchar key;

void run();

void main()
{
	init_keyboard();
	init_lcd();
	init_int(int0);
	init_timer(tm0, MAX_TIME);
	
	lcd_set_address(0, 0);
	lcd_print("HelloWorld! ");
	
	while (1)
	{
		run();
	}
}

void run()
{
	if ((key = scankey()) > 0)
	{
		lcd_print(hex[key - 1]);
	}
}

void int_actived(enum INT i)
{
	if (i == int0)
		P36 = !P36;
}

void timer_actived(enum TIMER tm)
{
	if (tm == tm0)
	{
		P37 = !P37;
		lcd_write_cmd(LCD_CMD_SHIFT | 0xc);
	}
}

// @11.0592MHz
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
