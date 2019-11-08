#include "keyboard.h"
#include "1602A.h"

byte scankey();
byte getkey(byte, byte);

bit key_lock;
bit key_shift;
byte key;

void (*keyinput)(word);

void init_keyboard(void (*callback)(word key))
{
	struct IntItem item;
	item.trigger = 1;
	item.callback = &keypressed;
	init_int(int0, item);
	keyinput = callback;
	KEY_PORT = 0x00;
}

void keypressed(enum INT i)
{
	if ((key = scankey()) > 0)
	{
		if (key == 4)
			key_lock = !key_lock;
		else if (key == 13)
			key_shift = !key_shift;
		else if (keyinput != 0)
			keyinput(key);
	}
}

/**
 * 线反转法扫描按键
 * 得到的按键码高位是列(x),低位是行(y)
 */
byte scankey()
{
	byte xpos, ypos;
	KEY_PORT = 0xf0;
	if ((KEY_PORT & 0xf0) != 0xf0)
	{
		delay(10); // 去抖动
		if ((KEY_PORT & 0xf0) != 0xf0)
		{
			xpos = ~KEY_PORT & 0x0f;
			xpos >>= 4;
			xpos = 5 - xpos;
			KEY_PORT = 0x0f;
			ypos = ~KEY_PORT & 0xf0;
			
			while ((KEY_PORT & 0x0f) != 0x0f); // 松开检测
			
			return getkey(xpos, ypos);
		}
	}
	return 0;
}

byte code chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

byte getkey(byte x, byte y)
{
	lcd_disp(1, 1, chars[x]);
	lcd_disp(1, 3, chars[y]);
	return (y - 1) * 4 + x;
}
