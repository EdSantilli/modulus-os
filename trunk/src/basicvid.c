/***************************************************************************
 *            video.c
 *
 *  Sun Feb 25 18:40:05 2007
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

#include "basicvid.h"
#include "string.h"


/* Forward declaration of static functions. */
static void _Scroll();
static void _MoveCursor();


/* Define the pointer to video memory. */
static uint16_t *const  _VM_BASEPTR = (uint16_t *)0xB8000;

/* Cursor state variables */
static uint16_t _ScreenX;
static uint16_t _ScreenY;
static uint16_t _Attr;


/* Scrolls the screen if we are beyond the bottom. */
static void _Scroll() {
    if(_ScreenY >= SCREEN_ROWS)
    {
        /* Copy the region that is still visible to the top of the screen. */
        uint16_t temp = _ScreenY - SCREEN_ROWS + 1;

        MemCopy8(_VM_BASEPTR + temp * SCREEN_COLS,
                 _VM_BASEPTR,
                 2 * (SCREEN_ROWS - temp) * SCREEN_COLS);

        /* Clear out the last line of the screen. */
        MemSet16((' ' | _Attr),
                 _VM_BASEPTR + (SCREEN_ROWS - temp) * SCREEN_COLS,
                 SCREEN_COLS);
        
        /* Set cursor to the last line. */
        _ScreenY = SCREEN_ROWS - 1;
    }
}


/* Updates the hardware cursor. */
static void _MoveCursor() {
    uint16_t screen_offset = _ScreenY * SCREEN_COLS + _ScreenX;
    
    /* This sends a command to indicies 14 and 15 in the
     * CRT Control Register of the VGA controller. These
     * are the high and low bytes of the index that show
     * where the hardware cursor is to be 'blinking'. To
     * learn more, you should look up some VGA specific
     * programming documents. A great start to graphics:
     * http://www.brackeen.com/home/vga */
    out8(0x3D4, 14);
    out8(0x3D5, screen_offset >> 8);
    out8(0x3D4, 15);
    out8(0x3D5, screen_offset);
}


/* Sets the video memory pointer and clears the screen */
void InitializeVideo() {
    SetTextColor(WHITE, BLACK);
    cls();
}


/* Clears the screen */
void cls() {
    uint16_t screen_offset;
    for(screen_offset = 0;
        screen_offset < SCREEN_COLS * SCREEN_ROWS;
        screen_offset++) {
        
        *(_VM_BASEPTR + screen_offset) = ' ' | _Attr;
    }
    
    _ScreenX = 0;
    _ScreenY = 0;
}


/* Set the position of the cursor. */
void SetCursorPosX(uint8_t new_x) {
    SetCursorPos(new_x, _ScreenY);
}
void SetCursorPos(uint8_t new_x, uint8_t new_y) {
    if ((new_x >= SCREEN_COLS) || (new_y >= SCREEN_ROWS)) return;
    
    _ScreenX = new_x % SCREEN_COLS;
    _ScreenY = new_y % SCREEN_ROWS;
    _MoveCursor();
}
void BumpCursorLeft() {
    if(_ScreenX == 0 && _ScreenY != 0) {
        _ScreenX = SCREEN_COLS - 1;
        _ScreenY--;
    }
    else _ScreenX--;
    
    _Scroll();
    _MoveCursor();
}
void BumpCursorRight() {
   if(_ScreenX == SCREEN_COLS - 1) {
        _ScreenX = 0;
        _ScreenY++;
    }
    else _ScreenX++;
    
    _Scroll();
    _MoveCursor();
}



/* Sets the current color attribute */
void SetTextColor(const uint32_t forecolor, const uint32_t backcolor) {
    _Attr = (uint16_t)((backcolor << 4) | forecolor) << 8;
}


