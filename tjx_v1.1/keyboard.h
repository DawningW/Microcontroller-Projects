#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <STC89C5xRC.H>

#define uchar unsigned char
#define uint unsigned int

#define KEY_PORT P1

extern void init_keyboard();
extern uchar scankey();

#endif
