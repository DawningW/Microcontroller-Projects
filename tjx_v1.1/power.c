#include "power.h"

void power_reset(RST_BOOTAREA area)
{
    SET_BIT(ISP_CONTR, 7);
    WRITE_REG_BIT(ISP_CONTR, 6, area);
    SET_BIT(ISP_CONTR, 5);
}