/* Tests for escape characters, _Print_s one character, then moves the cursor. */
void PutChar(const char c) {
    if(c == ESC_BS) {
        if(_ScreenX != 0) _ScreenX--;
    }
    else if(c == ESC_TAB) {
        _ScreenX = (_ScreenX + SCREEN_TABGRAN) & ~(SCREEN_TABGRAN - 1);
    }
    else if(c == ESC_CR) {
        _ScreenX = 0;
    }
    else if(c == ESC_LF) {
        _ScreenX = 0;
        _ScreenY++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ') {
        *(_VM_BASEPTR + (_ScreenY * SCREEN_COLS + _ScreenX)) = c | _Attr;
        _ScreenX++;
    }

    if(_ScreenX >= SCREEN_COLS) {
        _ScreenX = 0;
        _ScreenY++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    _Scroll();
    _MoveCursor();
}


/* Prints a string of characters by repeatedly calling PutChar(). */
void PutString(const char *str) {
    uint16_t pos;
    for (pos = 0; pos < strlen(str); pos++)
        PutChar(str[pos]);
}



/* The following lines are all for the kprintf and ksprintf functions. */
/*  TODO: Update all variable declarations. */

static void _PrintChar(char **str, int c)
{
    //extern void PutChar(int c);
    if (str) {
        **str = c;
        ++(*str);
    }
    else PutChar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int _Print_s(char **out, const char *string, int width, int pad)
{
    register int pc = 0, padchar = ' ';

    if (width > 0) {
        register int len = 0;
        register const char *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad & PAD_ZERO) padchar = '0';
    }
    if (!(pad & PAD_RIGHT)) {
        for ( ; width > 0; --width) {
            _PrintChar (out, padchar);
            ++pc;
        }
    }
    for ( ; *string ; ++string) {
        _PrintChar (out, *string);
        ++pc;
    }
    for ( ; width > 0; --width) {
        _PrintChar (out, padchar);
        ++pc;
    }

    return pc;
}

/* the following should be enough for 32 bit int */
// #define PRINT_BUF_LEN 12
/* the following should be enough for 64 bit int */
#define PRINT_BUF_LEN 22

/* out = ?
   i = integer to print,
   b = number base,
   sg = set to 0 to turn negative signature off and 1 to turn it on,
   width = width of number on the screen,
   pad = set to PAD_ZERO to print zeros as width padding
   letbase = ASCII offset for special chars*/
static int _Print_i(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
    char print_buf[PRINT_BUF_LEN];
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return _Print_s (out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) {
        t = u % b;
        if( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if( width && (pad & PAD_ZERO) ) {
            _PrintChar (out, '-');
            ++pc;
            --width;
        }
        else {
            *--s = '-';
        }
    }

    return pc + _Print_s (out, s, width, pad);
}

static int _Print(char **out, int *varg)
{
    register int width, pad;
    register int pc = 0;
    register char *format = (char *)(*varg++);
    char scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0') break;
            if (*format == '%') goto out;
            if (*format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for ( ; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if( *format == 's' ) {
                register char *s = *((char **)varg++);
                pc += _Print_s (out, s?s:"(null)", width, pad);
                continue;
            }
            if( *format == 'd' ) {
                pc += _Print_i (out, *varg++, 10, 1, width, pad, 'a');
                continue;
            }
            if( *format == 'x' ) {
                pc += _Print_i (out, *varg++, 16, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'X' ) {
                pc += _Print_i (out, *varg++, 16, 0, width, pad, 'A');
                continue;
            }
            if( *format == 'A' ) {
                if(*(++varg) == 0) {
                    pc += _Print_i (out, *(--varg), 16, 1, width, pad, 'A');
                    ++varg; ++varg;
                } else {
                    if(width > 8) width -= 8; else width = 0;
                    pc += _Print_i (out, *(varg--), 16, 1, width, pad, 'A');
                    pc += _Print_i (out, *(varg++), 16, 1, 8, PAD_ZERO, 'A');
                    ++varg;
                }
                continue;
            }
            if( *format == 'u' ) {
                pc += _Print_i (out, *varg++, 10, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'c' ) {
                /* char are converted to int then pushed on the stack */
                scr[0] = *varg++;
                scr[1] = '\0';
                pc += _Print_s (out, scr, width, pad);
                continue;
            }
        }
        else {
                out:
                        _PrintChar (out, *format);
                ++pc;
        }
    }
    if (out) **out = '\0';
    return pc;
}

/* assuming sizeof(void *) == sizeof(int) */

int kprintf(const char *format, ...)
{
    register int *varg = (int *)(&format);
    return _Print(0, varg);
}

int ksprintf(char *out, const char *format, ...)
{
    register int *varg = (int *)(&format);
    return _Print(&out, varg);
}
