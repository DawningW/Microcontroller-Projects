/**
 * @file system.h
 * @author Wu Chen
 * @brief 8051单片机设备系统
 * 
 * @copyright Copyright (c) 2020-2022
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/*
 * SDCC - Small Device C Compiler
 * http://sdcc.sourceforge.net/
 */
#if defined(SDCC) || defined(__SDCC)
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <sdcc-lib.h>
#include <compiler.h>
#include <lint.h>

#ifdef STC89
#include <stc89.h>
#else
#warning Not defined MCU model, use <8051.h> as default
#include <8051.h>
#endif

/*
 * Keil C51
 * https://www.keil.com/
 */
#elif defined(__C51__)
#include <stddef.h>
// Implement stdint.h
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint16_t;
typedef signed int int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;
// Implement stdbool.h
typedef unsigned char _Bool;
#define bool _Bool
#define false ((bool) 0)
#define true ((bool) 1)
#define __bool_true_false_are_defined 1
// Replace inline with null
#define inline

#include <intrins.h>
#define SBIT(name, addr, bit) sbit name = addr^bit
#define SFR(name, addr) sfr name = addr
#define INTERRUPT(name, vector) void name(void) interrupt vector
#define INTERRUPT_USING(name, vector, num) void name(void) interrupt vector using num

#ifdef STC89
#include <STC89C5xRC.h>
sbit PX3 = XICON^7;
sbit EX3 = XICON^6;
sbit IE3 = XICON^5;
sbit IT3 = XICON^4;
sbit PX2 = XICON^3;
sbit EX2 = XICON^2;
sbit IE2 = XICON^1;
sbit IT2 = XICON^0;
#else
#warning Not defined MCU model, use <reg51.h> as default
#include <reg51.h>
#endif

/*
 * IAR 8051
 * https://www.iar.com/
 */
#elif defined(__ICC8051__)
// TODO 移植到 IAR 8051

#define SBIT(name, addr, bit) __bit __no_init volatile bool name @ (addr+bit)
#define SFR(name, addr) __sfr __no_init volatile unsigned char name @ addr
#define INTERRUPT(name, vector) _Pragma(_PPARAM_(vector)) __interrupt void name(void)
#define INTERRUPT_USING(name, vector, num) _Pragma(_PPARAM2_(num)) _Pragma(_PPARAM_(vector)) __interrupt void name(void)

#ifdef STC89

#else
#warning Not defined MCU model, use <reg51.h> as default
#include <reg51.h>
#endif

/*
 * Unknown compiler
 */
#else
#warning Unknown compiler

#endif

// Not suggest using these macros :(
#define u8 uint8_t
#define u16 uint16_t
#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t

#include "config.h"

#ifndef FOSC
#error Please define oscillator frequency in config.h!
#endif

#ifndef CPI
#error Please define 6T/12T mode in config.h!
#endif

#define SET_BITS(x, mask)     (x |= (mask))
#define CLEAR_BITS(x, mask)   (x &= ~(mask))
#define REVERSE_BITS(x, mask) (x ^= (mask))
#define GET_BITS(x, mask)     (x & (mask))
#define TEST_BITS(x, mask)    (!(!GET_BITS(x, mask)))

#define BIT_MASK(n)           (1U << ((n) % 8))
#define SET_BIT(x, n)         SET_BITS(x, BIT_MASK(n))
#define CLEAR_BIT(x, n)       CLEAR_BITS(x, BIT_MASK(n))
#define REVERSE_BIT(x, n)     REVERSE_BITS(x, BIT_MASK(n))
#define GET_BIT(x, n)         GET_BITS(x, BIT_MASK(n))
#define TEST_BIT(x, n)        ((x & BIT_MASK(n)) >> (n))

#define MODIFY_REG(reg, clear_mask, set_mask) \
    (reg = (reg & (~(clear_mask))) | (set_mask))
#define MODIFY_REG_BIT(reg, n, val) \
    MODIFY_REG(reg, BIT_MASK(n), (val) << (n))

// Just make compiler happy :)
#define UNUSED(x) ((void) (x))

/**
 * @brief 延时指定毫秒后继续执行程序(不精确)
 * 
 * @param ms 延时毫秒数
 */
void delay(uint16_t ms);
/**
 * @brief 延时指定秒后继续执行程序(不精确)
 * 
 * @param s 延时秒数
 */
void sleep(uint16_t s);

#endif // __SYSTEM_H__
