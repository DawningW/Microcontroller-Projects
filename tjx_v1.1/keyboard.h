#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"

// TODO 按键码的定义

#define KEY_PORT P1

extern bit key_lock;
extern bit key_shift;
extern byte key;

extern void keyboard_init(void (*callback)(word key));
extern void keypressed(enum Ints);

#endif
