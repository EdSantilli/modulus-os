/***************************************************************************
 *            math.h
 *
 *  Tue 05 Feb 2008 12:29:12 EST
 *  Copyright  2008  Ed Santilli
 *  EdSantilli@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef __MATH_H__INCLUDED__
#define __MATH_H__INCLUDED__

#include "sys/system.h"


/* Some macros to help manipulate flags. */
#define GETBIT(flags, bit)      ((flags) & (1 << (bit)))
#define BITON(flags, bit)       (flags) |= (1 << (bit))
#define BITOFF(flags, bit)      (flags) &= ~(1 << (bit))
#define GETBITS(val, base, len) (((val) >> (base)) & ((1 << (len)) - 1))

/* Some macros to help with rounding. */
#define LEAST(val1, val2)           ((val1) < (val2)? (val1): (val2))
#define GREATEST(val1, val2)        ((val1) > (val2)? (val1): (val2))
#define ROUND(val)                  ((uint32_t)((val) - .5) + 1UL)
#define ROUND_DOWN(val)             ((uint32_t)(val))
#define ROUND_UP(val)               ((uint32_t)((float)(val) - .1F) + 1UL)
#define ROUND_DOWN_N(val, nearest)  (((val) / (nearest)) * (nearest))
#define ROUND_UP_N(val, nearest)    ((((val) - 1) / (nearest) + 1) * (nearest))
#define ROUND_DOWN_2N(val, bits)    (((val) >> bits) << bits)
#define ROUND_UP_2N(val, bits)      (((((val) - 1) >> bits) + 1) << bits)


/* 64-bit type declarations. */
#ifndef __64_BIT_MATH_NEEDED__
    typedef long long           int64_t;
    typedef unsigned long long  uint64_t;
#else
    typedef struct {
#       ifdef __LITTLE_ENDIAN__
        uint32_t _lbytes;
        uint32_t _hbytes;
#       else /* __LITTLE_ENDIAN__ */
        uint32_t _hbytes;
        uint32_t _lbytes;
#       endif /* __LITTLE_ENDIAN__ */
    } uint64_t;
#endif /* !__64_BIT_MATH_NEEDED__ */


/*! ALERT: THESE SHOULD ONLY BE USED IN MATH.H! */
/* Split a 64 bit unsigned integer into two 32 bit unsigned integers. */
#define _HIGH32(val)            (uint32_t)((val) >> 32)
#define _LOW32(val)             (uint32_t)((val) & 0xFFFFFFFFULL)
/*! END ALERT */

#ifdef __64_BIT_MATH_NEEDED__
/* Use this instead of the ULL suffix. */
#   define VAL64(val) \
        (uint64_t){._hbytes = _HIGH32((val)), ._lbytes = _LOW32((val))}
/* Creates a 64 bit unsigned integer from 2 32-bit unsigned integers. */
#   define JOIN32(high, low) \
        (uint64_t){._hbytes = (high), ._lbytes = (low)}
/* Grabs the high 32-bits of a 64-bit variable. */
#   define HIGH32(var64)    ((var64)._hbytes)
/* Grabs the low 32-bits of a 64-bit variable. */
#   define LOW32(var64)     ((var64)._lbytes)
/* Adds var1 and var2 and puts the result in var2.
    -> var1 can be a VAL64 or uint64_t variable.
    -> var2 must be a uint64_t variable. */
#   define ADD64(var1, var2) \
        __asm__ __volatile__("addl %%ebx, %%edx     \n\t"                   \
                             "adcl %%eax, %%ecx     \n\t"                   \
                             : "+c" ((var2)._hbytes), "+d" ((var2)._lbytes) \
                             : "a" ((var1)._hbytes), "b" ((var1)._lbytes))
/* Subtracts var1 from var2 and puts the result in var2.
    -> var1 can be a VAL64 or uint64_t variable.
    -> var2 must be a uint64_t variable. */
#   define SUB64(var1, var2) \
        __asm__ __volatile__("subl %%ebx, %%edx     \n\t"                   \
                             "sbbl %%eax, %%ecx     \n\t"                   \
                             : "+c" ((var2)._hbytes), "+d" ((var2)._lbytes) \
                             : "a" ((var1)._hbytes), "b" ((var1)._lbytes))
/* Multiplies the 32-bit var32 and least significant 32 bits of var64,
    then places the result in all 64 bits of var64.
    -> var32 can be at most a 32-bit integer.
    -> var64 must be a uint64_t variable. */
#   define MUL64(var32, var64) \
        __asm__ __volatile__("mull %%ebx            \n\t"                   \
                             : "=d" ((var64)._hbytes), "+a" ((var64)._lbytes) \
                             : "b" (var32))
/* Divides the 64-bit var64 by the 32-bit var32, then places the quotient in
    the least significant 32 bits of var64 and the remainder in the most
    significant 32 bits of var64.
    -> var32 can be at most a 32-bit integer.
    -> var64 must be a uint64_t variable. */
#   define DIV64(var32, var64) \
        __asm__ __volatile__("divl %2            \n\t"                   \
                             : "+d" ((var64)._hbytes), "+a" ((var64)._lbytes) \
                             : "r" (var32))
/* The quotient is stored in the low 32 bits of a 64-bit variable. */
#   define QUOTIENT(var64)  LOW32((var64))
/* The remainder is stored in the high 32 bits of a 64-bit variable. */
#   define REMAINDER(var64) HIGH32((var64))

#else  /* __64_BIT_MATH_NEEDED__ */
#   define VAL64(val)           ((uint64_t)(val))
#   define JOIN32(high, low)    \
        ((uint64_t)(((high) << 32) | ((low) & 0xFFFFFFFFULL)))

#   define HIGH32(var64)        _HIGH32((var64))
#   define LOW32(var64)         _LOW32((var64))

#   define ADD64(var1, var2)    (var2) += (var1)
#   define SUB64(var1, var2)    (var2) -= (var1)
#   define MUL64(var1, var2)    (var2) *= (var1)
#   define DIV64(var1, var2)    \
        (var2) = (((var2) / (var1)) & 0xFFFFFFFFULL) | (((var2) % (var1)) << 32)
#   define QUOTIENT(val)        LOW32((val))
#   define REMAINDER(val)       HIGH32((val))
#endif /* __64_BIT_MATH_NEEDED__ */



#endif /*__MATH_H__INCLUDED__*/

