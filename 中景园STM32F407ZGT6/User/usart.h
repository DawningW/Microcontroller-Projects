#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "system.h"

#define USART_BUF_LEN 255

extern uint8_t USART_RX_BUF[USART_BUF_LEN + 1];
extern uint16_t USART_RX_STA;

void uart_init(uint32_t bound);
void uart_send(uint8_t c);
void uart_print(const char *str);
void uart_println(const char *str);

#endif
