#include <stdio.h>
#include "board.h"

#define LED	PORTB, 10

#define RXD1	PORTA, 10
#define TXD1	PORTA, 9

int led = 0;

void timer2_func(void)
{
	/* Blink */
	led = !led;

	/* Blink frequency will be 1/2 timer frequency */
	pin_set(LED, led);

	/* Put short background code here */
}

int main(void)
{
	/* 16 MHz external XTAL, set PLL to 72 MHz */
	// rcc_high_performance(XTAL_16MHZ);

	/* Timer 2 should call function "timer2_func" 2 times per second */
	/* So LED will flash 1 time per second */
	timer_init_periodic_ns(2, TIMER_PERIOD_MS(500), timer2_func);

	pin_conf_output(LED);

	/* Do some foreground work. Timer2 will do the background work */
	for (;;)
	{
		/* Put foreground code here */
	}
}
