/***************************************************************************
 *            irq.S
 *
 *  Thu Apr  5 19:31:59 2007
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
 
#define __ASSEMBLY__

#include "sys/system.h"
#include "gdt.h"


.section    .text

ENTRY(irq0)
    cli
    pushl   $0
    pushl   $32
    jmp     irq_common_stub

ENTRY(irq1)
    cli
    pushl   $0
    pushl   $33
    jmp     irq_common_stub

ENTRY(irq2)
    cli
    pushl   $0
    pushl   $34
    jmp     irq_common_stub

ENTRY(irq3)
    cli
    pushl   $0
    pushl   $35
    jmp     irq_common_stub

ENTRY(irq4)
    cli
    pushl   $0
    pushl   $36
    jmp     irq_common_stub

ENTRY(irq5)
    cli
    pushl   $0
    pushl   $37
    jmp     irq_common_stub

ENTRY(irq6)
    cli
    pushl   $0
    pushl   $38
    jmp     irq_common_stub

ENTRY(irq7)
    cli
    pushl   $0
    pushl   $39
    jmp     irq_common_stub

ENTRY(irq8)
    cli
    pushl   $0
    pushl   $40
    jmp     irq_common_stub

ENTRY(irq9)
    cli
    pushl   $0
    pushl   $41
    jmp     irq_common_stub

ENTRY(irq10)
    cli
    pushl   $0
    pushl   $42
    jmp     irq_common_stub

ENTRY(irq11)
    cli
    pushl   $0
    pushl   $43
    jmp     irq_common_stub

ENTRY(irq12)
    cli
    pushl   $0
    pushl   $44
    jmp     irq_common_stub

ENTRY(irq13)
    cli
    pushl   $0
    pushl   $45
    jmp     irq_common_stub

ENTRY(irq14)
    cli
    pushl   $0
    pushl   $46
    jmp     irq_common_stub

ENTRY(irq15)
    cli
    pushl   $0
    pushl   $47
    jmp     irq_common_stub


irq_common_stub:
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
    CCALL(irq_common_handler)
    
    /* Restore the registers and the stack. */
    POP_REGS
    addl    $(2*4), %esp
	
    /* Return to whatever we were doing. */
    iret
