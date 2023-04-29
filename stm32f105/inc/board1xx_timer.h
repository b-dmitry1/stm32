#ifndef BOARD1XX_TIMER_H
#define BOARD1XX_TIMER_H

#include "stm32f10x.h"

#define TIMER_PERIOD_NS(nanoseconds)	((unsigned int)(nanoseconds  * 1ull))
#define TIMER_PERIOD_US(microseconds)	((unsigned int)(microseconds * 1000ull))
#define TIMER_PERIOD_MS(milliseconds)	((unsigned int)(milliseconds * 1000000ull))
#define TIMER_PERIOD_S(seconds)		((unsigned int)(seconds      * 1000000000ull))

#ifdef __cplusplus
extern "C" {
#endif

void timer_init_periodic_ns(int number, unsigned long long period_ns, void (*callback)(void));

#ifdef __cplusplus
}
#endif

#endif
