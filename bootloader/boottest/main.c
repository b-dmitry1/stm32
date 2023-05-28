#include <stdio.h>
#include "board.h"

#define RXD1	PORTA, 10
#define TXD1	PORTA, 9

const char *version = "$AppInfo$1.02 [14.05.2009]";

int value = 12345;

void wait(void)
{
	for (volatile int w = 0; w < 1000000; w++);
}

int main(void)
{
	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, NO485);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("\nTest app\n");

	// Print system clock values
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	for (;;)
	{
		printf("%d\n", value++);

		wait();
	}
}
