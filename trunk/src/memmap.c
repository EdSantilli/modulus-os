/***************************************************************************
 *            memmap.c
 *
 *  Sat Feb 9 2008 18:16:00 EST
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

#include "memmap.h"

#include "basicvid.h"
#include "math.h"
#include "multiboot.h"
#include "page.h"


/* The memory node descriptor. */
typedef struct {
    size_t  base;
    size_t  length;
    size_t  limit;
    size_t  flags;
} node_t;

/* Use these to set the flags in node_t. */
#define NODEFLAG_PRESENT    (1 << 0)
#define NODEFLAG_RESERVED   (1 << 1)
#define NODEFLAG_BIOSAREA   (1 << 2)
#define NODEFLAG_KERNELAREA (1 << 3)
#define NODEFLAG_DMAZONE    (1 << 4)
#define NODEFLAG_NORMALZONE (1 << 5)
#define NODEFLAG_HIGHZONE   (1 << 6)
#define NODEFLAG_CONVMEM    (1 << 7)
#define NODEFLAG_EXTMEM     (1 << 8)

/* We can use CLEAR_ZONEFLAGS on node and zone descriptors. */
#define CLEAR_ZONEFLAGS(_idx) \
do { \
    node[(_idx)].flags &= ~NODEFLAG_DMAZONE; \
    node[(_idx)].flags &= ~NODEFLAG_NORMALZONE; \
    node[(_idx)].flags &= ~NODEFLAG_HIGHZONE; \
} while (0)


/* Create some memory nodes. Whichever nodes are used will be registered into
    the MemNodeList and placed into a proper permenent memory location. The
    unused nodes will be discarded along with the .init.data section. */
#define NUMNODES    16
node_t    node[NUMNODES] __initdata;


/* Use these to set the flags in zone_t. */
#define ZONEFLAG_PRESENT    (1 << 0)
#define ZONEFLAG_DMAZONE    (1 << 4)
#define ZONEFLAG_NORMALZONE (1 << 5)
#define ZONEFLAG_HIGHZONE   (1 << 6)


/* Calculate the size of the frame stack for a _size byte zone. */
#define FRAMESTACKSIZE0(_size)  (4.0 * PAGEALIGN_DOWN((_size)) / PAGE_SIZE)
/* Same, but now consider the space we used up for the stack itself. */
#define _RCONSTANT              ((size_t)(PAGE_SIZE / 4.0 + 1.0))
#define FRAMESTACKSIZE1(_size)  ((size_t)(PAGEALIGN_DOWN((_size)) / _RCONSTANT))


/* Some hints to locate zone boundaries. */
#define MEMHINT_DMAZONE     0           /* DMA memory zone starts at 0MB.     */
#define MEMHINT_NORMALZONE  0x1000000   /* Normal memory zone starts at 16MB. */
#define MEMHINT_HIGHZONE    0x38000000  /* High memory zone starts at 896MB.  */


/* Forward declaration of code in .init.text section. */
void create_memmap(size_t mbiaddr) __init;
void sortnodes() __init;
bool_t insert_node(uint32_t idx) __init;

/* This is where the bootloader's memory info will go. */
multiboot_info_t *mbi __initdata;



