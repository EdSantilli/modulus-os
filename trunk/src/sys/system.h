/***************************************************************************
 *            system.h
 *
 *  Tue 05 Feb 2008 11:54:51 EST
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

/* System-wide settings are in the last section of the file. */


#ifndef __SYSTEM_H__INCLUDED__
#define __SYSTEM_H__INCLUDED__



/*! >> This code is compiler-dependent. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* C Code */
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

/*  Some compilers cannot intrinsically handle 64-bit operations. */
#   define __64_BIT_MATH_NEEDED__

/*  Does this compiler conform to C99 standards? */
#   ifndef __STDC__
#       error "This compiler does not conform to the ANSI C standard."
#   endif /* !__STDC__ */

/*  Declare the basic data types. */
#   include "sys/types.h"

/*  Declare memory location and size types. */
#   ifndef size_t
        typedef uint32_t    size_t;
#   endif /* !size_t */

#   ifdef __GNUC__
        /* Here are some GNU macros to optimize code. */
#       define likely(x)           __builtin_expect(!!(x), 1)
#       define unlikely(x)         __builtin_expect(!!(x), 0)

        /* Here are some GNU declaration attributes. */
#       define __align(n)          __attribute__ ((__align__(n)))
#       define __calign            __attribute__ ((__align__))
#       define __legacy            __attribute__ ((__deprecated__))
#       define __fastcall          __attribute__ ((__fastcall__))
#       define __finline           inline __attribute__ ((__always_inline__))
#       define __init              __attribute__ ((__section__ (".init.text")))
#       define __initdata          __attribute__ ((__section__ (".init.data")))
#       define __noinline          __attribute__ ((__noinline__))
#       define __nonnull           __attribute__ ((__nonnull__))
#       define __noreturn          __attribute__ ((__noreturn__))
#       define __nothrow           __attribute__ ((__nothrow__))
#       define __pack              __attribute__ ((__packed__))
#       define __printf(f,n)       __attribute__ ((__format__(printf, f, n)))
#       define __pure              __attribute__ ((__pure__))
#       define __transparent_union __attribute__ ((__transparent_union__))
#       define __unused            __attribute__ ((__unused__))
#   endif /* __GNUC__ */

#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */

/*! << End of compiler-dependent code. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/*! >> This code is platform-dependent. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Linker script code */
#ifdef __LINKER_SCRIPT__
/*  1 MB - Kernel entry point. */
#   define __KERN_START__   0x100000
#endif /* __LINKER_SCRIPT__ */

/* 16 KB - Careful, size matters. */
#define __STACK_SIZE__      0x4000

/* 2 ^ 12 = 4 kBytes. */
#define __PAGE_SIZE_BITS__  12
#define __PAGE_SIZE__       (1 << __PAGE_SIZE_BITS__)

/* Does this processor uses the big or little endian format? */
#define __LITTLE_ENDIAN__

/* Assembly language routines for C code. */
#include "i386.h"

/* C Code */
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)
/*  Use this structure to pass info to all IRQ handlers. */
    typedef struct {
        uint32_t    gs, fs, es, ds;     /* Pushed by PUSH_REGS. */
        uint32_t    edi, esi, ebp, esp, /* Pushed by PUSH_REGS. */
                    ebx, edx, ecx, eax; /* Pushed by PUSH_REGS. */
        uint32_t    int_no, err_code;   /* Pushed manually. */
        uint32_t    eip, cs, eflags,    /* Pushed by the processor automatically. */
                    useresp, ss;        /* Pushed by the processor automatically. */
    } regs_t;
#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */

/*! << End of platform-dependent code. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/*! >> This code is portable...neither platform nor compiler dependent. >>>>> */

/* Kernel identifier - Please don't lie...follow the reasonable rules! */
#define __MODULUS_VERSION__ "1.0"           /* Only edit like this: 1.0.Name. */

/* For release version, do not comment this out! Keep debug.h included!
    Instead, open debug.h and comment out '#define __DEBUG_ON__' */
#include "debug.h"

/* C Code */
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

    /* Get information about the physical memory layout from the linker. */
    extern uint32_t _ld_kern_start;
    extern uint32_t _ld_kern_end;
    extern uint32_t _ld_init_kern_start;
    extern uint32_t _ld_init_kern_end;

    /* Use these macros to access the linker info. */
#   define LD_KERN_START       ((uint32_t)&_ld_kern_start)
#   define LD_KERN_END         ((uint32_t)&_ld_kern_end)
#   define LD_INIT_KERN_START  ((uint32_t)&_ld_init_kern_start)
#   define LD_INIT_KERN_END    ((uint32_t)&_ld_init_kern_end)

#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__ */

/*! << End of portable code. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



#endif /* __SYSTEM_H__INCLUDED__ */


