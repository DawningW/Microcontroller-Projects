#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "system.h"

/* 4*4矩阵键盘布局
 * ----------------------------------
 * |   1   |   2/↑   |  3  |  Shift |
 * ----------------------------------
 * |  4/←  | 5/Enter | 6/→ | Delete |
 * ----------------------------------
 * |   7   |   8/↓   |  9  |  Back  |
 * ----------------------------------
 * | Space |    0    |  .  |  Enter |
 * ----------------------------------
 */
#define KEY_PORT P1
#define KEY_ROWS 4
#define KEY_COLS 4

typedef void key_callback(BYTE);

void key_init(key_callback *callback);
bool key_test(BYTE key, BYTE x, BYTE y);
BYTE key_get_num(BYTE key);
BYTE key_scan();

#endif
