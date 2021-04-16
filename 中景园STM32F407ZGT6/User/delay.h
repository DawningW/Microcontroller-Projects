#ifndef __DELAY_H
#define __DELAY_H
#include "system.h"

void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_s(float ns);

#endif
