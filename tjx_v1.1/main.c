#include <STC89C5xRC.H>
#include <intrins.h>
#include "keyboard.h"
#include "1602A.h"
#include "interrupt.h"
#include "timer.h"

uchar code hex[][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
uchar code content[] = "HelloWorld! I'm 89C52RC running on TJX V1.1!   ";

uchar key;
uchar pos;
uchar *next_char;

void run();

void main()
{
	init_keyboard();
	init_lcd();
	init_int(int0);
	init_int(int1);
	init_timer(tm0, MAX_TIME);
	
	lcd_set_address(0, 0);
	for (pos = 1, next_char = content; pos < MAX_DISPLAY_CHAR; pos++, next_char++)
	{
		if (*next_char == '\0') next_char = content;
		lcd_disp(0, pos, *next_char);
	}
	
	while (1)
	{
		run();
	}
}

void run()
{
	if ((key = scankey()) > 0)
	{
		lcd_set_address(1, key - 1);
		lcd_print(hex[key - 1]);
	}
}

void int_actived(enum INT i)
{
	if (i == int0)
		P35 = ~P35;
	else if (i == int1)
		P36 = ~P36;
}

void timer_actived(enum TIMER tm)
{
	if (tm == tm0)
	{
		P37 = ~P37;
		
		if (pos == MAX_STORE_CHAR) pos = 0;
		if (*next_char == '\0') next_char = content;
		lcd_disp(0, pos, *next_char);
		pos++, next_char++;
		lcd_write_cmd(LCD_CMD_SHIFT | 0x8);
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
