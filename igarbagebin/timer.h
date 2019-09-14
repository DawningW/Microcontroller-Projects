#ifndef __TIMER_H_
#define __TIMER_H_

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L
#define MODE1T //Timer clock mode, comment this line is 12T mode, uncomment is 1T mode

#ifdef MODE1T
#define T1MS (65536-FOSC/1000) //1ms timer calculation method in 1T mode
#else
#define T1MS (65536-FOSC/12/1000) //1ms timer calculation method in 12T mode
#endif

#endif
