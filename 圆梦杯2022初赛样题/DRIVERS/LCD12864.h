#ifndef __LCD12864_H
#define __LCD12864_H

#include "config.h"
#include "GPIO.h"

#define Lcd12864ENHigh() P00 = 1
#define Lcd12864ENLow() P00 = 0
#define Lcd12864RSHigh() P01 = 1
#define Lcd12864RSLow() P01 = 0
#define Lcd12864RWHigh() P02 = 1
#define Lcd12864RWLow() P02 = 0
#define Lcd12864CS1High() P03 = 1
#define Lcd12864CS1Low() P03 = 0
#define Lcd12864CS2High() P04 = 1
#define Lcd12864CS2Low() P04 = 0

#define Lcd12864Data() P6
#define Lcd12864DataIn() P6M1 = 0xFF
#define Lcd12864DataOut() P6M1 = 0x00
#define Lcd12864WriteData(dat) P6 = dat

void LCD12864_GPIO_config(void);
void LCD12864_init(void);
void LCD12864_clear(void);
void LCD12864_showXY(u8 row, u8 col, u8 x, u8 y, const u8 *tab);
void LCD12864_showStr(u8 row, u8 col, const u8 *s);

#endif
