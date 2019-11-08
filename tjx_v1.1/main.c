#include <intrins.h>
#include "system.h"
#include "registers.h"
#include "keyboard.h"
#include "1602A.h"
#include "interrupt.h"
#include "timer.h"

byte code hex[][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
byte code content[] = "HelloWorld! I'm 89C52RC running on TJX V1.1!   ";
byte code bell[] = {0x01, 0x1b, 0x1d, 0x19, 0x1d, 0x1b, 0x01, 0x00};

byte pos;
byte *next_char;

void run();
void key_input(word);
void int_actived(enum INT);
void timer_actived(enum TIMER tm);

void main()
{
	struct IntItem iitem;
	struct TimerItem titem;
	init_keyboard(&key_input);
	lcd_init();
	iitem.trigger = 1;
	iitem.callback = &int_actived;
	init_int(int1, iitem);
	titem.max_time = MAX_TIME;
	titem.callback = &timer_actived;
	init_timer(tm0, titem);
	
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

}

void key_input(word key)
{
	lcd_set_pos(1, 5);
	lcd_print(hex[key - 1]);
}

void int_actived(enum INT i)
{
	if (i == int1)
	{
		// 唤醒休眠
	}
}

void timer_actived(enum TIMER tm)
{
	if (tm == tm0 && 0)
	{
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
