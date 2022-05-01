#ifndef __POWER_H__
#define __POWER_H__

#include "system.h"

typedef enum
{
    RST_Bootarea_AP = 0x0, // 用户代码区
    RST_Bootarea_ISP       // ISP区
} RST_BOOTAREA;

void power_reset(RST_BOOTAREA area);

// TODO 省电和掉电模式
// TODO 看门狗

#endif
