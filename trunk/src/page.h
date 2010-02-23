/***************************************************************************
 *            page.h
 *
 *  Fri Feb 22 2008  13:06 EST
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

 The page table layout:

    directory[0] -> table_DMA[0][0..PAGE_TBL_ENTRIES-1]    (0 - 4 MB)
    directory[1] -> table_DMA[1][0..PAGE_TBL_ENTRIES-1]    (4 - 8 MB)
    directory[2] -> table_DMA[2][0..PAGE_TBL_ENTRIES-1]    (8 - 12 MB)
    directory[3] -> table_DMA[3][0..PAGE_TBL_ENTRIES-1]    (12 - 16 MB)

    directory[4..PAGE_TBL_ENTRIES-1] -> Normal memory page frames

==============================================================================*/


#ifndef __PAGE_H__INCLUDED__
#define __PAGE_H__INCLUDED__


/***********************/
/* Global declarations */
/***********************/
#include "sys/system.h"
#include "math.h"

/* We should use the page size defined in "sys/system.h". */
#define PAGE_SIZE_BITS          __PAGE_SIZE_BITS__
#define PAGE_SIZE               __PAGE_SIZE__
#define PAGE_TBL_SIZE           PAGE_SIZE
#define PAGE_TBL_ENTRIES        PAGE_TBL_SIZE / 4

/* Define the location of the first free page (at the end of the kernel). */
#define PAGE0_ADDR  LD_INIT_KERN_END
#define PAGE0_IDX   PAGE0_ADDR / PAGE_SIZE

/* Rounding functions for page alignment. */
#define PAGEALIGN_DOWN(addr)    ROUND_DOWN_2N((addr), (PAGE_SIZE_BITS))
#define PAGEALIGN_UP(addr)      ROUND_UP_2N((addr), (PAGE_SIZE_BITS))


/******************/
/* C declarations */
/******************/
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

/* Masks to help maintain the page tables. */
#define PAGE_FLAG_PRESENT   (1 << 0)
#define PAGE_FLAG_READWRITE (1 << 1)
#define PAGE_FLAG_USER      (1 << 2)
#define PAGE_FLAG_ACCESSED  (1 << 5)
#define PAGE_FLAG_DIRTY     (1 << 6)

#endif /*!__ASSEMBLY__ && !__LINKER_SCRIPT__*/


#endif /*__PAGE_H__INCLUDED__*/
