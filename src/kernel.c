/***************************************************************************
 *            kernel.c
 *
 *  Thu 07 Feb 2008 22:07:47 EST
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

#include "sys/system.h"
#include "basicvid.h"
//#include "page.h"
#include "math.h"


void kmain() {
//     /* Print LD info */
//     kprintf("Kernel      start = %X\n", LD_KERN_START);
//     kprintf("Kernel      end   = %X\n", LD_KERN_END);
//     kprintf("Kernel.init start = %X\n", LD_INIT_KERN_START);
//     kprintf("Kernel.init end   = %X\n", LD_INIT_KERN_END);


//     uint32_t na = 0;
//     uint32_t idx = 0;
// 
//     na = request_kern_page();
//     uint32_t *ptr = (uint32_t *)na;
//     *ptr = 0x01234567;
//     kprintf("na = %X\n", ptr);
// 
//     free_kern_page(na);
//     *ptr = 0x89ABCDEF;
//     kprintf("na = %X\n", *ptr);

//     /* Page fault! */
//     uint32_t *ptr = (uint32_t *)0x113000;
//     kprintf("ptr = %X\n", (uint32_t)*ptr);


    /* If we get here, halt the system. */
    SetTextColor(CYAN | BRIGHT, BLACK);
    kprintf("\nSystem OK...halting.");
    SystemHalt();
}

