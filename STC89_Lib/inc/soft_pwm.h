/**
 * @file soft_pwm.h
 * @author Chen Wu
 * @brief 软件PWM实现, 默认频率为50Hz, 可在soft_pwm.c中更改
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __SOFT_PWM_H__
#define __SOFT_PWM_H__

#include "system.h"

#if COMPILE_SOFT_PWM == 1

#include "timer.h"

// PWM信号输出
#ifndef PWM_PIN
#error PWM_PIN is not defined!
#endif

// PWM所使用的定时器
#ifndef PWM_TIMER
#error PWM_TIMER is not defined!
#endif

/**
 * @brief PWM初始化
 */
void pwm_init();
/**
 * @brief PWM占空比设置
 * 
 * @param duty 占空比, 范围0~100
 */
void pwm_set_duty(uint8_t duty);
/**
 * @brief PWM开关
 * 
 * @param enable 是否开启PWM输出
 */
void pwm_cmd(bool enable);

#endif // COMPILE_SOFT_PWM

#endif // __SOFT_PWM_H__
