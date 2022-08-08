#include "power.h"

#if COMPILE_POWER == 1

void power_reset(RST_BOOTAREA area)
{
    MODIFY_REG_BIT(ISP_CONTR, 6, area);
    SET_BIT(ISP_CONTR, 5);
}

void power_cmd(POWER_MODE mode)
{
    SET_BIT(PCON, mode);
}

#endif // COMPILE_POWER
