#ifndef __TIMER_H__
#define __TIMER_H__

#include "system.h"
#include "registers.h"

#define MAX_TIME 1000

#define FOSC 11059200L
//Timer clock mode, comment this line is 12T mode, uncomment is 1T mode
// #define MODE1T

#ifdef MODE1T
#define T1MS (65536-FOSC/1000) //1ms timer calculation method in 1T mode
#else
#define T1MS (65536-FOSC/12/1000) //1ms timer calculation method in 12T mode
#endif

enum TIMER { tm0, tm1 };

struct TimerItem
{
	ulong max_time;
	word count;
	void (*callback)(enum TIMER);
};

extern void init_timer(enum TIMER tm, struct TimerItem item);
extern void delay(byte ms);
	
#endif
