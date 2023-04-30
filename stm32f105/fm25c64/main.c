#include <stdio.h>
#include <stdlib.h>
#include "board1xx.h"
#include "fm25c64.h"

void fram_dump(int address)
{
	int i, j;
	unsigned char buffer[16];

	printf("\n");

	for (i = 0; i < 64; i += 16)
	{
		printf("%04X  ", address + i);

		fm25_read(address + i, buffer, 16);

		for (j = 0; j < 16; j++)
		{
			printf("%.2X ", buffer[j]);
		}

		printf(" ");

		for (j = 0; j < 16; j++)
		{
			printf("%c", buffer[j] >= 32 ? buffer[j] : '.');
		}

		printf("\n");
	}
}

int main(void)
{
	rcc_high_performance(XTAL_16MHZ);

	uart_open(2, 115200, 8, NOPARITY, ONESTOPBIT, NO485);

	select_stdio_uart(2);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	/* Print system clock values */
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	fm25_init();

	printf("\nWriting \"Test1234567890abcdefghij\" to FRAM address 5: ");
	fm25_write(5, "Test1234567890abcdefghij", 24);

	printf("\nRead: ");
	fram_dump(0);

	printf("\nWriting \"9876543210fedcbaTestTest\" to FRAM address 5: ");
	fm25_write(5, "9876543210fedcbaTestTest", 24);

	printf("\nRead: ");
	fram_dump(0);

	for (;;)
	{
	}
}
