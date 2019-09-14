/*
 * 智能垃圾桶程序
 * 作者: 吴晨
 */
#include <STC12C5410AD.H>
#include "intrins.h"
#include "timer.h"

typedef bit BOOL;

#define TIME 6 // 开门时间(秒)
#define CLOSE 0 // 0为关
#define OPEN 1 // 1为开

#define up_open P10
#define up_close P11
#define down_open P12
#define down_close P13

BOOL up = CLOSE;
BOOL down = CLOSE;

WORD count = 0;

void delay();
void switchDoor();

// INT0, 0003H
void exint0() interrupt 0
{
	count = TIME * 1000; //reset counter
	up = OPEN;
	down = CLOSE;
	switchDoor();
}

// Timer0, 000BH
void tm0_isr() interrupt 1
{
	TL0 = T1MS; //reload timer0 low byte
	TH0 = T1MS >> 8; //reload timer0 high byte
	if (count-- <= 0) //1ms * 1000 -> 1s
	{
		count = TIME * 1000; //reset counter
		up = CLOSE;
		down = OPEN;
		switchDoor();
	}
}

void main()
{
	// 打开全局中断
	EA = 1; //open global interrupt switch
	// 开启INT0
	EX0 = 1; //enable INT0 interrupt
	IT0 = 1; //set INT0 int type (1:Falling, 0:Low level)
	// 开启Timer0
#ifdef MODE1T
  AUXR = 0x80; //timer0 work in 1T mode
#endif
	ET0 = 1; //enable timer0 interrupt
	TMOD = 0x01; //set timer0 as mode1 (16-bit)
	// 装载计时器
  TL0 = T1MS; //initial timer0 low byte
  TH0 = T1MS >> 8; //initial timer0 high byte
  TR0 = 1; //timer0 start running
	// 待机
	while (1)
	{
		INT0 = 1; //ready read INT0 port
    while (!INT0); //check INT0
    _nop_();
    _nop_();
    PCON = 0x02; //MCU power down
		_nop_();
    _nop_();
	}
}

void delay()
{
	BYTE i, j;
	_nop_();
	_nop_();
	i = 6;
	j = 210;
	do
	{
		while (--j);
	}
	while (--i);
}

void switchDoor()
{
	up_open = up;
	up_close = !up;
	down_open = down;
	down_close = !down;
}
