/***************************************************************************
 *            handlers.h
 *
 *  Tue 05 Feb 2008 20:34:36 EST
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
 
#ifndef __INTS_H__INCLUDED__
#define __INTS_H__INCLUDED__

#include "sys/system.h"


/* Function declarations */
void __init init_handlers();
bool_t irq_install_handler(uint8_t irq_no, void (*pFunc)(regs_t *r));
void irq_uninstall_handler(uint8_t irq_no);


#endif /*__INTS_H__INCLUDED__*/
