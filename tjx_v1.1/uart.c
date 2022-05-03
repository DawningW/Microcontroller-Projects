#include "uart.h"
#include <stdio.h>

#define BUFFER_SIZE 15

UART_PARITY parity;
uart_callback *uart_received = NULL;
char buffer[BUFFER_SIZE + 1];
size_t length;

static uint16_t get_generator_count(TIM_NUM tim, uint32_t baudrate)
{
    switch (tim)
    {
        case TIM_1: return 256 - (FOSC / 12 / 32 / baudrate);
        case TIM_2: return 65536 - (FOSC / 32 / baudrate);
        default: return 0;
    }
}

void uart_init(UART_CONFIG *uart)
{
    TIM_CONFIG tm = {0};
    
    ES = 0;
    parity = uart->parity;
    uart_received = uart->callback;
    length = 0;
    SM0 = uart->parity != UART_Parity_None;
    SM1 = uart->baudrate > 0;
    REN = uart->callback != NULL;
    TB8 = uart->parity != UART_Parity_Space;
    nvic_set_priority(4, uart->priority);
    ES = 1;
    if (!SM1 || uart->baud_generator == TIM_0)
        return;
    tm.mode = TIM_Mode_2;
    tm.value = tm.period = get_generator_count(uart->baud_generator, uart->baudrate);
    timer_init(uart->baud_generator, &tm);
    timer_cmd(uart->baud_generator, true);
}

void uart_multi_baudrate(bool enable)
{
    WRITE_REG_BIT(PCON, 7, enable);
}

// 0023H
void uart0() interrupt 4
{
    char c;
    if (RI)
    {
        c = SBUF;
        RI = 0;
        if (c != '\r')
        {
            if (c == '\n' || length >= BUFFER_SIZE)
            {
                buffer[length] = '\0';
                if (uart_received)
                    uart_received(buffer, length);
                length = 0;
            }
            if (c != '\n')
                buffer[length++] = c;
            // RB8为奇偶校验位
        }
    }
}

static void uart_send_char(char c)
{
    if (parity >= UART_Parity_Odd)
    {
        ACC = c;
        TB8 = P;
        if (parity == UART_Parity_Odd)
            TB8 = ~TB8;
    }
    SBUF = c;
    while (!TI);
    TI = 0;
}

void uart_send(const char *str)
{
    while (*str)
    {
        uart_send_char(*str++);
    }
}

// Implement stdio.h
char putchar(char c)
{
    if (c == '\n')
        uart_send_char('\r');
    uart_send_char(c);
    return c;
}

// 来自Keil的实现
/*
char _getkey()
{
    char c;

    while (!RI);
    c = SBUF;
    RI = 0;
    return c;
}
*/
