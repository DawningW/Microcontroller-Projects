#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <STC89C5xRC.H>

#define uchar unsigned char
#define uint unsigned int

enum INT { int0, int1 };

struct IntItem
{
	uchar trigger;
	void (*callback)(enum INT);
};

extern void init_int(enum INT i, struct IntItem item);

#endif
