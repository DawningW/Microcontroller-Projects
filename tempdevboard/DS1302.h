#ifndef __DS1302_H__
#define __DS1302_H__

#include "includes.h"

// 选择引脚
#define DS_SCLK P17
#define DS_IO P34
#define DS_CE P35

// 命令
#define DS_SECOND 0x80
#define DS_MINUTE 0x82
#define DS_HOUR   0x84
#define DS_DATE   0x86
#define DS_MONTH  0x88
#define DS_DAY    0x8A
#define DS_YEAR   0x8C
#define DS_WP     0x8E

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t dayofweek;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} ds_time_t;

void ds1302_init();
void ds1302_write(BYTE cmd, BYTE dat);
BYTE ds1302_read(BYTE cmd);
void ds1302_set_time(ds_time_t *tm);
void ds1302_get_time(ds_time_t *tm);

#endif
