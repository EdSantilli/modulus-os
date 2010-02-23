/***************************************************************************
 *            gdt.h
 *
 *  Wed Apr  4 14:20:07 2007
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

#ifndef __GDT_H__INCLUDED__
#define __GDT_H__INCLUDED__


/* Global Descriptor Table values */
#define __KERN_CS_ENTRY 4                   /* Fix these if the GDT changes.  */
#define __KERN_DS_ENTRY 5
#define __USER_CS_ENTRY 6
#define __USER_DS_ENTRY 7

#define __KERN_CS   (__KERN_CS_ENTRY * 8)   /* Do not change these! These are */          
#define __KERN_DS   (__KERN_DS_ENTRY * 8)   /*  the segment selectors.        */
#define __USER_CS   (__USER_CS_ENTRY * 8)
#define __USER_DS   (__USER_DS_ENTRY * 8)


#endif /*__GDT_H__INCLUDED__*/
