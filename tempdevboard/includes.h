#ifndef __INCLUDES_H__
#define __INCLUDES_H__

// TODO 待STC89_Lib支持STC12C5A60S2后迁移过去

#include <STC12C5A60S2.H>
#include <intrins.h>
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

#define u8 uint8_t
#define u16 uint16_t
#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

#define FOSC 22118400L

#endif
