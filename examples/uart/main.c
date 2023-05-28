#include <stdio.h>
#include "board.h"

#define RXD1	PORTA, 10
#define TXD1	PORTA, 9

int main(void)
{
	// rcc_high_performance(XTAL_16MHZ);

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, NO485);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	// Print system clock values
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	for (;;)
	{
		// This will echo the char back
		getchar();
	}
}
