#include "exti.h"

/** IPH
 *  7    6    5    4    3    2    1    0
 * PX3H PX2H PT2H PSH PT1H PX1H PT0H PX0H
 */
#define WRITE_PRIORITY(low, high, priority)         \
    do {                                            \
        low = GET_BIT(priority, 0);                 \
        MODIFY_REG_BIT(IPH, high, GET_BIT(priority, 1)); \
    } while (0)

void nvic_set_priority(uint8_t channel, NVIC_PRIORITY priority)
{
    switch (channel)
    {
        case 0: WRITE_PRIORITY(PX0, 0, priority); break;
        case 1: WRITE_PRIORITY(PT0, 1, priority); break;
        case 2: WRITE_PRIORITY(PX1, 2, priority); break;
        case 3: WRITE_PRIORITY(PT1, 3, priority); break;
        case 4: WRITE_PRIORITY(PS, 4, priority); break;
        case 5: WRITE_PRIORITY(PT2, 5, priority); break;
        case 6: WRITE_PRIORITY(PX2, 6, priority); break;
        case 7: WRITE_PRIORITY(PX3, 7, priority); break;
        default: break;
    }
}

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
}

void exti_cmd(EXTI_NUM exti, bool enable)
{
    switch (exti)
    {
        case EXTI_0: EX0 = enable; break;
        case EXTI_1: EX1 = enable; break;
        case EXTI_2: EX2 = enable; break;
        case EXTI_3: EX3 = enable; break;
        default: break;
    }
}
