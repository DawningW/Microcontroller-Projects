#ifndef __LCD1602_H
#define __LCD1602_H

#include "config.h"
#include "GPIO.h"

#define Lcd1602ENHigh() P00 = 1
#define Lcd1602ENLow() P00 = 0
#define Lcd1602RSHigh() P01 = 1
#define Lcd1602RSLow() P01 = 0
#define Lcd1602RWHigh() P02 = 1
#define Lcd1602RWLow() P02 = 0
#define Lcd1602Busy() P03

#define Lcd1602Data() P6
#define Lcd1602DataIn() \
    P6M0 = 0xFF;        \
    P6M1 = 0xFF
#define Lcd1602DataOut() \
    P6M0 = 0x00;         \
    P6M1 = 0x00
#define Lcd1602WriteData(dat) P6 = dat

#define CMD_clear 0x01   // �����Ļ
#define CMD_back 0x02    // DDRAM����λ
#define CMD_dec1 0x04    // �����AC��ָ�룩��1������д
#define CMD_add1 0x06    // �����AC��ָ�룩��1������д
#define CMD_dis_gb3 0x0c // ����ʾ_�ع��_�ع����˸
#define CMD_set82 0x38   // 8λ����_2����ʾ

void LCD1602_GPIO_config(void);
void LCD1602_init(void);
void LCD1602_clear(void);
void LCD1602_show_char(u8 pos, u8 dat);
void LCD1602_show_str(u8 pos, u8 *str);

#endif
