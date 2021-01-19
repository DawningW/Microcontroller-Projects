#ifndef __TIMER_H__
#define __TIMER_H__

#include "system.h"

// Timer clock mode, comment this line is 12T mode, uncomment is 1T mode
// #define MODE1T

#ifdef MODE1T
#define T1MS (65536 - FOSC / 1000) // 1ms timer calculation method in 1T mode
#else
#define T1MS (65536 - FOSC / 12 / 1000) // 1ms timer calculation method in 12T mode
#endif

#define MAX_TIME 1000

enum Timers { TM_0, TM_1, TM_2 };

// TODO 定时器模式可选择
struct TimerItem
{
    ulong max_time;
    word count;
    void (*callback)(enum Timers);
};

extern void timer_init(enum Timers tm, struct TimerItem item);
extern void delay(word ms);
extern void delays(word s);
    
#endif
