/**
 * @file mem.h
 * @author Wu Chen
 * @brief 扩展RAM配置
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __MEM_H__
#define __MEM_H__

#include "system.h"

#if COMPILE_MEM == 1

/**
 * @brief 是否启用ALE
 *  
 * @param enable 是否启用
 */
void mem_ale(bool enable);
/**
 * @brief 是否启用内部扩展RAM
 * RC: 0000H~00FFH (256 bytes)
 * RD+: 0000H~03FFH (1024 bytes)
 * 
 * @param enable 是否启用
 */
void mem_internal_exram(bool enable);

#endif // COMPILE_MEM

#endif // __MEM_H__
