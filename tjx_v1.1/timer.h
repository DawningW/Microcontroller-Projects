#ifndef __TIMER_H__
#define __TIMER_H__

#include <STC89C5xRC.H>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

#define MAX_TIME 1000

#define FOSC 11059200L
// #define MODE1T //Timer clock mode, comment this line is 12T mode, uncomment is 1T mode

#ifdef MODE1T
#define T1MS (65536-FOSC/1000) //1ms timer calculation method in 1T mode
#else
#define T1MS (65536-FOSC/12/1000) //1ms timer calculation method in 12T mode
#endif

enum TIMER { tm0, tm1 };

extern void init_timer(enum TIMER tm, ulong ms);
extern void delay(uchar ms);

extern void timer_actived(enum TIMER tm);
	
#endif
