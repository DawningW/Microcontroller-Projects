#include "power.h"

void power_reset(RST_BOOTAREA area)
{
    WRITE_REG_BIT(ISP_CONTR, 6, area);
    SET_BIT(ISP_CONTR, 5);
}

void power_cmd(POWER_MODE mode)
{
    SET_BIT(PCON, mode);
}

void wdt_cmd(bool enable)
{
    WRITE_REG_BIT(WDT_CONTR, 5, enable);
}

void wdt_set_prescale(WDT_PRESCALE prescale)
{
    WDT_CONTR = (WDT_CONTR & 0xf8) | prescale;
}

void wdt_set_idle_count(bool enable)
{
    WRITE_REG_BIT(WDT_CONTR, 3, enable);
}

void wdt_feed()
{
    SET_BIT(WDT_CONTR, 4);
}
