#ifndef BOARD32F1XX_H
#define BOARD32F1XX_H

#include "board_config.h"
#include "board1xx_rcc.h"
#include "board1xx_gpio.h"
#include "board1xx_uart.h"
#include "board1xx_spi.h"
#include "board1xx_timer.h"
#include "board1xx_flash.h"

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
