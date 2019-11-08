#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"
#include "registers.h"
#include "interrupt.h"
#include "timer.h"

#define KEY_PORT P1

extern bit key_lock;
extern bit key_shift;
extern byte key;

extern void init_keyboard(void (*callback)(word));
extern void keypressed(enum INT);

#endif
