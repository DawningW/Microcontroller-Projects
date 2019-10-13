#include "interrupt.h"
#include "keyboard.h"

struct IntItem int0_item;
struct IntItem int1_item;

void init_int(enum INT i, struct IntItem item)
{
	// 打开全局中断
	EA = 1;
	if (i == int0)
	{
		// 开启INT0
		EX0 = 1;
		IT0 = item.trigger; // 下降沿触发
		int0_item = item;
	}
	else if (i == int1)
	{
		// 开启INT1
		EX1 = 1;
		IT1 = item.trigger; // 下降沿触发
		int1_item = item;
	}
}

// 0003H
void exint0() interrupt 0
{
	if (int0_item.callback != 0)
		int0_item.callback(int0);
}

// 0013H
void exint1() interrupt 2
{
	if (int1_item.callback != 0)
		int1_item.callback(int1);
}
