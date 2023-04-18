#include <stdio.h>
#include "board4xx.h"

void wait(void)
{
	volatile int w;
	for (w = 0; w < 10000000; w++);
}

int main(void)
{
	int n = 0;

	board_init();

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, NO485);

	for (;;)
	{
		printf("Value = %d\n", n++);

		wait();
	}
}
