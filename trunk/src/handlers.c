/***************************************************************************
 *            handlers.c
 *
 *  Tue 05 Feb 2008 20:47:00 EST
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


#include "handlers.h"
#include "idt.h"
#include "basicvid.h"



/* >> The TEMPORARY ISR handler declarations. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
extern void ignore_int();

void divide_error();
void debug();
void nmi();
void int3();

void overflow();   /* int 4 can be called by all. */
void bounds();
void invalid_op();
void device_not_available();

//set_kern_gate(8, GDT_ENTRY_DOUBLEFAULT_TSS);
void coprocessor_segment_overrun();
void invalid_TSS();
void segment_not_present();

void stack_segment();
void general_protection();
//void page_fault();
void spurious_interrupt_bug();

void coprocessor_error();
void alignment_check();
void reserved_error();
/* << End of the the TEMPORARY ISR handler declarations. <<<<<<<<<<<<<<<<<<<< */



/* >> The external ISR and IRQ handler declarations. >>>>>>>>>>>>>>>>>>>>>>>> */
/* ISRs */
extern void isr14();    /* Page fault */

/* IRQs */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();

extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();

extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();

extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
/* << End of the external ISR and IRQ handler declarations. <<<<<<<<<<<<<<<<< */



/* Installs the interrupt traps into memory. */
void init_handlers() {
    /* ISRs */
    set_kern_trap_gate(0, &divide_error);
    set_kern_trap_gate(1, &debug);
    set_kern_int_gate(2, &nmi);
    set_kern_int_gate(3, &int3);        /* int 3 can be called by all. */

    set_kern_trap_gate(4, &overflow);   /* int 4 can be called by all. */
    set_kern_trap_gate(5, &bounds);
    set_kern_trap_gate(6, &invalid_op);
    set_kern_trap_gate(7, &device_not_available);

    set_kern_int_gate(8, &ignore_int); //~ set_kern_gate(8, GDT_ENTRY_DOUBLEFAULT_TSS);
    set_kern_trap_gate(9, &coprocessor_segment_overrun);
    set_kern_trap_gate(10, &invalid_TSS);
    set_kern_trap_gate(11, &segment_not_present);

    set_kern_trap_gate(12, &stack_segment);
    set_kern_trap_gate(13, &general_protection);
    set_kern_int_gate(14, &isr14);
    set_kern_trap_gate(15, &spurious_interrupt_bug);
    
    set_kern_trap_gate(16, &coprocessor_error);
    set_kern_trap_gate(17, &alignment_check);
    set_kern_int_gate(18, &reserved_error);
    set_kern_int_gate(19, &reserved_error);
    
    set_kern_int_gate(20, &reserved_error);
    set_kern_int_gate(21, &reserved_error);
    set_kern_int_gate(22, &reserved_error);
    set_kern_int_gate(23, &reserved_error);
    
    set_kern_int_gate(24, &reserved_error);
    set_kern_int_gate(25, &reserved_error);
    set_kern_int_gate(26, &reserved_error);
    set_kern_int_gate(27, &reserved_error);
    
    set_kern_int_gate(28, &reserved_error);
    set_kern_int_gate(29, &reserved_error);
    set_kern_int_gate(30, &reserved_error);
    set_kern_int_gate(31, &reserved_error);
    
    /* Remap the PIC so that IRQ 0-15 is sent to IDT entries 32-47. */
    out8(0x20, 0x11);
    out8(0xA0, 0x11);
    out8(0x21, 0x20);
    out8(0xA1, 0x28);
    out8(0x21, 0x04);
    out8(0xA1, 0x02);
    out8(0x21, 0x01);
    out8(0xA1, 0x01);
    out8(0x21, 0x00);
    out8(0xA1, 0x00);
    
    /* IRQs */
    set_kern_int_gate(32, &irq0);
    set_kern_int_gate(33, &irq1);
    set_kern_int_gate(34, &irq2);
    set_kern_int_gate(35, &irq3);
    
    set_kern_int_gate(36, &irq4);
    set_kern_int_gate(37, &irq5);
    set_kern_int_gate(38, &irq6);
    set_kern_int_gate(39, &irq7);
    
    set_kern_int_gate(40, &irq8);
    set_kern_int_gate(41, &irq9);
    set_kern_int_gate(42, &irq10);
    set_kern_int_gate(43, &irq11);
    
    set_kern_int_gate(44, &irq12);
    set_kern_int_gate(45, &irq13);
    set_kern_int_gate(46, &irq14);
    set_kern_int_gate(47, &irq15);
}



