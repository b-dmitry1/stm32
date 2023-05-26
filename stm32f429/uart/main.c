#include <stdio.h>
#include "board.h"

void wait(void)
{
	volatile int w;
	for (w = 0; w < 10000000; w++);
}

int main(void)
{
	int n = 0;

	rcc_high_performance(XTAL_16MHZ);

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, NO485);

	printf("\n\nFrequences:\nsys = %d, ahb = %d, apb1 = %d, apb2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	for (;;)
	{
		printf("Value = %d\n", n++);

		wait();
	}
}
