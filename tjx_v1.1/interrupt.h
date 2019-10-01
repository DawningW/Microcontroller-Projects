#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <STC89C5xRC.H>

enum INT { int0, int1 };

extern void init_int(enum INT i);

extern void int_actived(enum INT i);

#endif
