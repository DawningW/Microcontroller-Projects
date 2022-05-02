#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"

#define KEY_PORT P1
#define KEY_ROWS 4
#define KEY_COLS 4

typedef void key_callback(BYTE);

void key_init(key_callback *callback);
bool key_test(BYTE key, BYTE x, BYTE y);
BYTE key_get_num(BYTE key);
BYTE key_scan();

#endif
