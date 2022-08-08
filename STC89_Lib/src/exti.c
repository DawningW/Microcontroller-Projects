#include "exti.h"

#if COMPILE_EXTI == 1

void exti_init(EXTI_NUM num, EXTI_CONFIG *exti)
{
    if (num == EXTI_0)
    {
        nvic_set_priority(0, exti->priority);
        IT0 = exti->trigger;
    }
    else if (num == EXTI_1)
    {
        nvic_set_priority(2, exti->priority);
        IT1 = exti->trigger;
    }
#ifdef STC89
    else if (num == EXTI_2)
    {
        nvic_set_priority(6, exti->priority);
        IT2 = exti->trigger;
    }
    else if (num == EXTI_3)
    {
        nvic_set_priority(7, exti->priority);
        IT3 = exti->trigger;
    }
#endif
}

void exti_cmd(EXTI_NUM exti, bool enable)
{
    switch (exti)
    {
        case EXTI_0: EX0 = enable; break;
        case EXTI_1: EX1 = enable; break;
#ifdef STC89
        case EXTI_2: EX2 = enable; break;
        case EXTI_3: EX3 = enable; break;
#endif
        default: break;
    }
}

#endif // COMPILE_EXTI
