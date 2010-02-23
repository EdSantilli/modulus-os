/***************************************************************************
 *            boot.S
 *
 *  Tue 05 Feb 2008 13:36:54 EST
 *  Copyright  20078  Ed Santilli
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
#include "multiboot.h"
#include "basicvid.h"
#include "gdt.h"

/* We should use the stack size defined in "sys/system.h". */
#define STACK_SIZE  __STACK_SIZE__



.section    .text

/* The kernel's entry point */
ENTRY(start)
    jmp	kernel_init



/* >> The multiboot header. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	.align	4
multiboot_header:
	.long	MULTIBOOT_HEADER_MAGIC                              /* magic    */
	.long	MULTIBOOT_HEADER_FLAGS                              /* flags    */
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)  /* checksum */
#   ifndef __ELF__
	.long	multiboot_header    /* header_addr   */
	.long	_start              /* load_addr     */
	.long	_edata              /* load_end_addr */
	.long	_end                /* bss_end_addr  */
	.long	multiboot_entry     /* entry_addr    */
#   endif /*!__ELF__*/
/* << End of the multiboot header. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



.section    .init.text

/* >> The basic initialization routines. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
kernel_init:
    /* Initialize the stack pointer. */
    movl	$(stack + STACK_SIZE), %esp
	
    /* Reset EFLAGS. */
    pushl	$0
    popfl

    /* Push magic values to beggining of stack. */
    DEBUG(pushl   $0xDECAFBAD)
    DEBUG(pushl   $0xDEADBEEF)

    /* Push the multiboot info structure and magic onto the stack. */
    pushl   %ebx
    pushl   %eax

    /* Setup basic text services. */
    CCALL(InitializeVideo)

    /* Print opening message. */
    pushl   $(BLACK)
    pushl   $(CYAN | BRIGHT)
    CCALL(SetTextColor)
    pushl   $banner_msg0
    CCALL(kprintf)
    pushl   $banner_msg1
    CCALL(kprintf)
    pushl   $banner_msg2
    CCALL(kprintf)

    pushl   $(BLACK)
    pushl   $(WHITE)
    CCALL(SetTextColor)
    pushl   $compile_info2
    pushl   $compile_info1
    pushl   $compile_info0
    CCALL(kprintf)

    addl    $(10*4), %esp

    /* Test for multiboot magic number. */
    popl    %eax
    cmpl    $(MULTIBOOT_BOOTLOADER_MAGIC), %eax
    jne     no_magic

    /* Load the GDT and reset the segment registers. */
    lea     GDT_descriptor, %edi
    lgdt    (%edi)
    movl    $(__KERN_DS), %eax
    movl    %eax, %ds
    movl    %eax, %es
    movl    %eax, %fs
    movl    %eax, %gs
    movl    %eax, %ss
    
    /* Clear prefetch and normalize %eip. */
    ljmp    $(__KERN_CS), $set_cs_long_jump0
set_cs_long_jump0:

    /* Initialize the interrupts. */
    CCALL(init_idt)         /* Initialize the IDT. */
    CCALL(init_handlers)    /* Initialize the basic error handlers. */
    CCALL(init_timer)       /* Initialize the timer IRQ. */
    sti                     /* Enable the interrupts. */

    /* Create the memory map. */
    CCALL(create_memmap)
    CCALL(create_zones)

    /* Enable paging. The address of the multiboot info is on the stack. */
    CCALL(init_paging)
    
    /* Clear prefetch and normalize %eip. */
    ljmp    $(__KERN_CS), $set_cs_long_jump1
set_cs_long_jump1:

    /* Call the kernel proper. */
    CCALL(kmain)
    cli
    hlt
