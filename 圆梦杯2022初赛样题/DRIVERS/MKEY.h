#ifndef	__MKEY_H
#define	__MKEY_H

#include "config.h"
#include "GPIO.h"
#include "MKEY.h"

#define R1 P30
#define R2 P31
#define R3 P32
#define R4 P33
#define C1 P34
#define C2 P35
#define C3 P36
#define C4 P37

void MKEY_GPIO_config(void);
char Mkey_Read(void);

#endif
