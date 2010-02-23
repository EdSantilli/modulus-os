#define __LINKER_SCRIPT__
#include "sys/system.h"


OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

_ld_kern_start = __KERN_START__;
_ld_page_size = __PAGE_SIZE__;


SECTIONS {
    .text _ld_kern_start : AT(_ld_kern_start) {
        _ld_text_start = .;
        *(.text);
        _ld_text_end = .;
        . = ALIGN(_ld_page_size);
    }
    
    .rodata ADDR(.text) + SIZEOF(.text): AT(ADDR(.text) + SIZEOF(.text)) {
        _ld_rodata_start = .;
        *(.rodata*);
        _ld_rodata_end = .;
        . = ALIGN(_ld_page_size);
    }
    
    .data ADDR(.rodata) + SIZEOF(.rodata): AT(ADDR(.rodata) + SIZEOF(.rodata)) {
        _ld_data_start = .;
        *(.data);
        _ld_data_end = .;
        . = ALIGN(_ld_page_size);
    }
    
    .bss ADDR(.data) + SIZEOF(.data): AT(ADDR(.data) + SIZEOF(.data)) {
        _ld_bss_start = .;
        *(.bss);
        _ld_bss_end = .;
        . = ALIGN(_ld_page_size);
        _ld_kern_end = .;
    }
    
    .init.text ADDR(.bss) + SIZEOF(.bss): AT(ADDR(.bss) + SIZEOF(.bss)) {
        _ld_init_kern_start = .;
        _ld_init_text_start = .;
        *(.init.text);
        _ld_init_text_end = .;
        . = ALIGN(_ld_page_size);
    }

    .init.data ADDR(.init.text) + SIZEOF(.init.text): AT(ADDR(.init.text) + SIZEOF(.init.text)) {
        _ld_init_data_start = .;
        *(.init.text);
        _ld_init_data_end = .;
    }
    . = ALIGN(_ld_page_size);
    _ld_init_kern_end = .;
}
