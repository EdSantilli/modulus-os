/***************************************************************************
 *            types.h
 *
 *  Thu Mar  1 20:17:20 2007
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
 
#ifndef __TYPES_H__INCLUDED__
#define __TYPES_H__INCLUDED__


/* Declare integer types - See math.h for details on using 64-bit integers. */
typedef char                int8_t;
typedef short               int16_t;
typedef long                int32_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;



/* Declare the boolean type. */
#if !defined(true) && !defined(false)
#   ifdef bool_t
#       undef bool_t
#   endif /* bool_t */
    typedef enum {
        false = (0 == 1),
        true = (1 == 1)
    } bool_t;
#endif /* !defined(true) && !defined(false) */


/* The NULL pointer. */
#ifndef NULL
#   define NULL ((void *)0)
#endif /*NULL*/


#endif /* !__TYPES_H__INCLUDED__ */
