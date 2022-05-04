#ifndef	__DCMOTO_H
#define	__DCMOTO_H

#include "config.h"
#include "GPIO.h"

#define IN1 P30
#define	IN2 P31
#define	PWM_DC P06

void DCMOTO_GPIO_config(void);
void Time0_16_Config_Init(void);
void DCMoto_Sta(u8 sta, u8 dir);

#endif
