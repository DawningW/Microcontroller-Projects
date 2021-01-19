#include "interrupt.h"

// PS 为啥不用typedef呢 =_=
// typedef void (*_CALLBACK)(enum Ints);
void (*callbacks[4])(enum Ints) = {NULL, NULL, NULL, NULL};

void interrupt_init(enum Ints i, struct InterruptItem item)
{
    EA = 0;
    if (i == INT_0)
    {
        IT0 = item.trigger;
        EX0 = 1;
    }
    else if (i == INT_1)
    {
        IT1 = item.trigger;
        EX1 = 1;
    }
    else if (i == INT_2)
    {
        IT2 = item.trigger;
        EX2 = 1;
    }
    else if (i == INT_3)
    {
        IT3 = item.trigger;
        EX3 = 1;
    }
    callbacks[i] = item.callback;
    EA = 1; // 打开全局中断
}

// 0003H
void exint0() interrupt 0
{
    if (callbacks[INT_0])
        callbacks[INT_0](INT_0);
}

// 0013H
void exint1() interrupt 2
{
    if (callbacks[INT_1])
        callbacks[INT_1](INT_1);
}

// 0033H
void exint2() interrupt 6
{
    if (callbacks[INT_2])
        callbacks[INT_2](INT_2);
}

// 003BH
void exint3() interrupt 7
{
    if (callbacks[INT_3])
        callbacks[INT_3](INT_3);
}
