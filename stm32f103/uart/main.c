#include <stdio.h>
#include "board.h"

int main(void)
{
	int a = 0;

	rcc_high_performance(XTAL_8MHZ);

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, NO485);

	select_stdio_uart(1);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	/* Print system clock values */
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	for (;;)
	{
		a++;
		printf("a = %d\n", a);
	}
}
