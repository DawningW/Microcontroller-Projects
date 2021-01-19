#include "keyboard.h"

bit key_lock;
bit key_shift;
byte key;

void (*keyinput)(word) = NULL;

void keyboard_init(void (*callback)(word))
{
    struct InterruptItem item;
    item.trigger = 1;
    item.callback = keypressed;
    interrupt_init(INT_0, item);
    keyinput = callback;
    KEY_PORT = 0x00;
}

byte getkey(byte x, byte y)
{
    return (y - 1) * 4 + x;
}

/**
 * 线反转法扫描按键
 * 得到的按键码高位是列(x), 低位是行(y)
 */
byte scankey()
{
    byte x, y;
    KEY_PORT = 0xf0;
    if ((KEY_PORT & 0xf0) != 0xf0)
    {
        delay(10); // 去抖动
        if ((KEY_PORT & 0xf0) != 0xf0)
        {
            x = ~KEY_PORT & 0x0f;
            x >>= 4;
            x = 5 - x;
            KEY_PORT = 0x0f;
            y = ~KEY_PORT & 0xf0;
            while ((KEY_PORT & 0x0f) != 0x0f); // 松开检测
            return getkey(x, y);
        }
    }
    return 0;
}

void keypressed(enum Ints i)
{
    if (key = scankey())
    {
        if (key == 4)
            key_lock = !key_lock;
        else if (key == 13)
            key_shift = !key_shift;
        else if (keyinput)
            keyinput(key);
    }
}
