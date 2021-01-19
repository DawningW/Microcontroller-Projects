#ifndef _SYSTEM_H_
#define _SYSTEM_H_

typedef unsigned char byte;
typedef unsigned int word;
#define ulong unsigned long

#define NULL ((void*) 0)
#define false 0
#define true 1

// 外部晶振频率11.0592MHZ
#define FOSC 11059200L

#include <intrins.h>
#include "registers.h"
#include "interrupt.h"
#include "timer.h"

#endif
