#ifndef _CLOCK_H
#define _CLOCK_H
#include "kernel/types.h"

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040
#define TIMER_FREQ     1193182L/* clock frequency for timer in PC and AT */
#define HZ             100  /* clock freq (software settable on IBM-PC) */

#define CLOCK_IRQ 0

extern int ticks;

void IRQ_clock();
void clock_handler(int irq);
void schdule();

#endif

