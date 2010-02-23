/***************************************************************************
 *            isr.S
 *
 *  Thu Jan 31 20:01:00 2008
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
 
#define __ASSEMBLY__

#include "sys/system.h"
#include "gdt.h"
#include "basicvid.h"


.section    .text


/* Divide by Zero Exception ***************************************************/
ENTRY(isr0)
    cli
    pushl   $0
    pushl   $0

    /* Push the rest of the regs_t structure onto the stack. */
    PUSH_REGS
    
    /* Switch to the kernel's data segment. */
    movl    $(__KERN_DS), %eax
    movl    %eax, %ds
    movl    %eax, %es
    movl    %eax, %fs
    movl    %eax, %gs
    
    /* Call the handler function. */
    CCALL(divide_error)
    
    /* Restore the registers and the stack. */
    POP_REGS
    addl    $(2*4), %esp
    
    /* Return to whatever we were doing. */
    iret


ENTRY(isr1)
    cli
    pushl   $0
    pushl   $1
    jmp     isr_common_stub

ENTRY(isr2)
    cli
    pushl   $0
    pushl   $2
    jmp     isr_common_stub

ENTRY(isr3)
    cli
    pushl   $0
    pushl   $3
    jmp     isr_common_stub

ENTRY(isr4)
    cli
    pushl   $0
    pushl   $4
    jmp     isr_common_stub

ENTRY(isr5)
    cli
    pushl   $0
    pushl   $5
    jmp     isr_common_stub

ENTRY(isr6)
    cli
    pushl   $0
    pushl   $6
    jmp     isr_common_stub

ENTRY(isr7)
    cli
    pushl   $0
    pushl   $7
    jmp     isr_common_stub


/* Double Fault Exception *****************************************************/
ENTRY(isr8)
    cli
    pushl   $8
    jmp     isr_common_stub

ENTRY(isr9)
    cli
    pushl   $0
    pushl   $9
    jmp     isr_common_stub


/* Bad TSS Exception **********************************************************/
ENTRY(isr10)
    cli
    pushl   $10
    jmp     isr_common_stub


/* Segment Not Present Exception **********************************************/
ENTRY(isr11)
    cli
    pushl   $11
    jmp     isr_common_stub


/* Stack Fault Exception ******************************************************/
ENTRY(isr12)
    cli
    pushl   $12
    jmp     isr_common_stub


/* General Protection Fault Exception *****************************************/
ENTRY(isr13)
    cli
    pushl   $13
    jmp     isr_common_stub


/* Page Fault Exception *******************************************************/
ENTRY(isr14)
    cli
    pushl   $14
    cld
    
    /* Push the rest of the regs_t structure onto the stack. */
    PUSH_REGS
    
    /* Switch to the kernel's data segment. */
    movl    $(__KERN_DS), %eax
    movl    %eax, %ds
    movl    %eax, %es
    movl    %eax, %fs
    movl    %eax, %gs
    
    /* Call the IRQ common handler function. */
    CCALL(page_fault_handler)
    
    /* Restore the registers and the stack. */
    POP_REGS
    addl    $(2*4), %esp
    
    /* Return to whatever we were doing. */
    iret


ENTRY(isr15)
    cli
    pushl   $0
    pushl   $15
    jmp     isr_common_stub


ENTRY(isr16)
    cli
    pushl   $0
    pushl   $16
    jmp     isr_common_stub


ENTRY(isr17)
    cli
    pushl   $0
    pushl   $17
    jmp     isr_common_stub


ENTRY(isr18)
    cli
    pushl   $0
    pushl   $18
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr19)
    cli
    pushl   $0
    pushl   $19
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr20)
    cli
    pushl   $0
    pushl   $20
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr21)
    cli
    pushl   $0
    pushl   $21
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr22)
    cli
    pushl   $0
    pushl   $22
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr23)
    cli
    pushl   $0
    pushl   $23
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr24)
    cli
    pushl   $0
    pushl   $24
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr25)
    cli
    pushl   $0
    pushl   $25
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr26)
    cli
    pushl   $0
    pushl   $26
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr27)
    cli
    pushl   $0
    pushl   $27
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr28)
    cli
    pushl   $0
    pushl   $28
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr29)
    cli
    pushl   $0
    pushl   $29
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr30)
    cli
    pushl   $0
    pushl   $30
    jmp     isr_common_stub

/* Reserved Exception *********************************************************/
ENTRY(isr31)
    cli
    pushl   $0
    pushl   $31
    jmp     isr_common_stub


isr_common_stub:
    cld
    
    /* Push the rest of the regs_t structure onto the stack. */
    PUSH_REGS
    
    /* Switch to the kernel's data segment. */
    movl    $(__KERN_DS), %eax
    movl    %eax, %ds
    movl    %eax, %es
    movl    %eax, %fs
    movl    %eax, %gs
    
    /* Call the IRQ common handler function. */
    /*CCALL(isr_common_handler)*/
    
    /* Print ignored isr message. */
    pushl   $(BLACK)
    pushl   $(RED | BRIGHT)
    CCALL(SetTextColor)
    pushl   $ignored_isr_msg
    CCALL(kprintf)
    pushl   $(BLACK)
    pushl   $(WHITE)
    CCALL(SetTextColor)
    addl    $(5*4), %esp
    
    /* Restore the registers and the stack. */
    POP_REGS
    addl    $(2*4), %esp
    
    /* Return to whatever we were doing. */
    iret


.section    .data
/******************************/
/* Text messages for kprintf. */
/******************************/
ignored_isr_msg:
    .asciz  "Ignored ISR\n"
