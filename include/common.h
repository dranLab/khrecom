#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long s64;           /* EE GCC: long is 64-bit */
typedef unsigned long u64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));
typedef float f32;
typedef double f64;

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* COMMON_H */
