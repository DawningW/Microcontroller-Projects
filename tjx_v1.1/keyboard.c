#include "keyboard.h"
#include "timer.h"

key_callback *key_pressed = NULL;

void key_init(key_callback *callback)
{
    EXTI_CONFIG exti = {0};
    
    KEY_PORT = 0x00;
    if (callback)
    {
        key_pressed = callback;
        exti.trigger = EXTI_Trigger_Falling;
        exti_init(EXTI_0, &exti);
        exti_cmd(EXTI_0, true);
    }
}

bool key_test(BYTE key, BYTE x, BYTE y)
{
    return GET_BIT(key, 7 - x) && GET_BIT(key, y);
}

BYTE key_get_num(BYTE key)
{
    BYTE x, y;
    for (x = 0; x < KEY_COLS; x++)
    {
        for (y = 0; y < KEY_ROWS; y++)
        {
            if (key_test(key, x, y))
            {
                return y * 4 + x + 1;
            }
        }
    }
    return 0;
}

/**
 * 线反转法扫描按键
 * 得到的按键码高位是列(x), 低位是行(y)
 */
BYTE key_scan()
{
    BYTE x, y;
    KEY_PORT = 0xf0;
    if ((KEY_PORT & 0xf0) != 0xf0)
    {
        delay(10); // 去抖动
        if ((KEY_PORT & 0xf0) != 0xf0)
        {
            x = ~KEY_PORT & 0xf0;
            KEY_PORT = 0x0f;
            y = ~KEY_PORT & 0x0f;
            while ((KEY_PORT & 0x0f) != 0x0f); // 松开检测
            return x | y;
        }
    }
    return 0;
}

// FIXME 中断有bug, 疑似开发板问题
void exint0() interrupt 0
{
    BYTE key;
    if (key = key_scan())
    {
        if (key_pressed)
            key_pressed(key);
    }
}
