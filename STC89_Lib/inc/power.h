/**
 * @file power.h
 * @author Wu Chen
 * @brief 电源管理, 包括复位和省电模式
 * 
 * @copyright Copyright (c) 2020-2022
 */
#ifndef __POWER_H__
#define __POWER_H__

#include "system.h"

#if COMPILE_POWER == 1

/**
 * @brief 复位后的启动区域
 */
typedef enum
{
    RST_Bootarea_AP = 0x0, // 用户代码区
    RST_Bootarea_ISP,      // ISP区
} RST_BOOTAREA;

/**
 * @brief 电源模式
 */
typedef enum
{
    POWER_Idle = 0x0, // 空闲模式, 可由任意中断唤醒
    POWER_Down,       // 掉电模式, 仅可由外部中断唤醒
} POWER_MODE;

/**
 * @brief 软件复位
 * 
 * @param area 复位后的启动区域 
 */
void power_reset(RST_BOOTAREA area);
/**
 * @brief 切换电源模式
 * 
 * @param mode 电源模式
 */
void power_cmd(POWER_MODE mode);

#endif // COMPILE_POWER

#endif // __POWER_H__
