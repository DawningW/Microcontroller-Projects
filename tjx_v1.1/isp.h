#ifndef __ISP_H__
#define __ISP_H__

#include "system.h"

#define ISP_ADDR_START 0x2000
#define ISP_ADDR_END 0x2FFF
#if   (FOSC <=  1000000UL)
    #define ISP_WAIT_TIME 0x07
#elif (FOSC <=  2000000UL)
    #define ISP_WAIT_TIME 0x06
#elif (FOSC <=  3000000UL)
    #define ISP_WAIT_TIME 0x05
#elif (FOSC <=  6000000UL)
    #define ISP_WAIT_TIME 0x04
#elif (FOSC <= 12000000UL)
    #define ISP_WAIT_TIME 0x03
#elif (FOSC <= 20000000UL)
    #define ISP_WAIT_TIME 0x02
#elif (FOSC <= 24000000UL)
    #define ISP_WAIT_TIME 0x01
#else 
    #define ISP_WAIT_TIME 0x00
#endif

typedef enum
{
    ISP_Command_Idle = 0x0,
    ISP_Command_Read,
    ISP_Command_Write,
    ISP_Command_Erase
} ISP_COMMAND;

void mem_ale(bool enable);
// RC: 0000H~00FFH (256 bytes)
// RD+: 0000H~03FFH (1024 bytes)
void mem_internal_exram(bool enable);
void isp_config();
void isp_cmd(bool enable);
void isp_idle();
BYTE isp_read_byte(uint16_t addr);
bool isp_write_byte(uint16_t addr, BYTE dat);
bool isp_erase_sector(uint16_t addr);

#endif
