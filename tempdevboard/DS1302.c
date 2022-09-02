#include "DS1302.h"

// 十进制转BCD码
#define DEC_TO_BCD(x) (((x) / 10) << 4) | ((x) % 10)
// BCD码转十进制
#define BCD_TO_DEC(x) (((x) >> 4) * 10 + ((x) & 0x0F))

void ds1302_init()
{
    DS_CE = 0;
    DS_SCLK = 0;
}

void ds1302_write(BYTE cmd, BYTE dat)
{
    BYTE i;
    DS_CE = 1;
    for (i = 0; i < 8; i++)
    {
        DS_IO = cmd & (0x01 << i);
        DS_SCLK = 1;
        DS_SCLK = 0;
    }
    for (i = 0; i < 8; i++)
    {
        DS_IO = dat & (0x01 << i);
        DS_SCLK = 1;
        DS_SCLK = 0;
    }
    DS_CE = 0;
}

BYTE ds1302_read(BYTE cmd)
{
    BYTE i, dat = 0x00;
    cmd |= 0x01; // 将指令转换为读指令
    DS_CE = 1;
    for (i = 0; i < 8; i++)
    {
        DS_IO = cmd & (0x01 << i);
        DS_SCLK = 0;
        DS_SCLK = 1;
    }
    for (i = 0; i < 8; i++)
    {
        DS_SCLK = 1;
        DS_SCLK = 0;
        if (DS_IO)
        {
            dat |= (0x01 << i);
        }
    }
    DS_CE = 0;
    DS_IO = 0; // 不置为0读出的数据会出错
    return dat;
}

void ds1302_set_time(ds_time_t *tm)
{
    ds1302_write(DS_WP, 0x00);
    ds1302_write(DS_YEAR, DEC_TO_BCD(tm->year));
    ds1302_write(DS_MONTH, DEC_TO_BCD(tm->month));
    ds1302_write(DS_DATE, DEC_TO_BCD(tm->day));
    ds1302_write(DS_HOUR, DEC_TO_BCD(tm->hour));
    ds1302_write(DS_MINUTE, DEC_TO_BCD(tm->minute));
    ds1302_write(DS_SECOND, DEC_TO_BCD(tm->second));
    ds1302_write(DS_DAY, DEC_TO_BCD(tm->day));
    ds1302_write(DS_WP, 0x80);
}

void ds1302_get_time(ds_time_t *tm)
{
    BYTE temp;
    temp = ds1302_read(DS_YEAR);
    tm->year = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_MONTH);
    tm->month = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_DATE);
    tm->day = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_HOUR);
    tm->hour = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_DAY);
    tm->dayofweek = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_MINUTE);
    tm->minute = BCD_TO_DEC(temp);
    temp = ds1302_read(DS_SECOND);
    tm->second = BCD_TO_DEC(temp);
}
