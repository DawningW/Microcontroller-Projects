/**
 * @file soft_pwm.h
 * @author Wu Chen
 * @brief 软件PWM实现
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __SOFT_PWM_H__
#define __SOFT_PWM_H__

#include "system.h"

#if COMPILE_SOFT_PWM == 1

#include "timer.h"

void pwm_init();
void pwm_set_duty(uint8_t duty);
void pwm_cmd(bool enable);

#endif // COMPILE_SOFT_PWM

#endif // __SOFT_PWM_H__
