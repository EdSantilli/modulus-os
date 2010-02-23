/* FILE: idt.c
 * BORN: 08-02-2007
 * DESC: Defines the IDT and the functions that maintain it. */

#include "idt.h"
#include "gdt.h"


/* Define an IDT entry. */
typedef struct __pack {
    uint16_t    offset_low;
    uint16_t    selector;
    uint16_t    attr;
    uint16_t    offset_high;
} idt_entry_t;

/* Define the IDT. */
static idt_entry_t idt[IDT_COUNT];


/* Always use this function to change the IDT. Never access the IDT directly. */
void set_user_int_gate(uint8_t index, void (*pFunc)(void)) {
    idt[index].offset_low  = (((uint32_t)pFunc) & 0xFFFF);
    idt[index].offset_high = ((((uint32_t)pFunc) >> 16) & 0xFFFF);
    idt[index].selector    = __USER_CS;
    idt[index].attr        = 0xEE00;
}

void set_user_trap_gate(uint8_t index, void (*pFunc)(void)) {
    idt[index].offset_low  = (((uint32_t)pFunc) & 0xFFFF);
    idt[index].offset_high = ((((uint32_t)pFunc) >> 16) & 0xFFFF);
    idt[index].selector    = __USER_CS;
    idt[index].attr        = 0xEF00;
}

void set_kern_int_gate(uint8_t index, void (*pFunc)(void)) {
    idt[index].offset_low  = (((uint32_t)pFunc) & 0xFFFF);
    idt[index].offset_high = ((((uint32_t)pFunc) >> 16) & 0xFFFF);
    idt[index].selector    = __KERN_CS;
    idt[index].attr        = 0x8E00;
}

void set_kern_trap_gate(uint8_t index, void (*pFunc)(void)) {
    idt[index].offset_low  = (((uint32_t)pFunc) & 0xFFFF);
    idt[index].offset_high = ((((uint32_t)pFunc) >> 16) & 0xFFFF);
    idt[index].selector    = __KERN_CS;
    idt[index].attr        = 0x8F00;
}


/* SetLIDT is a platform-dependent routine. It is defined in i386.h. */
void init_idt() {
    SetLIDT((void *)idt, IDT_COUNT);
}
