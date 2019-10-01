#include "keyboard.h"
#include "timer.h"

void init_keyboard()
{
	KEY_PORT = 0xff;
}

/**
 * 线反转法扫描按键
 * 得到的按键码高位是列,低位是行
 */
uchar scankey()
{
	uchar temp, result;
	KEY_PORT = 0xf0;
	if ((KEY_PORT & 0xf0) != 0xf0)
	{
		delay(10); // 去抖动
		if ((KEY_PORT & 0xf0) != 0xf0)
		{
			temp = KEY_PORT;
			KEY_PORT = 0x0f;
			result = temp | KEY_PORT;
			
			while ((KEY_PORT & 0x0f) != 0x0f); // 松开检测
			
			switch (result)
			{
				default: return 0;
				
				case 0x7e: return 1;
				case 0xbe: return 2;
				case 0xde: return 3;
				case 0xee: return 4;
				
				case 0x7d: return 5;
				case 0xbd: return 6;
				case 0xdd: return 7;
				case 0xed: return 8;
				
				case 0x7b: return 9;
				case 0xbb: return 10;
				case 0xdb: return 11;
				case 0xeb: return 12;
				
				case 0x77: return 13;
				case 0xb7: return 14;
				case 0xd7: return 15;
				case 0xe7: return 16;
			}
		}
	}
	return 0;
}
