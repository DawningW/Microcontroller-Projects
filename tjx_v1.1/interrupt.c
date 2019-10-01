#include "interrupt.h"

void init_int(enum INT i)
{
	// 打开全局中断
	EA = 1;
	if (i == int0)
	{
		// 开启INT0
		EX0 = 1;
		IT0 = 1; // 下降沿触发
	}
	else if (i == int1)
	{
		// 开启INT1
		EX1 = 1;
		IT1 = 1; // 下降沿触发
	}
}

// 0003H
void exint0() interrupt 0
{
	int_actived(int0);
}

// 0013H
void exint1() interrupt 2
{
	int_actived(int1);
}
