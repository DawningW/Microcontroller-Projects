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
BYTE code bell[] = {0x01, 0x1b, 0x1d, 0x19, 0x1d, 0x1b, 0x01, 0x00};

WORD timer_count;
BYTE pos;
BYTE *next_char;

void uart_received(char *dat, size_t length)
{
    UNUSED(length);
    puts(dat);
}

void key_pressed(WORD key)
{
    BYTE key_num = key_get_num(key);
    lcd_disp(1, key_num, hex[key_num]);
    if (key_num == 13)
        power_reset(RST_Bootarea_AP);
    else if (key_num == 15)
        power_cmd(POWER_Idle);
    else if (key_num == 16)
        power_cmd(POWER_Down);
}

void init()
{
    EXTI_CONFIG exti = {0};
    TIM_CONFIG tm = {0};
    UART_CONFIG uart = {0};

    exti.trigger = EXTI_Trigger_Falling;
    exti_init(EXTI_1, &exti);
    exti_cmd(EXTI_1, true);

    timer_count = MAX_TIME;
    tm.mode = TIM_Mode_1;
    tm.enable_int = true;
    tm.value = T1MS;
    timer_init(TIM_0, &tm);
    timer_cmd(TIM_0, true);

    uart.baudrate = 9600;
    uart.baud_generator = TIM_1;
    uart.parity = UART_Parity_None;
    uart.priority = NVIC_Priority_1;
    uart.callback = uart_received;
    uart_init(&uart);

    nvic_enable();

    key_init(NULL);
    lcd_init();
}

void main()
{
    BYTE key;

    init();
    puts("STC89C52RC");
    puts("System init successfully!");     // 末尾无需换行符
    printf("%s is form printf\n", "This"); // 末尾需要加换行符
    for (pos = 1, next_char = helloworld; pos < MAX_DISPLAY_CHAR; pos++, next_char++)
    {
        if (*next_char == '\0') next_char = helloworld;
        lcd_disp(0, pos, *next_char);
    }

    while (true)
    {
        if (key = key_scan())
            key_pressed(key);
        delay(10);
    }
}

EXINT1
{
   P36 = ~P36; 
}

TIMER0
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
