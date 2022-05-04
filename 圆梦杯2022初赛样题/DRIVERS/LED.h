#ifndef	__LED_H
#define	__LED_H

#include "config.h"
#include "GPIO.h"

#define LED_RED P10
#define LED_GREEN P11
#define LED_YELLOW P13

#define RED 0
#define GREEN 1
#define YELLOW 2

void LED_GPIO_config(void);
void LED_Switch(u8 led);

#endif
