/***************************************************************************
 *            timer.c
 *
 *  Wed Apr  4 20:18:57 2007
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


#include "timer.h"
#include "handlers.h"
#include "basicvid.h"


/* Forward declaration of static functions. */
static void timer_phase(uint32_t freq);


/* Timer register names */
#define TIMER_REG_D0      0x40
#define TIMER_REG_D1      0x41
#define TIMER_REG_D2      0x42
#define TIMER_REG_CMD     0x43

/* Timer state constants */
#define TIMER_BCD_OFF     0x00    /* 16-bit counters */
#define TIMER_BCD_ON      0x01    /* 4x BCD decade counters */
#define TIMER_MODE_0      0x00    /* Int on terminal count */
#define TIMER_MODE_1      0x02    /* Hdwr retriggerable 1-shot */
#define TIMER_MODE_2      0x04    /* Rate generator */
#define TIMER_MODE_3      0x06    /* Square wave */
#define TIMER_MODE_4      0x08    /* Software strobe */
#define TIMER_MODE_5      0x0A    /* Hdwr strobe */
#define TIMER_RW_LSB      0x10    /* Least significant bit */
#define TIMER_RW_MSB      0x20    /* Most significant bit */
#define TIMER_RW_LTM      0x30    /* Least then most significant bit */
#define TIMER_CNTR_0      0x00    /* Counter 1 */
#define TIMER_CNTR_1      0x40    /* Counter 2 */
#define TIMER_CNTR_2      0x80    /* Counter 3 */


static struct {
    struct {uint32_t low, high;}    jiffies64;
    struct {uint32_t low, high;}    monotonic_clock64;
} timer = {.jiffies64 = {0, 0}, .monotonic_clock64 = {0, 0}};

uint32_t *_pjiffies = &(timer.jiffies64.low);
#define jiffies     (*_pjiffies)


/* This function is called to handle the timer's IRQs. */
void timer_handler(regs_t *r) {
    jiffies++;
//    if(unlikely(jiffies % 100 == 0)) {
//        kprintf("One second has passed...%u\n", jiffies);
//    }
}


/* Sets up the system clock by installing the timer handler into IRQ0. */
void init_timer() {
    timer_phase(1);
    irq_install_handler(0, &timer_handler);
}

/* This sets how frequently the timer's IRQ is triggered. */
static void timer_phase(uint32_t freq) {
    uint32_t divisor = TIMER_TICK_FREQ / freq;
    out8(TIMER_REG_CMD, TIMER_MODE_3 | TIMER_RW_LTM);
    out8(TIMER_REG_D0, divisor & 0xFF);
    out8(TIMER_REG_D0, divisor >> 8);
}


/* This will continuously loop until the given time has been reached. */
void timer_wait(uint32_t ticks) {
    uint32_t eticks = jiffies + ticks;
    while(jiffies < eticks);
}
