/***************************************************************************
 *            bitmap.h
 *
 *  Sun Apr 29 2007 15:19:33 EST
 *  Copyright  2007  Ed Santilli
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
 
#ifndef __BITMAP_H__INCLUDED__
#define __BITMAP_H__INCLUDED__

#include "sys/system.h"


/* Returns the mangled bitmap name. */
#define BITMAP(_name_) _Namespace_BITMAP__##_name_


/* Gets the size of the bitmap. */
#define BITMAP_SIZE_IN_BITS(_name_)     (BITMAP(_name_##_Bits))
#define BITMAP_SIZE_IN_DWORDS(_name_)   (BITMAP(_name_##_Count))


/* Declares the bitmap and some miscellaneous info. */
#define CREATE_STATIC_BITMAP(_name_, _bits_) \
    static const uint32_t BITMAP_SIZE_IN_BITS(_name_) = (_bits_); \
    static const uint32_t BITMAP_SIZE_IN_DWORDS(_name_)=ROUND_UP((_bits_)/32); \
    static uint32_t BITMAP(_name_)[ROUND_UP((_bits_) / 32)]

/* Zeros out a bitmap. */
#define OPEN_BITMAP(_name_) \
do { \
    static uint32_t _bidx; \
    for(_bidx = 0; _bidx < BITMAP_SIZE_IN_DWORDS(_name_); _bidx++) \
        BITMAP(_name_)[_bidx] = 0; \
} while(0)


/* Gets/sets/clears a bit. */
#define GET_BIT(_name_, _bit_) \
    ((BITMAP(_name_)[ROUND_DOWN((_bit_) / 32)] >> ((_bit_) % 32)) & 1)
#define OPEN_BIT(_name_, _bit_) \
    (BITMAP(_name_)[ROUND_DOWN((_bit_) / 32)] &= ~(1UL << ((_bit_) % 32)))
#define CLOSE_BIT(_name_, _bit_) \
    (BITMAP(_name_)[ROUND_DOWN((_bit_) / 32)] |= (1UL << ((_bit_) % 32)))
#define TOGGLE_BIT(_name_, _bit_) \
    (BITMAP(_name_)[ROUND_DOWN((_bit_) / 32)] ^= (1UL << ((_bit_) % 32)))

/* Search for the first bit set to 0. */
#define FIND_FIRST_OPEN_BIT(_name_, _bit_ret_var_) \
do { \
    _bit_ret_var_ = 0; \
    while(GET_BIT(_name_, (_bit_ret_var_))) (_bit_ret_var_)++; \
} while(0)


#ifdef __DEBUG_ON__
#   include "basicvid.h"
#   define dump_bitmap(_name_) \
    do { \
        static uint32_t _curbit; \
        for(_curbit = 0; _curbit < BITMAP_SIZE_IN_BITS(_name_); _curbit++) { \
            if(_curbit % 8 == 0)  kprintf(" "); \
            if(_curbit % 32 == 0) kprintf("\n"); \
            kprintf("%u", GET_BIT(_name_, _curbit)); \
        } \
    } while(0)
#else /*__DEBUG_ON__*/
#   define dump_bitmap(_name_)
#endif /*__DEBUG_ON__*/


#endif /*__BITMAP_H__INCLUDED__*/
