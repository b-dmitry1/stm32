#ifndef BOARD_H
#define BOARD_H

#include "board_config.h"

#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"
#include "flash.h"
#include "ltdc.h"

#define WEAK __attribute__ ((weak))
#define PACKED	__attribute__((__packed__))

extern volatile int ticks;
	
#ifdef __cplusplus
extern "C" {
#endif

void board_init(void);
void WEAK msleep(int ms);
int get_tick_count(void);

#ifdef __cplusplus
}
#endif

#endif
