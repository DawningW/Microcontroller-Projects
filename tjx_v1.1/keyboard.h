#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <STC89C5xRC.H>
#include "interrupt.h"
#include "timer.h"

#define uchar unsigned char
#define uint unsigned int

#define KEY_PORT P1

extern bit key_lock;
extern bit key_shift;
extern uchar key;

extern void init_keyboard(void (*callback)(uint));
extern void keypressed(enum INT);

#endif
