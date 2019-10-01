#include "timer.h"

ulong max_time;
uint count;

void init_timer(enum TIMER tm, ulong ms)
{
	// 打开全局中断
	EA = 1;
	if (tm == tm0)
	{
		// 开启Timer0
	#ifdef MODE1T
		AUXR = 0x80; // 1T
	#endif
		ET0 = 1; // 打开Timer0中断
		TMOD = 0x01; // 16位自动重载
		// 装载Timer0
		TL0 = T1MS; // 低字节
		TH0 = T1MS >> 8; // 高字节
		// 启动Timer0
		TR0 = 1;
		// 初始化计数器
		max_time = ms;
		count = 0;
	}
	else if (tm == tm1)
	{
		// TODO 计时器1
	}
}

void tm0_isr() interrupt 1
{
	// 重载Timer0
	TL0 = T1MS; // 低字节
	TH0 = T1MS >> 8; // 高字节
	if (count-- == 0)
	{
		// 重置计数器
		count = max_time;
		// 调用监听器
		timer_actived(tm0);
	}
}

/**
 * 延时子程序
 * 11.0592MHZ
 */
void delay(uchar ms)
{
	uchar i;
	while (ms-- != 0)
		for (i = 0; i < 91; i++);
}
