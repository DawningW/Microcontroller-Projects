#include "keyboard.h"
#include "timer.h"

key_callback *key_pressed = NULL;
BYTE key;

void key_init(key_callback *callback)
{
    EXTI_CONFIG exti;
    
    KEY_PORT = 0x00;
    key_pressed = callback;
    exti.trigger = EXTI_Trigger_Falling;
    exti_init(EXTI_0, &exti);
    exti_cmd(EXTI_0, true);
}

static BYTE getkey(BYTE x, BYTE y)
{
    return (y - 1) * 4 + x;
}

/**
 * 线反转法扫描按键
 * 得到的按键码高位是列(x), 低位是行(y)
 */
BYTE scankey()
{
    BYTE x, y;
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
// FIXME 当前写法不适合中断

void exint0() interrupt 0
{
    if (key = scankey())
    {
        if (key_pressed)
            key_pressed(key);
    }
}
