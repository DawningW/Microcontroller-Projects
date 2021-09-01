#ifndef __AM2302_H
#define __AM2302_H

#include "main.h"

bool am2302_init(void);
void am2302_reset(void);
bool am2302_check(void);
uint8_t am2302_read_bit(void);
uint8_t am2302_read_byte(void);
bool am2302_get_data(float *temperature, float *humidity);

#endif /* __AM2302_H */
