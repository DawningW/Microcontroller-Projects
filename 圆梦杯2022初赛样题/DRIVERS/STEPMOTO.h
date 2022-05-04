#ifndef __STEPMOTO_H
#define __STEPMOTO_H

#include "config.h"
#include "GPIO.h"

#define INA P14
#define INB P15
#define INC P16
#define IND P17

void STEP_MOTO_GPIO_config(void);
void STEP_MOTO_Run(u16 speed);

#endif
