#include <string.h>
#include "stm32f10x.h"
#include "board.h"

volatile int ticks = 0;

void timer4_func(void)
{
	ticks++;
}

void WEAK msleep(int ms)
{
	int t, cycles;

	while (ms --> 0)
	{
		t = ticks;
		/* Add a timeout to prevent hangs if Timer 4 is stopped */
		for (cycles = 0; cycles < 10000 && t == ticks; cycles++);
	}
}

int get_tick_count(void)
{
	return ticks;
}

void board_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	
	PWR->CR |= PWR_CR_DBP;
	
	/* Disable JTAG */
	AFIO->MAPR &= ~(7 << 24ul);
	AFIO->MAPR |= (2 << 24ul);

	memset((void *)uarts, 0, sizeof(uarts));

	/* Need Timer 4 to measure time intervals */
	/* SysTick can be used instead but it is reserved for RTOS */
	timer_init_periodic_ns(4, TIMER_PERIOD_MS(1), timer4_func);
}

void clock_changed(void)
{
	// Reprogram system timer
	timer_init_periodic_ns(4, TIMER_PERIOD_MS(1), timer4_func);
}
