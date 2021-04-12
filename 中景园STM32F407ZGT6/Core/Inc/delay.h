#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f4xx.h"

void delay_init();
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
#endif
