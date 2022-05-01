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

#define __SET_BIT(x, mask)     (x |= (mask))
#define __CLEAR_BIT(x, mask)   (x &= ~(mask))
#define __REVERSE_BIT(x, mask) (x ^= (mask))
#define __GET_BIT(x, mask)     (x & (mask))
#define __TEST_BIT(x, mask)    (!(!__GET_BIT(x, mask)))
#define BIT_MASK(n)            (1U << ((n) % 8))
#define SET_BIT(x, n)          __SET_BIT(x, BIT_MASK(n))
#define CLEAR_BIT(x, n)        __CLEAR_BIT(x, BIT_MASK(n))
#define REVERSE_BIT(x, n)      __REVERSE_BIT(x, BIT_MASK(n))
#define GET_BIT(x, n)          __GET_BIT(x, BIT_MASK(n))
#define TEST_BIT(x, n)         ((x & BIT_MASK(n)) >> (n))

#define WRITE_REG(reg, mask, n, val) \
    (reg = (reg & ~((mask) << (n))) | ((val) << (n)))
#define WRITE_REG_BIT(reg, n, val) WRITE_REG(reg, 1U, n, val)

// Just make compiler happy :)
#define UNUSED(x) ((void) (x))

// 外部晶振频率 11.0592MHz
#define FOSC 11059200L

#endif