void create_memmap(size_t mbiaddr) {

    SetTextColor(WHITE, BLACK);

    /* Set mbi to the address of the multiboot information structure. */
    mbi = (multiboot_info_t *) mbiaddr;


    /* Are mem_* valid? */
    if(!GETBIT(mbi->flags, 0)) {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("The bootloader did not give us all the info we needed! "
                "Try using GRUB.\n"
                "Halting the system.\n");
        SystemHalt();
    }

    /* Was the kernel compiled in a.out format? */
    if(GETBIT(mbi->flags, 4)) {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("The a.out format is not supported. Try compiling as ELF.\n"
                "Halting the system.\n");
        SystemHalt();
    }

    /* Is the section header table of ELF valid? */
    if(!GETBIT(mbi->flags, 5)) {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("The multiboot information has been corrupted!.\n"
                "Halting the system.\n");
        SystemHalt();
    }

    /* Are mmap_* valid? */
    if(GETBIT(mbi->flags, 6)) {
        memory_map_t *mmap;
        uint32_t idx = 0;

        /* Copy the memory_map_t structures to the node structures. */
        for(mmap = (memory_map_t *)mbi->mmap_addr;
            (size_t)mmap < mbi->mmap_addr + mbi->mmap_length;
            mmap = (memory_map_t *)
                        ((size_t)mmap + mmap->size + sizeof(mmap->size))) {

/*            node[idx].base = (size_t)JOIN32(mmap->base_addr_high, mmap->base_addr_low);
            node[idx].length = (size_t)JOIN32(mmap->length_high, mmap->length_low);*/
            node[idx].base = mmap->base_addr_low;
            node[idx].length = mmap->length_low;
            node[idx].limit = node[idx].base + node[idx].length - 1;
            node[idx].flags = NODEFLAG_PRESENT;
            if(mmap->type != 1) node[idx].flags |= NODEFLAG_RESERVED;
            idx++;
        }

        /* Clear the rest of the node structure array. */
        for(; idx < NUMNODES; idx++) {
            node[idx].base = 0;
            node[idx].length = 0;
            node[idx].limit = 0;
            node[idx].flags = 0;
        }

        /* Sort the nodes. Split nodes that are members of multiple zones. */
        sortnodes();

    } else {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("The multiboot loader did not return any information about the "
                "system's memory.\nHalting the system.\n");
        SystemHalt();
    }
}


void create_zones() {
    uint32_t idx, jdx;

    /* Find the DMA zone. */
    for(idx = 0, jdx = NUMNODES; idx < NUMNODES; idx++)
        if((node[idx].flags & NODEFLAG_PRESENT)   &&
           !(node[idx].flags & NODEFLAG_RESERVED) &&
           (node[idx].flags & NODEFLAG_EXTMEM)    &&
           (node[idx].flags & NODEFLAG_DMAZONE)   &&
           (jdx == NUMNODES || node[idx].length > node[jdx].length))
                jdx = idx;
    /* Did we get something useful? */
    if(jdx == NUMNODES) {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("Could not find a node to use as the DMA memory zone.\n"
                "Halting the system.\n");
        SystemHalt();
    } else {
        /* Set the zone descriptor. */
        zone[0].base = PAGEALIGN_UP(node[jdx].base);
        zone[0].limit = PAGEALIGN_DOWN(node[jdx].limit + 1) - 1;
        zone[0].length = zone[0].limit - zone[0].base + 1;
        zone[0].flags = (ZONEFLAG_PRESENT || ZONEFLAG_DMAZONE);
    }

    /* Find the normal zone...if it exists. */
    for(idx = 0, jdx = NUMNODES; idx < NUMNODES; idx++)
        if((node[idx].flags & NODEFLAG_PRESENT)    &&
           !(node[idx].flags & NODEFLAG_RESERVED)  &&
           (node[idx].flags & NODEFLAG_EXTMEM)     &&
           (node[idx].flags & NODEFLAG_NORMALZONE) &&
           (jdx == NUMNODES || node[idx].length > node[jdx].length))
                jdx = idx;
    /* Did we get something useful? If so, set the descriptor. */
    if(jdx == NUMNODES) {
        zone[1].base   = 0;
        zone[1].limit  = 0;
        zone[1].length = 0;
        zone[1].flags  = 0;
    } else {
        /* Set the zone descriptor. */
        zone[1].base = PAGEALIGN_UP(node[jdx].base);
        zone[1].limit = PAGEALIGN_DOWN(node[jdx].limit + 1) - 1;
        zone[1].length = zone[1].limit - zone[1].base + 1;
        zone[1].flags = (ZONEFLAG_PRESENT || ZONEFLAG_NORMALZONE);
    }


    /* Find the high zone...if it exists. */
    for(idx = 0, jdx = NUMNODES; idx < NUMNODES; idx++)
        if((node[idx].flags & NODEFLAG_PRESENT)   &&
           !(node[idx].flags & NODEFLAG_RESERVED) &&
           (node[idx].flags & NODEFLAG_EXTMEM)    &&
           (node[idx].flags & NODEFLAG_HIGHZONE)  &&
           (jdx == NUMNODES || node[idx].length > node[jdx].length))
                jdx = idx;
    /* Did we get something useful? If so, set the descriptor. */
    if(jdx == NUMNODES) {
        zone[2].base   = 0;
        zone[2].limit  = 0;
        zone[2].length = 0;
        zone[2].flags  = 0;
    } else {
        zone[2].base = PAGEALIGN_UP(node[jdx].base);
        zone[2].limit = PAGEALIGN_DOWN(node[jdx].limit + 1) - 1;
        zone[2].length = zone[2].limit - zone[2].base + 1;
        zone[2].flags = (ZONEFLAG_PRESENT || ZONEFLAG_HIGHZONE);
    }

    /* Put the DMA stack after the kernel. */
    if(zone[0].flags > 0) {
        zone[0].framestack_base   = zone[0].base;
        zone[0].framestack_length = FRAMESTACKSIZE1(zone[0].length);
        zone[0].framestack_limit  = zone[0].framestack_base +
                    zone[0].framestack_length - 1;

        zone[0].base = PAGEALIGN_UP(zone[0].framestack_limit + 1);
        zone[0].length = zone[0].limit - zone[0].base + 1;
    } else {
        SetTextColor(RED | BRIGHT, BLACK);
        kprintf("The DMA memory zone could not be found.\n"
                "Halting the system.\n");
        SystemHalt();
    }

    /* Put the normal stack at the beginning of the normal zone. */
    if(zone[1].flags > 0) {
        zone[1].framestack_base   = zone[1].base;
        zone[1].framestack_length = FRAMESTACKSIZE1(zone[1].length);
        zone[1].framestack_limit  = zone[1].framestack_base +
                    zone[1].framestack_length - 1;

        zone[1].base = PAGEALIGN_UP(zone[1].framestack_limit + 1);
        zone[1].length = zone[1].limit - zone[1].base + 1;
    }

    /* Put the high stack at the beginning of the high zone. */
    if(zone[2].flags > 0) {
        zone[2].framestack_base   = zone[2].base;
        zone[2].framestack_length = FRAMESTACKSIZE1(zone[2].length);
        zone[2].framestack_limit  = zone[2].framestack_base +
                    zone[2].framestack_length - 1;

        zone[2].base = PAGEALIGN_UP(zone[2].framestack_limit + 1);
        zone[2].length = zone[2].limit - zone[2].base + 1;
    }

    /* Calculate the amount of available RAM. */
    memory.eff = zone[0].length + zone[1].length + zone[2].length;
    memory.total = memory.eff + zone[0].framestack_length +
                   zone[1].framestack_length + zone[2].framestack_length;
    SetTextColor(WHITE, BLACK);
    kprintf("\nTotal memory = %X\t\tEffective memory = %X\n",
                memory.total, memory.eff);
    DUMPNODES();
    DUMPZONES();
}


