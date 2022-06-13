#include "soft_i2c.h"

// 延时5us
#define si2c_delay() do { _nop_(); _nop_(); _nop_(); _nop_(); } while(0)

void si2c_init()
{
    SCL = 1;
    SDA = 1;
}

void si2c_start()
{
    si2c_init();
    si2c_delay();
    SDA = 0;
    si2c_delay();
    SCL = 0;
    si2c_delay();
}

void si2c_stop()
{
    SDA = 0;
    si2c_delay();
    SCL = 1;
    si2c_delay();
    SDA = 1;
    si2c_delay();
}

void si2c_send_ack(bit noack)
{
    SDA = noack;
    si2c_delay();
    SCL = 1;
    si2c_delay();
    SCL = 0;
    si2c_delay();
}

bit si2c_test_ack()
{
    SDA = 1;
    si2c_delay();
    SCL = 1;
    si2c_delay();
    F0 = SDA;
    SCL = 0;
    si2c_delay();
    return F0;
}

void si2c_send_byte(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            SDA = 1;
        else
            SDA = 0;
        byte <<= 1;
        si2c_delay();
        SCL = 1;
        si2c_delay();
        SCL = 0;
        si2c_delay();
    }
}

uint8_t si2c_read_byte()
{
    uint8_t byte, i;
    SDA = 1;
    for (i = 0; i < 8; i++)
    {
        SCL = 1;
        si2c_delay();
        byte <<= 1;
        byte |= SDA;
        SCL = 0;
        si2c_delay();
    }
    return byte;
}

void si2c_write_reg(uint8_t address, uint8_t reg_addr, uint8_t* dat, uint8_t len)
{
    uint8_t i;

    si2c_start();
    si2c_send_byte(address | 0x00);
    if (si2c_test_ack()) goto stop;
    si2c_send_byte(reg_addr);
    if (si2c_test_ack()) goto stop;
    for (i = 0; i < len; i++)
    {
        si2c_send_byte(dat[i]);
        if (si2c_test_ack()) goto stop;
    }
stop:
    si2c_stop();
}

void si2c_read_reg(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint8_t len)
{
    uint8_t i;

    si2c_start();
    si2c_send_byte(address | 0x00);
    if (si2c_test_ack()) goto stop;
    si2c_send_byte(reg_addr);
    if (si2c_test_ack()) goto stop;
    si2c_send_byte(address | 0x01);
    if (si2c_test_ack()) goto stop;
    for (i = 0; i < len; i++)
    {
        *buffer = si2c_read_byte();
        buffer++;
        si2c_send_ack(i == len - 1);
    }
stop:
    si2c_stop();
}
