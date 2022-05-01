#include <stdio.h>
#include "system.h"
#include "power.h"
#include "isp.h"
#include "exti.h"
#include "timer.h"
#include "uart.h"
#include "keyboard.h"
#include "1602A.h"

#define MAX_TIME 1000

char code hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char code helloworld[] = "HelloWorld! I'm 89C52RC running on TJX V1.1!     ";
char code bell[] = {0x01, 0x1b, 0x1d, 0x19, 0x1d, 0x1b, 0x01, 0x00};

WORD timer_count;
bool key_lock;
bool key_shift;
BYTE pos;
BYTE *next_char;

void uart_received(char *dat, size_t length)
{
    UNUSED(length);
    uart_putstring(dat);
}

void key_pressed(WORD key)
{
    if (key == 4)
        key_lock = !key_lock;
    else if (key == 13)
        key_shift = !key_shift;
    lcd_disp(1, 5, hex[key - 1]);
}

void main()
{
    EXTI_CONFIG exti;
    TIM_CONFIG tm;
    UART_CONFIG uart;

    exti.trigger = EXTI_Trigger_Falling;
    exti_init(EXTI_1, &exti);
    exti_cmd(EXTI_1, true);

    timer_count = MAX_TIME;
    tm.mode = TIM_Mode_1;
    tm.enable_int = true;
    tm.priority = NVIC_Priority_1;
    tm.value = T1MS;
    timer_init(TIM_0, &tm);
    timer_cmd(TIM_0, true);

    uart.baudrate = 9600;
    uart.baud_generator = TIM_1;
    uart.parity = UART_Parity_None;
    uart.priority = NVIC_Priority_2;
    uart.callback = uart_received;
    uart_init(&uart);

    nvic_enable();

    key_init(key_pressed);
    lcd_init();

    uart_putstring("STC89C52RC\r\nUart has inited successfully!\r\n");
    for (pos = 1, next_char = helloworld; pos < MAX_DISPLAY_CHAR; pos++, next_char++)
    {
        if (*next_char == '\0') next_char = helloworld;
        lcd_disp(0, pos, *next_char);
    }

    while (true)
    {
        P35 = ~P35;
        delays(1);
    }
}

void exint1() interrupt 2
{
    P36 = ~P36;
}

void timer0() interrupt 1
{
    timer_set_value(TIM_0, T1MS);
    if (--timer_count == 0)
    {
        timer_count = MAX_TIME;
        P37 = ~P37;
        if (pos == MAX_STORE_CHAR) pos = 0;
        if (*next_char == '\0') next_char = helloworld;
        lcd_disp(0, pos, *next_char);
        pos++, next_char++;
        lcd_write_cmd(LCD_CMD_SHIFT | 0x8);
    }
}
