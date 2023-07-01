#ifndef BOARD_H
#define BOARD_H

#include "config.h"

#ifdef STM32F1XX
#include "stm32f10x.h"
#endif

#ifdef STM32F4XX
#include "stm32f4xx.h"
#endif

#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"
#include "flash.h"
#include "ltdc.h"
#include "sdram.h"

#define WEAK __attribute__ ((weak))
#define PACKED	__attribute__((__packed__))

extern volatile int ticks;
	
#ifdef __cplusplus
extern "C" {
#endif

void board_init(void);
void clock_changed(void);
void WEAK msleep(int ms);
int get_tick_count(void);

#ifdef __cplusplus
}
#endif

#endif