void sortnodes() {
    uint32_t idx;

    for(idx = 0; idx < NUMNODES; idx++) {
        /* Check if node is present. */
        if(node[idx].flags == 0) continue;

        /* Check if entire node is in DMA zone. */
        if(node[idx].base >= MEMHINT_DMAZONE &&
                    node[idx].limit < MEMHINT_NORMALZONE) {
            CLEAR_ZONEFLAGS(idx);
            node[idx].flags |= NODEFLAG_DMAZONE;
            continue;
        }

        /* Check if entire node is in normal zone. */
        if(node[idx].base >= MEMHINT_NORMALZONE &&
                    node[idx].limit < MEMHINT_HIGHZONE) {
            CLEAR_ZONEFLAGS(idx);
            node[idx].flags |= NODEFLAG_NORMALZONE;
            continue;
        }

        /* Check if entire node is in high zone. */
        if(node[idx].base >= MEMHINT_HIGHZONE) {
            CLEAR_ZONEFLAGS(idx);
            node[idx].flags |= NODEFLAG_HIGHZONE;
            continue;
        }

        /* If we get here, the node is a member of more than one zone. */

        /* Grab the part of the node in the DMA zone. */
        if(node[idx].base < MEMHINT_NORMALZONE &&
                    node[idx].limit >= MEMHINT_NORMALZONE) {
            /* Shift all nodes after idx down. */
          CLEAR_ZONEFLAGS(idx);
          if(insert_node(idx)) {
                SetTextColor(RED | BRIGHT, BLACK);
                kprintf("Error in sortnodes. "
                        "Needed more than %d nodes.\n\n", NUMNODES);
                DUMPNODES();
                SetTextColor(RED | BRIGHT, BLACK);
                kprintf("\nHalting the system.\n");
                SystemHalt();
            }

            /* Update the new node at idx to end at the zone boundary. */
            node[idx].limit  = MEMHINT_NORMALZONE - 1;
            node[idx].length = node[idx].limit - node[idx].base + 1;
            node[idx].flags |= NODEFLAG_DMAZONE;
            /* Update the old node at idx+1 to start at the zone boundary. */
            idx++;
            node[idx].base   = MEMHINT_NORMALZONE;
            node[idx].length = node[idx].limit - node[idx].base + 1;
            node[idx].flags |= NODEFLAG_NORMALZONE;
            /* Run the old node through our sorting mechanism again. */
            idx--;
            continue;
        }

        /* Grab the part of the node in the normal zone. */
        if(node[idx].base < MEMHINT_HIGHZONE &&
                    node[idx].limit >= MEMHINT_HIGHZONE) {
            /* Shift all nodes after idx down. */
            CLEAR_ZONEFLAGS(idx);
            if(insert_node(idx)) {
                SetTextColor(RED | BRIGHT, BLACK);
                kprintf("Error in sortnodes. "
                        "Needed more than %d nodes.\n\n", NUMNODES);
                DUMPNODES();
                SetTextColor(RED | BRIGHT, BLACK);
                kprintf("\nHalting the system.\n");
                SystemHalt();
            }
            /* Update the new node at idx to end at the zone boundary. */
            node[idx].limit  = MEMHINT_HIGHZONE - 1;
            node[idx].length = node[idx].limit - node[idx].base + 1;
            node[idx].flags |= NODEFLAG_NORMALZONE;
            /* Update the old node at idx+1 to start at the zone boundary. */
            idx++;
            node[idx].base   = MEMHINT_HIGHZONE;
            node[idx].length = node[idx].limit - node[idx].base + 1;
            node[idx].flags |= NODEFLAG_HIGHZONE;
            /* Run the old node through our sorting mechanism again. */
            idx--;
            continue;
        }
    }

    for(idx = 0; node[idx].flags > 0; idx++) {
        /* Identify convetional and extended memory nodes. */
        if(node[idx].base < (size_t)mbi->mem_lower)
            node[idx].flags |= NODEFLAG_CONVMEM;
        else
            node[idx].flags |= NODEFLAG_EXTMEM;

        /* Identify BIOS nodes. */
        if(node[idx].base < 0x100000)
            node[idx].flags |= NODEFLAG_BIOSAREA;

        /* Could this be a kernel node? */
        if(node[idx].base < LD_KERN_END) {
            /* Is this node completely below the range of the kernel? */
            if(node[idx].limit < LD_KERN_START) continue;
            /* Is this node below and in the kernel node? */
            if(node[idx].base < LD_KERN_START && node[idx].limit >= LD_KERN_START) {
                insert_node(idx);
                node[idx].limit = LD_KERN_START - 1;
                node[idx].length = node[idx].limit - node[idx].base + 1;

                idx++;
                node[idx].base = LD_KERN_START;
                node[idx].length = node[idx].limit - node[idx].base + 1;
                node[idx].flags |= NODEFLAG_KERNELAREA;
            }
            /* Is this node in and beyond the kernel node? */
            if(node[idx].base < LD_KERN_END && node[idx].limit >= LD_KERN_END) {
                insert_node(idx);
                node[idx].limit = LD_KERN_END - 1;
                node[idx].length = node[idx].limit - node[idx].base + 1;
                node[idx].flags |= NODEFLAG_KERNELAREA;

                idx++;
                node[idx].base = LD_KERN_END;
                node[idx].length = node[idx].limit - node[idx].base + 1;
                node[idx].flags &= ~NODEFLAG_KERNELAREA;
            }
        }
    }
}


bool_t insert_node(uint32_t idx) {
    /* If all nodes are used, return with an error. */
    if(idx >= NUMNODES - 1) return true;

    /* Shift nodes with an index >= idx down. */
    for(uint32_t jdx = NUMNODES - 1; jdx > idx; jdx--)
        node[jdx] = node[jdx - 1];

    /* Return without error. */
    return false;
}
