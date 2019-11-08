#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "system.h"
#include "registers.h"

enum INT { int0, int1 };

struct IntItem
{
	byte trigger;
	void (*callback)(enum INT);
};

extern void init_int(enum INT i, struct IntItem item);

#endif
