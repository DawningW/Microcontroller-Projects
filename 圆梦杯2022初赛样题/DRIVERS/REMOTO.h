#ifndef	__REMOTO_H
#define	__REMOTO_H

#include "config.h"
#include "GPIO.h"

#define	PWM_REMOTO P06

extern u16 ServoHighPulse;

void REMOTO_GPIO_config(void);
void REMOTO_Start(void);
void REMOTO_Stop(void);

#endif