/* << End of the basic initialization routines. <<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >> Very basic error handlers. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* This is the default interrupt "handler." */
ENTRY(ignore_int)
    cld
    
    pushal
	pushl   %ds
	pushl   %es
    pushl   %fs
    pushl   %gs
    
	movl    $(__KERN_DS), %eax
	movl    %eax, %ds
	movl    %eax, %es
	movl    %eax, %fs
	movl    %eax, %gs
    
    pushl   $int_msg
	CCALL(kprintf)
    addl    $(1*4), %esp
        
	popl    %gs
	popl    %fs
	popl    %es
	popl    %ds
    popal
    
	iret

no_magic:
    pushl   $magic_error_msg
    jmp     halt_system

halt_system:
    /* Print the error message and halt the system. */
    pushl   $(BLACK)
    pushl   $(RED | BRIGHT)
    CCALL(SetTextColor)
    addl    $(2*4), %esp
    CCALL(kprintf)
    cli
    hlt

/* << End of the basic error handlers. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



.section    .data
/* >> Variables and text messages. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* The opening messages. */
banner_msg0:
    .asciz  "Starting Modulus "
banner_msg1:
    .asciz  __MODULUS_VERSION__
banner_msg2:
    .asciz  "...\n"
compile_info0:
    .asciz  "Kernel compiled on %s at %s.\n"
compile_info1:
    .asciz  __DATE__
compile_info2:
    .asciz  __TIME__

/* Error messages. */
magic_error_msg:
    .asciz  "Boot loader is not multiboot compliant. Try using GRUB.\n"
assert_msg:
    .asciz  "Value = %x, %x\n"
int_msg:
	.asciz  "Unknown interrupt or fault...ignoring.\n"

/* << End of variables and text messages. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >> The Global Descriptor Table. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* The Global Descriptor Table contains 16 quadwords. */
ENTRY(GDT_base)
    .quad   0x0000000000000000        /* 0x00: NULL descriptor */
    .quad   0x0000000000000000        /* 0x08: Unused */
    .quad   0x0000000000000000        /* 0x10: Unused */
    .quad   0x0000000000000000        /* 0x18: Unused */
    
    .quad   0x00cf9a000000ffff        /* 0x20: kernel 4GB code at 0x00000000 */
    .quad   0x00cf92000000ffff        /* 0x28: kernel 4GB data at 0x00000000 */
    .quad   0x00cffa000000ffff        /* 0x30: user 4GB code at 0x00000000 */
    .quad   0x00cff2000000ffff        /* 0x38: user 4GB data at 0x00000000 */
    
    .quad   0x0000000000000000        /* 0x40: TSS descriptor */
    .quad   0x0000000000000000        /* 0x48: LDT descriptor */
    .quad   0x0000000000000000        /* 0x50: Unused */
    .quad   0x0000000000000000        /* 0x58: Unused */
    
    .quad   0x0000000000000000        /* 0x60: Double-fault TSS */
    .quad   0x0000000000000000        /* 0x68: Unused */
    .quad   0x0000000000000000        /* 0x70: Unused */
    .quad   0x0000000000000000        /* 0x78: Unused */
GDT_limit:

/* The GDT descriptor. */
ENTRY(GDT_descriptor)                 /* Finally, the GDT descriptor. */
    .short  GDT_limit-GDT_base
    .long   GDT_base
/* << End of the Global Descriptor Table. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >> The page directory and tables for the DMA zone. >>>>>>>>>>>>>>>>>>>>>>> */
    .align __PAGE_SIZE__, 0
    
    .globl  _PageDirectory
_PageDirectory:
    .fill __PAGE_SIZE__, 1, 0
    
    .globl  _PageTable0
_PageTable0:
    .fill __PAGE_SIZE__, 1, 0
    
    .globl  _PageTable1
_PageTable1:
    .fill __PAGE_SIZE__, 1, 0
    
    .globl  _PageTable2
_PageTable2:
    .fill __PAGE_SIZE__, 1, 0
    
    .globl  _PageTable3
_PageTable3:
    .fill __PAGE_SIZE__, 1, 0

/* << End of the page tables. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >> The kernel's stack area. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
.section    .bss
    .comm	stack, STACK_SIZE
/* << End of the kernel's stack area. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
