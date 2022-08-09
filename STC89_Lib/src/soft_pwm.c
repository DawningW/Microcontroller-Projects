#include "soft_pwm.h"

#if COMPILE_SOFT_PWM == 1

#define TIMER_COUNT US_TO_COUNT(16, 200)
#define MAX_TICK 100

static uint8_t duty = 0;
static uint8_t tick = 0;

void pwm_init()
{
    TIM_CONFIG tm = {0};

    tick = 0;
#if PWM_TIMER == 0 || PWM_TIMER == 1
    tm.mode = TIM_Mode_1;
#elif defined(STC89) && PWM_TIMER == 2
    tm.mode = TIM_Mode_0;
    tm.period = TIMER_COUNT;
#endif
    tm.value = TIMER_COUNT;
    tm.enable_int = true;
    tm.priority = NVIC_Priority_1;
    timer_init(PWM_TIMER, &tm);
}

void pwm_set_duty(uint8_t value)
{
    duty = value;
}

void pwm_cmd(bool enable)
{
    timer_cmd(PWM_TIMER, enable);
}

#if PWM_TIMER == 0
TIMER0
#elif PWM_TIMER == 1
TIMER1
#elif PWM_TIMER == 2
TIMER2
#endif
{
#if PWM_TIMER == 0 || PWM_TIMER == 1
    timer_set_value(PWM_TIMER, TIMER_COUNT);
#endif
    if (++tick >= MAX_TICK)
        tick = 0;
    PWM_PIN = tick < duty;
}

#endif // COMPILE_SOFT_PWM
