#include "serial.h"

void (*receive)(byte) = NULL;
bit busy;

void serial_init(void (*callback)(byte))
{
    EA = 0;
    receive = callback;
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50; // 8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xDA; // 9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xD2; // 9-bit variable UART, parity bit initial to 0
#endif
    ES = 1; // Enable UART interrupt
    // Set Timer1 as 8-bit auto reload mode(TMOD^5 = 1, TMOD^4 = 0)
    TMOD &= 0xCF;
    TMOD |= 0x20;
    TH1 = TL1 = -(FOSC / 12 / 32 / BAUD); // Set auto-reload value
    TR1 = 1; // Timer1 start run
    EA = 1; // 打开全局中断
}

// 0023H
void UART() interrupt 4
{
    if (RI)
    {
        RI = 0; // Clear receive interrupt flag
        if (receive)
            receive(SBUF); // Call function to receive data
        // RB8 get the parity bit
    }
    if (TI)
    {
        TI = 0; // Clear transmit interrupt flag
        busy = 0; // Clear transmit busy flag
    }
}

void serial_putchar(byte dat)
{
    while (busy);
    busy = 1;
    ACC = dat;
#if (PARITYBIT == ODD_PARITY)
    TB8 = ~P;
#elif (PARITYBIT == EVEN_PARITY)
    TB8 = P;
#endif
    SBUF = ACC;
}

void serial_putstring(const char *str)
{
    while (*str)
    {
        serial_putchar(*str++);
    }
}

void serial_printf(const char *str, ...)
{
    serial_putstring(str);
    // 实现不了了啊
}
