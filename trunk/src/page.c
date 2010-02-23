/***************************************************************************
 *            page.c
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


#include "page.h"

#include "memmap.h"
#include "stack.h"
#include "basicvid.h"


/* Define the page directory and page table types. */
typedef volatile struct {uint32_t _data[PAGE_TBL_ENTRIES];} PageDir_t;
typedef volatile struct {uint32_t _data[PAGE_TBL_ENTRIES];} PageTbl_t;

/* Returns the mangled page directory and table names. */
#define PD(name)            (_Namespace_PD__##name)
#define PT(name)            (_Namespace_PT__##name)

/* Returns a pointer to the page directory and tables. */
#define pPD(name)           ((uint32_t)PD(name))
#define pPT(name)           ((uint32_t)PT(name))

/* Converts a virtual address into page directory and table indicies. */
#define PDidx(vaddr)        (GETBITS((vaddr), 22, 10))
#define PTidx(vaddr)        (GETBITS((vaddr), 12, 10))

/* Macros that access page directory and table entries. */
#define PDE(name, vaddr)    (PD(name)->_data[PDidx(vaddr)])
#define PTE(name, vaddr)    (PT(name)->_data[PTidx(vaddr)])


/* The kernel's page directory and tables. */
extern uint32_t _PageDirectory;
extern uint32_t _PageTable0;
extern uint32_t _PageTable1;
extern uint32_t _PageTable2;
extern uint32_t _PageTable3;

static PageDir_t* PD(kern) = (PageDir_t*)&_PageDirectory;
static PageTbl_t* PT(kern[4]) = {
    (PageTbl_t*)&_PageTable0,
    (PageTbl_t*)&_PageTable1,
    (PageTbl_t*)&_PageTable2,
    (PageTbl_t*)&_PageTable3
};


/* The page frame stacks. */
CREATE_DYNAMIC_STACK_SYMBOL(DMA_frames, size_t);
CREATE_DYNAMIC_STACK_SYMBOL(Normal_frames, size_t);
CREATE_DYNAMIC_STACK_SYMBOL(High_frames, size_t);


void __init init_paging() {
    /* Initialize the frame stacks. */
    INIT_DYNAMIC_STACK(DMA_frames, zone[0].framestack_base);
    INIT_DYNAMIC_STACK(Normal_frames, zone[1].framestack_base);
    INIT_DYNAMIC_STACK(High_frames, zone[2].framestack_base);
    
    /* Identity-map the DMA zone. */
    size_t addr = 0;
    uint32_t idx = (uint32_t)(-1);
    for(; addr < PAGEALIGN_UP(zone[0].framestack_limit); addr += PAGE_SIZE) {
        if(idx != PDidx(addr)) {
            idx = PDidx(addr);
            PD(kern)->_data[idx] = pPT(kern[idx])
                                 | PAGE_FLAG_PRESENT
                                 | PAGE_FLAG_READWRITE;
        }
        PTE(kern[idx], addr) = addr | PAGE_FLAG_PRESENT | PAGE_FLAG_READWRITE;
    }
    for(; addr < zone[0].limit; addr += PAGE_SIZE) {
        if(idx != PDidx(addr)) {
            idx = PDidx(addr);
            PD(kern)->_data[idx] = pPT(kern[idx]);
        }
        PTE(kern[idx], addr) = addr;
    }
/*    for(; addr < zone[0].limit; addr += PAGE_SIZE) {*/
/*        if(idx != PDidx(addr)) {*/
/*            idx = PDidx(addr);*/
/*            PD(kern)->_data[idx] = pPT(kern[idx]) | PAGE_FLAG_PRESENT | PAGE_FLAG_READWRITE;*/
/*        }*/
/*        PTE(kern[idx], addr) = addr | PAGE_FLAG_PRESENT | PAGE_FLAG_READWRITE;*/
/*    }*/
    
    /* Install the kernel's page directory. */
    SetCR3(pPD(kern));
    
    /* Start paging. */
    uint32_t cr0;
    GetCR0(cr0);
    SetCR0(cr0 | 0x80000000);
}


void page_fault_handler(regs_t* r) {
    DisableInts();
    
    static uint32_t faulty_vaddr;
    GetCR2(faulty_vaddr);
    
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: Page fault accessing 0x%X.\n", faulty_vaddr);
    
    SystemHalt();
}

