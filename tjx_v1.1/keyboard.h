#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"

#define KEY_PORT P1

typedef void key_callback(BYTE);

void key_init(key_callback *callback);

#endif
