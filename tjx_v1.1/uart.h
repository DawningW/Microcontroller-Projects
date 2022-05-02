#ifndef __UART_H__
#define __UART_H__

#include "system.h"

#include "exti.h"
#include "timer.h"

typedef void uart_callback(char*, size_t);

// 串口工作模式
// 0: 8位同步移位寄存器, 波特率为fosc/12
// 1: 8位, 波特率可变, 为2^SMOD*定时器1或2的溢出率
// 2: 9位, 波特率固定, 为2^SMOD*fosc/64
// 3: 9位, 波特率可变, 为2^SMOD*定时器1或2的溢出率

// 奇偶校验方式, 仅当工作模式为2或3时可用
typedef enum
{
    UART_Parity_None = 0x0, // 无奇偶校验
    UART_Parity_Odd,        // 奇校验
    UART_Parity_Even,       // 偶校验
    UART_Parity_Space,      // 校验位为0
    UART_Parity_Mark        // 校验位为1
} UART_PARITY;

typedef struct
{
    uint32_t       baudrate;
    TIM_NUM        baud_generator;
    UART_PARITY    parity;
    NVIC_PRIORITY  priority;
    uart_callback* callback;
} UART_CONFIG;

void uart_init(UART_CONFIG *uart);
void uart_multi_baudrate(bool enable); // 尚未完全实现
void uart_send(const char *str);

#endif
