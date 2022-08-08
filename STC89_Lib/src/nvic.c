#include "nvic.h"

/* IP
 *  7    6    5    4    3    2    1    0
 *  X    X   PT2   PS  PT1  PX1  PT0  PX0
 */
/* IPH
 *  7    6    5    4    3    2    1    0
 * PX3H PX2H PT2H PSH PT1H PX1H PT0H PX0H
 */

#ifdef STC89
#define WRITE_PRIORITY(low, high, priority)         \
    do {                                            \
        low = GET_BIT(priority, 0);                 \
        MODIFY_REG_BIT(IPH, high, GET_BIT(priority, 1)); \
    } while (0)
#else
#define WRITE_PRIORITY(low, high, priority) \
    do {                                    \
        low = priority;                     \
    } while (0)
#endif

void nvic_set_priority(uint8_t channel, NVIC_PRIORITY priority)
{
    switch (channel)
    {
        case 0: WRITE_PRIORITY(PX0, 0, priority); break;
        case 1: WRITE_PRIORITY(PT0, 1, priority); break;
        case 2: WRITE_PRIORITY(PX1, 2, priority); break;
        case 3: WRITE_PRIORITY(PT1, 3, priority); break;
        case 4: WRITE_PRIORITY(PS, 4, priority); break;
#ifdef STC89
        case 5: WRITE_PRIORITY(PT2, 5, priority); break;
        case 6: WRITE_PRIORITY(PX2, 6, priority); break;
        case 7: WRITE_PRIORITY(PX3, 7, priority); break;
#endif
        default: break;
    }
}
