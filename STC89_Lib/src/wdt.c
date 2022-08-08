#include "wdt.h"

#if COMPILE_WDT == 1

void wdt_cmd(bool enable)
{
    MODIFY_REG_BIT(WDT_CONTR, 5, enable);
}

void wdt_set_prescale(WDT_PRESCALE prescale)
{
    WDT_CONTR = (WDT_CONTR & 0xf8) | prescale;
}

void wdt_set_idle_count(bool enable)
{
    MODIFY_REG_BIT(WDT_CONTR, 3, enable);
}

void wdt_feed()
{
    SET_BIT(WDT_CONTR, 4);
}

#endif // COMPILE_WDT
