#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "system.h"

#define SDA P01
#define SCL P00

void si2c_write_reg(uint8_t address, uint8_t reg_addr, uint8_t* dat, uint8_t len);
void si2c_read_reg(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint8_t len);

#endif
