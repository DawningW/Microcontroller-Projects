#ifndef _DELAY_H
#define _DELAY_H

#include <msp430.h>
#include <stdint.h>

void delay(uint16_t nClk);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif // _DELAY_H
