#include "mem.h"

#if COMPILE_MEM == 1

void mem_ale(bool enable)
{
    MODIFY_REG_BIT(AUXR, 0, enable);
}

void mem_internal_exram(bool enable)
{
    MODIFY_REG_BIT(AUXR, 1, !enable);
}

#endif // COMPILE_MEM
