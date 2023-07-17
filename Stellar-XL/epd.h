#ifndef _EPD_H
#define _EPD_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define EPD_WIDTH  200
#define EPD_WIDTH_FULL (EPD_WIDTH * 2)
#define EPD_HEIGHT 300
#define EPD_BUFFER_SIZE 7500 // (EPD_WIDTH * EPD_HEIGHT / 8)

// P3.4
#define EPD_CS1_LOW P3OUT &= ~BIT4
#define EPD_CS1_HIGH P3OUT |= BIT4
// P3.7 在其他价签中是空(unknow), 在这里是从IC的片选CS2
#define EPD_CS2_LOW P3OUT &= ~BIT7
#define EPD_CS2_HIGH P3OUT |= BIT7
// P2.4
#define EPD_SDA_LOW P2OUT &= ~BIT4
#define EPD_SDA_HIGH P2OUT |= BIT4
// P2.3
#define EPD_SCL_LOW P2OUT &= ~BIT3
#define EPD_SCL_HIGH P2OUT |= BIT3
// P2.5
#define EPD_BUSY (P2IN & BIT5)
// P3.6
#define EPD_RST_LOW P3OUT &= ~BIT6
#define EPD_RST_HIGH P3OUT |= BIT6
// P3.5
#define EPD_DC_LOW P3OUT &= ~BIT5
#define EPD_DC_HIGH P3OUT |= BIT5
// P3.1
#define EPD_BS_LOW P3OUT &= ~BIT1
#define EPD_BS_HIGH P3OUT |= BIT1
// P2.6
#define EPD_POWER_ON P2OUT &= ~BIT6
#define EPD_POWER_OFF P2OUT |= BIT6

void EPD_Init();
void EPD_Sleep();
bool EPD_IsBusy();
void EPD_Clear();
void EPD_WriteLeft(const uint8_t *dat);
void EPD_WriteRight(const uint8_t *dat);
// 暂时不支持局部刷新
void EPD_Refresh();

#endif // _EPD_H
