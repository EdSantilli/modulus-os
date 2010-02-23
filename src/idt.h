/* FILE: idt.h
 * BORN: 08-02-2007
 * DESC: Declares the IDT and the functions that maintain it. */

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

#ifndef __IDT_H__INCLUDED__
#define __IDT_H__INCLUDED__


/***********************/
/* Global declarations */
/***********************/
/* Set number of descriptors. */
#define IDT_COUNT       256


/******************/
/* C declarations */
/******************/
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

#include "sys/system.h"

/* Function declarations */
void init_idt() __init;

void set_user_int_gate(uint8_t index, void (*pFunc)(void));
void set_user_trap_gate(uint8_t index, void (*pFunc)(void));
void set_kern_int_gate(uint8_t index, void (*pFunc)(void));
void set_kern_trap_gate(uint8_t index, void (*pFunc)(void));

#endif /*!__ASSEMBLY__ && !__LINKER_SCRIPT__*/

                         
#endif /*__IDT_H__INCLUDED__*/
