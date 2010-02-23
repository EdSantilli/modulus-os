/***************************************************************************
 *            video.h
 *
 *  Thu Mar  1 20:24:43 2007
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

#ifndef __VIDEO_H__INCLUDED__
#define __VIDEO_H__INCLUDED__


/***********************/
/* Global declarations */
/***********************/
/* Define screen constants - if 80 x 25 changes, check everything in here.*/
#define SCREEN_COLS     80
#define SCREEN_ROWS     25
#define SCREEN_TABGRAN  4

/* Define escape characters */
#define ESC_BS  0x08
#define ESC_TAB 0x09
#define ESC_CR  '\r'
#define ESC_LF  '\n'

/* Define color, hi intensity, and blink macros */
#define BLACK   0x00
#define BLUE    0x01
#define GREEN   0x02
#define CYAN    0x03
#define RED     0x04
#define MAGENTA 0x05
#define YELLOW  0x06
#define WHITE   0x07
#define BRIGHT  0x08
#define BLINK   0x08


/******************/
/* C declarations */
/******************/
#if !defined(__ASSEMBLY__) && !defined(__LINKER_SCRIPT__)

#include "sys/system.h"

/* Function prototypes */
void InitializeVideo();
void cls();
void SetCursorPosX(uint8_t new_x);
void SetCursorPos(uint8_t new_x, uint8_t new_y);
void BumpCursorLeft();
void BunpCursorRight();
void SetTextColor(const uint32_t forecolor, const uint32_t backcolor);
void PutChar(const char c);
void PutString(const char *c);
int kprintf(const char *format, ...); //__printf(1,2);
int ksprintf(char *out, const char *format, ...); //__printf(2,3);

#endif /* !__ASSEMBLY__ && !__LINKER_SCRIPT__*/


#endif /*__VIDEO_H__INCLUDED__*/
