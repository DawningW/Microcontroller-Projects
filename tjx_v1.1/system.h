#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stddef.h>
#include <intrins.h>
#include "STC89C5xRC.h"

#ifdef __C51__
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

#else
#include <stdint.h>
#include <stdbool.h>

#endif

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t

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

// 外部晶振频率 11.0592MHz
#define FOSC 11059200L

#endif
