#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include <stm32f4xx_conf.h>

// 最大接收字节数
#define USART_BUF_LEN 255

// 接收缓冲, 最大USART_REC_LEN个字节, 末字节为换行符
extern uint8_t USART_RX_BUF[USART_BUF_LEN + 1];
// 接收状态标记
// bit15 接收完成标志
// bit14 接收到0x0d
// bit13~0 接收到的有效字节数目
extern uint16_t USART_RX_STA;

void uart_init(uint32_t bound);
#endif
