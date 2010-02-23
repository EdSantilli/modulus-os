/***************************************************************************
 *            memmap.h
 *
 *  Sat Feb 9 2008 18:17:00 EST
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

/*==============================================================================

 Physical memory layout (in MBytes):

   <-------Identity-mapped---------->
   0      1         2                16
   ***************************************************************************
   *<--------zone[0] (DMA)---------->*<-----------zone[1] (Normal)---------->*
   *      *         * *              * *                                     *
   * BIOS *  Kernel * *    DMA       * *             Normal                  *
   * data *   code  * *   memory     * *             memory                  *
   *      *         * *              * *                                     *
   ***************************************************************************
                     ^                ^
        DMA zone frame stack     Normal zone frame stack
        stored after kernel      stored at start of zone

==============================================================================*/

#ifndef __MEMMAP_H__INCLUDED__
#define __MEMMAP_H__INCLUDED__

#include "sys/system.h"



/* The memory zone descriptor. */
struct {
    uint32_t    flags;
    size_t      base;
    size_t      limit;
    size_t      length;
    size_t      framestack_base;
    size_t      framestack_limit;
    size_t      framestack_length;
    uint32_t    numframes;
    uint32_t    freeframes;
} zone[3];


/* Holds the amount of RAM on the system for quick estimates. */
struct {
    size_t      total;
    size_t      eff;
} memory;



#endif /*__MEMMAP_H__INCLUDED__*/
