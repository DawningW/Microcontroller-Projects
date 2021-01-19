#include "timer.h"

struct TimerItem timer0_item;

void timer_init(enum Timers tm, struct TimerItem item)
{
    EA = 0;
#ifdef MODE1T
    AUXR = 0x80; // 1T
#endif
    if (tm == TM_0)
    {
        // 初始化计数器
        timer0_item = item;
        timer0_item.count = timer0_item.max_time;
        if (timer0_item.callback)
            ET0 = 1; // 打开Timer0中断
        // 16位定时器模式(TMOD^1 = 0, TMOD^0 = 1)
        TMOD &= 0xFC;
        TMOD |= 0x01;
        // 装载Timer0
        TL0 = T1MS; // 低字节
        TH0 = T1MS >> 8; // 高字节
        // 启动Timer0
        TR0 = 1;
    }
    else if (tm == TM_1)
    {
        // TODO 定时器1用做串口波特率发生器, 将初始化代码移至此处
    }
    else if (tm == TM_2)
    {
        // TODO 计时器2
    }
    EA = 1; // 打开全局中断
}

// 000BH
void timer0() interrupt 1
{
    // 重载Timer0
    TL0 = T1MS; // 低字节
    TH0 = T1MS >> 8; // 高字节
    if (--timer0_item.count == 0)
    {
        // 重置计数器
        timer0_item.count = timer0_item.max_time;
        // 调用监听器
        if (timer0_item.callback)
            timer0_item.callback(TM_0);
    }
}

/*
// 001BH
void timer1() interrupt 3
{

}
*/

// 002BH
void timer2() interrupt 5
{

}

/**
 * 延时子程序
 */
void delay(word ms)
{
    byte i;
    while (ms-- != 0)
        for (i = 0; i < 91; i++);
}

void delays(word s)
{
    byte i, j, k;
    while (s-- != 0)
    {
        i = 8, j = 1, k = 243;
        _nop_();
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
}
