#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "system.h"

/* Define UART parity mode */
#define NONE_PARITY 0
#define ODD_PARITY 1
#define EVEN_PARITY 2
#define MARK_PARITY 3
#define SPACE_PARITY 4

#define BAUD 9600 // UART baudrate
#define PARITYBIT EVEN_PARITY // UART parity mode

extern void serial_init(void (*callback)(byte));
extern void serial_putchar(byte dat);
extern void serial_putstring(const char *str);
extern void serial_printf(const char *str, ...);

#endif
