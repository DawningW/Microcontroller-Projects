/*
 * 测试程序
 * 作者: 吴晨
 */
#include <STC12C5410AD.H>

void delay()
{
	_nop_();
	_nop_();

	unsigned char i = 6, j = 210;
	do
	{
		while (--j);
	}
	while (--i);
}

void main()
{
	
}