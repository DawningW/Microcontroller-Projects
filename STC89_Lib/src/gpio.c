#include "gpio.h"

#if COMPILE_GPIO == 1

inline uint8_t gpio_read(GPIO_PORT port)
{
    switch (port)
    {
        case GPIO_0: return P0;
        case GPIO_1: return P1;
        case GPIO_2: return P2;
        case GPIO_3: return P3;
#ifdef STC89
        case GPIO_4: return P4;
#endif
        default: return 0x00;
    }
}

inline bool gpio_read_bit(GPIO_PORT port, GPIO_PIN pin)
{
    return TEST_BITS(gpio_read(port), pin);
}

inline void gpio_write(GPIO_PORT port, uint8_t value)
{
    switch (port)
    {
        case GPIO_0: P0 = value; break;
        case GPIO_1: P1 = value; break;
        case GPIO_2: P2 = value; break;
        case GPIO_3: P3 = value; break;
#ifdef STC89
        case GPIO_4: P4 = value; break;
#endif
        default: break;
    }
}

inline void gpio_write_bit(GPIO_PORT port, GPIO_PIN pin, bool value)
{
    if (value)
    {
        gpio_set_bits(port, pin);
    }
    else
    {
        gpio_reset_bits(port, pin);
    }
}

inline void gpio_set_bits(GPIO_PORT port, GPIO_PIN pin)
{
    switch (port)
    {
        case GPIO_0: SET_BITS(P0, pin); break;
        case GPIO_1: SET_BITS(P1, pin); break;
        case GPIO_2: SET_BITS(P2, pin); break;
        case GPIO_3: SET_BITS(P3, pin); break;
#ifdef STC89
        case GPIO_4: SET_BITS(P4, pin); break;
#endif
        default: break;
    }
}

inline void gpio_reset_bits(GPIO_PORT port, GPIO_PIN pin)
{
    switch (port)
    {
        case GPIO_0: CLEAR_BITS(P0, pin); break;
        case GPIO_1: CLEAR_BITS(P1, pin); break;
        case GPIO_2: CLEAR_BITS(P2, pin); break;
        case GPIO_3: CLEAR_BITS(P3, pin); break;
#ifdef STC89
        case GPIO_4: CLEAR_BITS(P4, pin); break;
#endif
        default: break;
    }
}

inline void gpio_toggle_bits(GPIO_PORT port, GPIO_PIN pin)
{
    switch (port)
    {
        case GPIO_0: REVERSE_BITS(P0, pin); break;
        case GPIO_1: REVERSE_BITS(P1, pin); break;
        case GPIO_2: REVERSE_BITS(P2, pin); break;
        case GPIO_3: REVERSE_BITS(P3, pin); break;
#ifdef STC89
        case GPIO_4: REVERSE_BITS(P4, pin); break;
#endif
        default: break;
    }
}

#endif // COMPILE_GPIO
