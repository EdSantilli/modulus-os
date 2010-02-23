/***************************************************************************
 *            debug.h
 *
 *  Tue Feb 12 2008 17:25:00 EST
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


#ifndef __DEBUG_H__INCLUDED__
#define __DEBUG_H__INCLUDED__


/* Comment out before release. */
#define __DEBUG_ON__


/* Put code that should not be in the release version inside of this macro. */
#ifdef __DEBUG_ON__
#   define DEBUG(x) x
#   if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)
#       include "basicvid.h"
#   endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */
#else /* __DEBUG_ON__ */
#   define DEBUG(x)
#endif /*__DEBUG_ON__ */



#ifdef __ASSEMBLY__

/*  This function pops two long ints, prints them, then halts the system. */
#   define TESTSTACKMAGIC()     \
        pushl   $assert_msg;    \
        CCALL(kprintf);         \
        addl    $(3*4), %esp;   \
        cli;                    \
        hlt

#endif /* __ASSEMBLY__ */



#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

/*  Dumps the registers to the screen. */
#   define DUMP_REGS()     \
        kprintf("regs:\n"); \
        kprintf("\tgs = 0x%X,\tfs = 0x%X,\tes = 0x%X,\tds = 0x%X\n", r->gs, r->fs, r->es, r->ds);  \
        kprintf("\tedi = 0x%X,\tesi = 0x%X,\tebp = 0x%X,\tesp = 0x%X\n", r->edi, r->esi, r->ebp, r->esp);  \
        kprintf("\tebx = 0x%X,\tedx = 0x%X,\tecx = 0x%X,\teax = 0x%X\n", r->ebx, r->edx, r->ecx, r->eax);  \
        kprintf("\tint_no = %u,\terr_code = %u\n", r->int_no, r->err_code);    \
        kprintf("\teip = 0x%X,\tcs = 0x%X\n", r->eip, r->cs);  \
        kprintf("\teflags = 0x%X,\tuseresp = %u,\tss = 0x%X\n", r->eflags, r->useresp, r->ss)

/*  Dumps info about the memory nodes to the screen. */
#   define DUMPNODES()                                                         \
    do {                                                                       \
        uint32_t _idx;                                                         \
        uint32_t _bgclr = BLACK;                                               \
        SetTextColor(WHITE | BRIGHT, BLACK);                                   \
        kprintf("\nNode map dump...\t"                                         \
                "Text: Gray = reserved, White = free\n"                        \
                "\t\t\t\t    Background: Blue = BIOS, Red = Kernel\n");        \
        for(_idx = 0; node[_idx].flags > 0; _idx++) {                          \
            if(node[_idx].flags & NODEFLAG_KERNELAREA)                         \
                _bgclr = RED;                                                  \
            else if(node[_idx].flags & NODEFLAG_BIOSAREA)                      \
                _bgclr = BLUE;                                                 \
            else                                                               \
                _bgclr = BLACK;                                                \
            if(node[_idx].flags & NODEFLAG_RESERVED)                           \
                SetTextColor(BLACK | BRIGHT, _bgclr);                          \
            else                                                               \
                SetTextColor(WHITE, _bgclr);                                   \
            kprintf("-> base = 0x%X", node[_idx].base);                        \
            SetCursorPosX(23);                                                 \
            kprintf("limit = 0x%X", node[_idx].limit);                         \
            SetCursorPosX(43);                                                 \
            kprintf("length = 0x%X", node[_idx].length);                       \
            SetCursorPosX(63);                                                 \
            if(node[_idx].flags & NODEFLAG_DMAZONE) kprintf("DMA zone");       \
            if(node[_idx].flags & NODEFLAG_NORMALZONE) kprintf("Normal zone"); \
            if(node[_idx].flags & NODEFLAG_HIGHZONE) kprintf("High zone");     \
            if(node[_idx].flags & NODEFLAG_CONVMEM) kprintf("|CONV");          \
            if(node[_idx].flags & NODEFLAG_EXTMEM) kprintf("|EXT");            \
            kprintf("\n");                                                     \
        }                                                                      \
    } while(0)

/*  Dumps info about the memory zones to the screen. */
#   define DUMPZONES()                                                         \
    do {                                                                       \
        uint32_t _idx;                                                         \
        SetTextColor(WHITE | BRIGHT, BLACK);                                   \
        kprintf("\nZone descriptor dump...\n");                                \
        kprintf("   base             limit               length\n");           \
        SetTextColor(WHITE, BLACK);                                            \
        for(_idx = 0; zone[_idx].flags > 0; _idx++) {                          \
            kprintf("-> 0x%X", zone[_idx].framestack_base);                    \
            SetCursorPosX(20);                                                 \
            kprintf("0x%X", zone[_idx].framestack_limit);                      \
            SetCursorPosX(40);                                                 \
            kprintf("0x%X\n", zone[_idx].framestack_length);                   \
            kprintf("   0x%X", zone[_idx].base);                               \
            SetCursorPosX(20);                                                 \
            kprintf("0x%X", zone[_idx].limit);                                 \
            SetCursorPosX(40);                                                 \
            kprintf("0x%X", zone[_idx].length);                                \
            SetCursorPosX(60);                                                 \
            if(zone[_idx].flags & ZONEFLAG_DMAZONE) kprintf(" DMA zone");      \
            if(zone[_idx].flags & ZONEFLAG_NORMALZONE) kprintf(" Normal zone");\
            if(zone[_idx].flags & ZONEFLAG_HIGHZONE) kprintf(" High zone");    \
            kprintf("\n");                                                     \
        }                                                                      \
    } while(0)

#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */


#endif /* __DEBUG_H__INCLUDED__ */