/* >> TEMPORARY error handlers. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void divide_error() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: Divide by zero.\n");
    SystemHalt();
}

void debug() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: debug\n");
    SystemHalt();
}
    
void nmi() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: nmi\n");
    SystemHalt();
}

void int3() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: int3\n");
    SystemHalt();
}

void overflow() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: overflow\n");
    SystemHalt();
}

void bounds() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: bounds\n");
    SystemHalt();
}

void invalid_op() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: invalid_op\n");
    SystemHalt();
}

void device_not_available() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: device_not_available\n");
    SystemHalt();
}

void coprocessor_segment_overrun() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: coprocessor_segment_overrun\n");
    SystemHalt();
}

void invalid_TSS() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: invalid_TSS\n");
    SystemHalt();
}

void segment_not_present() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: segment_not_present\n");
    SystemHalt();
}

void stack_segment() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: stack_segment\n");
    SystemHalt();
}

void general_protection() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: general_protection\n");
    SystemHalt();
}

void spurious_interrupt_bug() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: spurious_interrupt_bug\n");
    SystemHalt();
}

void coprocessor_error() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: coprocessor_error\n");
    SystemHalt();
}

void alignment_check() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: alignment_check\n");
    SystemHalt();
}

void reserved_error() {
    DisableInts();
    SetTextColor(RED | BRIGHT, BLACK);
    kprintf("\nERROR: reserved error\n");
    SystemHalt();
}

/* << End of TEMPORARY error handlers. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* Function pointers to the IRQ handlers. */
static void *irq_handlers[16] = {
    (void *)0, (void *)0, (void *)0, (void *)0,
    (void *)0, (void *)0, (void *)0, (void *)0,
    (void *)0, (void *)0, (void *)0, (void *)0,
    (void *)0, (void *)0, (void *)0, (void *)0
};

typedef uint32_t     irq_retval_t;
typedef irq_retval_t (*irq_handler_t)(regs_t *);
typedef uint32_t     irq_flags_t;

#define IRQ_FLAG_CRITICAL       ((irq_flags_t)0x0001)
#define IRQ_FLAG_NONCRITICAL    ((irq_flags_t)0x0002)
#define IRQ_FLAG_INPROGRESS     ((irq_flags_t)0x0010)
#define IRQ_FLAG_DISABLED       ((irq_flags_t)0x0020)
#define IRQ_FLAG_PENDING        ((irq_flags_t)0x0040)
#define IRQ_FLAG_WAITING        ((irq_flags_t)0x0080)

// struct irq_action {
//     char                *Name;
//     irq_handler_t       pHandler;
//     irq_flags_t         flags;
//     struct irq_action   *pNextAction;
// };


bool_t irq_install_handler(uint8_t irq_no, void (*pFunc)(regs_t *r)) {
    /* Is ths a valid IRQ number? */
    if(irq_no >= 16) return true;
    
    /* Is this IRQ line already in use? */
    if(irq_handlers[irq_no]) return true;
    
    /* Set the IRQ line and return without error. */
    irq_handlers[irq_no] = pFunc;
    return false;
}

void irq_uninstall_handler(uint8_t irq_no) {
    /* Remove the IRQ handler only if we have a valid IRQ number. */
    if(irq_no < 16) irq_handlers[irq_no] = (void *)0;
}

void irq_common_handler(regs_t *r) {
    /* This is a blank function pointer */
    static void (*handler)(regs_t *r);

    /* If an IRQ handler is installed, call it. */
    handler = irq_handlers[r->int_no - 32];
    if(handler) handler(r);
    
    /* Send EOI to controllers. */
    if (r->int_no >= 40) out8(0xA0, 0x20);
    out8(0x20, 0x20);
}
