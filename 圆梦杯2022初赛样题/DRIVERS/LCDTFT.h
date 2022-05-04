#ifndef __LCDTFT_H
#define __LCDTFT_H

#include "config.h"
#include "GPIO.h"
// #include "SPI_WS.h"

typedef long s32;

#define TFTDataHigh P7
#define TFTDataLow  P6
#define TFTRWHigh() P01 = 1
#define TFTRWLow()  P01 = 0
#define TFTRSHigh() P02 = 1
#define TFTRSLow()  P02 = 0
#define TFTCSHigh() P00 = 1
#define TFTCSLow()  P00 = 0

void TFT_GPIO_config(void);
void TFT_Set_Window(u16 startX, u16 startY, u16 endX, u16 endY);
void TFT_SetCursor(u16 xCursor, u16 yCursor);
void TFT_WR_Data(u16 dat);
void TFT_DrawPoint(u16 x, u16 y, u16 color);
void TFT_FillRect(u16 startX, u16 startY, u16 endX, u16 endY, u16 color);
void TFT320_Clear(u16 color);
void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void TFT_ShowStr(u16 x, u16 y, u16 color, u16 bkColor, u8 *s);
void TFT_ShowAnyStr(u16 x, u16 y, u16 color, u16 bkColor, u8 *s);

#endif
