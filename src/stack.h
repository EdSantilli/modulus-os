/***************************************************************************
 *            stack.h
 *
 *  Sat Apr 14 2007 16:52:38 EST
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
 
#ifndef __STACK_H__INCLUDED__
#define __STACK_H__INCLUDED__

#include "sys/system.h"


/* To find the stack size, use: sizeof(_objtype_) * numobjs + STACK_OVERHEAD. */
#define STACK_OVERHEAD          4

/* Returns the mangled stack name. */
#define STACK(_name_) _Namespace_STACK__##_name_


/* Defines a static stack symbol and some miscellaneous stack info. */
#define CREATE_STATIC_STACK_SYMBOL(_name_, _objtype_, _numobjs_) \
    typedef _objtype_  STACK(_name_##_t); \
    static  _objtype_  STACK(_name_)[_numobjs_]; \
    static  uint32_t   STACK(_name_##_Idx) = 0

/* Defines a dynamic stack symbol and some miscellaneous stack info. */
#define CREATE_DYNAMIC_STACK_SYMBOL(_name_, _objtype_) \
    typedef _objtype_  STACK(_name_##_t); \
    static  _objtype_  *STACK(_name_) = (_objtype_ *)0; \
    static  uint32_t   STACK(_name_##_Idx) = 0

/* Points a dynamic stack symbol to an actual stack. */
#define INIT_DYNAMIC_STACK(_name_, _vaddr_) \
    STACK(_name_) = (STACK(_name_##_t) *)(_vaddr_)

/* Retrieves the current stack pointer. */
#define STACK_IDX(_name_)     (STACK(_name_##_Idx))

/* Pushes and pops data onto and off of the stack. */
#define PUSH(_obj_, _stack_)  (STACK(_stack_)[STACK(_stack_##_Idx)++] = (_obj_))
#define POP(_stack_, _obj_)   ((_obj_) = STACK(_stack_)[--STACK(_stack_##_Idx)])


#endif /*__STACK_H__INCLUDED__*/
