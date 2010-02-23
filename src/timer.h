/***************************************************************************
 *            timer.h
 *
 *  Wed Apr  4 20:21:46 2007
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
 
#ifndef __TIMER_H__INCLUDED__
#define __TIMER_H__INCLUDED__

#include "sys/system.h"

#define TIMER_TICK_FREQ     1193182


/* Function prototypes */
void init_timer() __init;
void timer_wait(uint32_t ticks);
#define timer_swait(secs)   timer_wait(TIMER_TICK_FREQ * (secs))


#endif /*__TIMER_H__INCLUDED__*/
