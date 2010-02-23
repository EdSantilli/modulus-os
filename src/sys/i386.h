/***************************************************************************
 *            i386.h
 *
 *  Sun Feb 25 18:23:10 2007
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
 
#ifndef __i386_H__INCLUDED__
#define __i386_H__INCLUDED__


/*************************/
/* Assembly declarations */
/*************************/
#ifdef __ASSEMBLY__

/* CMANGLE mangles C function names. */
#define CMANGLE(sym) sym

/* CCALL calls C functions. */
#define CCALL(func)         \
    .extern CMANGLE(func);  \
    call    CMANGLE(func);

/* ENTRY aligns global symbols. */
#define ENTRY(name) \
    .globl name;    \
    .align 4, 0;    \
    name:

/* This macro pushes most of the regs_t structure onto the stack.
    The regs_t type is defined in system.h. */
#define PUSH_REGS       \
    pushal;             \
    pushl   %ds;        \
    pushl   %es;        \
    pushl   %fs;        \
    pushl   %gs;        \
    movl    %esp, %eax; \
    pushl   %eax

/* This macro cleans up the registers and the stack after using PUSH_REGS.
    The regs_t type is defined in system.h. */
#define POP_REGS    \
    popl    %eax;   \
	popl    %gs;    \
	popl    %fs;    \
	popl    %es;    \
	popl    %ds;    \
    popal

#endif /* __ASSEMBLY__ */


/******************/
/* C declarations */
/******************/
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

#define EnableInts()    __asm__ __volatile__ ("sti")
#define DisableInts()   __asm__ __volatile__ ("cli")

#define SystemHalt()                    \
    do {__asm__ __volatile__ ("cli  \n\t" "hlt");} while(1==1)


#define in8(port, data)     \
    __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (data) : "d" (port))
#define out8(port, data)    \
    __asm__ __volatile__ ("outb %%al, %%dx" : : "a" (data), "d" (port))
#define in16(port, data)    \
    __asm__ __volatile__ ("inw %%dx, %%ax" : "=a" (data) : "d" (port))
#define out16(port, data)   \
    __asm__ __volatile__ ("outw %%ax, %%dx" : : "a" (data), "d" (port))


#define MemCopy8(src, dest, count)              \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep movsb    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "S" (src), "D" (dest), "c" (count))
#define MemCopy16(src, dest, count)             \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep movsw    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "S" (src), "D" (dest), "c" (count))
#define MemCopy32(src, dest, count)             \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep movsl    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "S" (src), "D" (dest), "c" (count))

#define MemSet8(value, dest, count)             \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep stosb    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "a" (value), "D" (dest), "c" (count))
#define MemSet16(value, dest, count)            \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep stosw    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "a" (value), "D" (dest), "c" (count))
#define MemSet32(value, dest, count)            \
    __asm__ __volatile__ ("pushal       \n\t"   \
                          "pushfl       \n\t"   \
                          "cld          \n\t"   \
                          "rep stosl    \n\t"   \
                          "popfl        \n\t"   \
                          "popal"               \
                          : : "a" (value), "D" (dest), "c" (count))


#define GetCR0(variable)                    \
    __asm__ __volatile__ ("movl %%cr0, %0"  \
                          : "=r" (variable))

#define SetCR0(variable)                    \
    __asm__ __volatile__ ("movl %0, %%cr0"  \
                          : : "r" (variable))

#define GetCR2(variable)                    \
    __asm__ __volatile__ ("movl %%cr2, %0"  \
                          : "=r" (variable))

#define GetCR3(variable)                    \
    __asm__ __volatile__ ("movl %%cr3, %0"  \
                          : "=r" (variable))

#define SetCR3(variable)                    \
    __asm__ __volatile__ ("movl %0, %%cr3"  \
                          : : "r" (variable))


#define SetLIDT(pIDT, IDTCount)                     \
    __asm__ __volatile__("pushfl            \n\t"   \
                         "cli               \n\t"   \
                         "pushl %0          \n\t"   \
                         "pushw %%ax        \n\t"   \
                         "lidt  (%%esp)     \n\t"   \
                         "addl  $6, %%esp   \n\t"   \
                         "popfl"                    \
                         : : "r" (pIDT), "a" ((IDTCount) * 8))

#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */


#endif /* __i386_H__INCLUDED__ */
