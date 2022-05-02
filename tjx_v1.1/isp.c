#include "isp.h"

#define CHECK_ADDR(addr)                                  \
    do {                                                  \
        if (addr < ISP_ADDR_START || addr > ISP_ADDR_END) \
            return false;                                 \
    } while (0)
#define SET_ADDR(low, high, addr) \
    do {                          \
        low = addr;               \
        high = addr >> 8;       \
    } while (0)

void mem_ale(bool enable)
{
    WRITE_REG_BIT(AUXR, 0, enable);
}

void mem_internal_exram(bool enable)
{
    WRITE_REG_BIT(AUXR, 1, !enable);
}

void isp_config()
{
    ISP_CONTR = (ISP_CONTR & 0xf8) | ISP_WAIT_TIME;
}

void isp_cmd(bool enable)
{
    WRITE_REG_BIT(ISP_CONTR, 7, enable);
}

static void isp_trigger()
{
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xb9;
    _nop_();
}

void isp_idle()
{
    ISP_CMD = ISP_Command_Idle;
    SET_ADDR(ISP_ADDRL, ISP_ADDRH, 0xffff);
    ISP_DATA = 0xff;
    ISP_TRIG = 0x00;
}

BYTE isp_read_byte(uint16_t addr)
{
    BYTE dat = 0x00;
    ISP_CMD = ISP_Command_Read;
    SET_ADDR(ISP_ADDRL, ISP_ADDRH, addr);
    isp_trigger();
    dat = ISP_DATA;
    return dat;
}

bool isp_write_byte(uint16_t addr, BYTE dat)
{
    CHECK_ADDR(addr);
    ISP_CMD = ISP_Command_Write;
    SET_ADDR(ISP_ADDRL, ISP_ADDRH, addr);
    ISP_DATA = dat;
    isp_trigger();
    return true;
}

bool isp_erase_sector(uint16_t addr)
{
    CHECK_ADDR(addr);
    ISP_CMD = ISP_Command_Erase;
    SET_ADDR(ISP_ADDRL, ISP_ADDRH, addr);
    isp_trigger();
    return true;
}
