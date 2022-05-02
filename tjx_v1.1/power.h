#ifndef __POWER_H__
#define __POWER_H__

#include "system.h"

typedef enum
{
    RST_Bootarea_AP = 0x0, // 用户代码区
    RST_Bootarea_ISP       // ISP区
} RST_BOOTAREA;

typedef enum
{
    POWER_Idle = 0x0, // 空闲模式, 可由任意中断唤醒
    POWER_Down,       // 掉电模式, 仅可由外部中断唤醒
} POWER_MODE;

typedef enum
{
    WDT_Prescale_2 = 0x0,
    WDT_Prescale_4,
    WDT_Prescale_8,
    WDT_Prescale_16,
    WDT_Prescale_32,
    WDT_Prescale_64,
    WDT_Prescale_128,
    WDT_Prescale_256
} WDT_PRESCALE;

void power_reset(RST_BOOTAREA area);
void power_cmd(POWER_MODE mode);
void wdt_cmd(bool enable);
void wdt_set_prescale(WDT_PRESCALE prescale);
void wdt_set_idle_count(bool enable);
void wdt_feed();

#endif
