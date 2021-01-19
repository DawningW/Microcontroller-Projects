#include "system.h"
#include "serial.h"
#include "keyboard.h"
#include "1602A.h"

byte code hex[][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
byte code content[] = "HelloWorld! I'm 89C52RC running on TJX V1.1!     ";
byte code bell[] = {0x01, 0x1b, 0x1d, 0x19, 0x1d, 0x1b, 0x01, 0x00};
byte code info[] = "STC89C52RC\r\nUart has inited successfully!\r\n";

byte pos;
byte *next_char;

void run();
void int_actived(enum Ints);
void timer_actived(enum Timers);
void serial_receive(byte);
void key_input(word);

void main()
{
    struct InterruptItem intItem;
    struct TimerItem tmItem;

    intItem.trigger = 1;
    intItem.callback = int_actived;
    interrupt_init(INT_1, intItem);

    tmItem.max_time = MAX_TIME;
    tmItem.callback = timer_actived;
    timer_init(TM_0, tmItem);

    serial_init(serial_receive);
    serial_putstring(info);
    keyboard_init(key_input);
    lcd_init();

    for (pos = 1, next_char = content; pos < MAX_DISPLAY_CHAR; pos++, next_char++)
    {
        if (*next_char == '\0') next_char = content;
        lcd_disp(0, pos, *next_char);
    }

    while (1)
    {
        run();
    }
}

void run()
{
    // TODO 啥也没有
}

void int_actived(enum Ints i)
{
    if (i == INT_1)
    {
		P36 = ~P36;
    }
}

void timer_actived(enum Timers tm)
{
    if (tm == TM_0)
    {
        // 定时器0
        P37 = ~P37;

        if (pos == MAX_STORE_CHAR) pos = 0;
        if (*next_char == '\0') next_char = content;
        lcd_disp(0, pos, *next_char);
        pos++, next_char++;
        lcd_write_cmd(LCD_CMD_SHIFT | 0x8);
    }
}

void serial_receive(byte dat)
{
    P2 = ~dat;
}

void key_input(word key)
{
    P35 = ~P35;

    lcd_set_pos(1, 5);
    lcd_print(hex[key - 1]);
}
