#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "fm25c64.h"

#define RXD1		PORTA, 10
#define TXD1		PORTA, 9

#define SPI_PORT	2
#define SPI_CS		PORTB, 12
#define SPI_MISO	PORTB, 14
#define SPI_MOSI	PORTB, 15
#define SPI_SCK		PORTB, 13

spi_t spi;

void fram_dump(int address)
{
	int i, j;
	unsigned char buffer[16];

	printf("\n");

	for (i = 0; i < 64; i += 16)
	{
		printf("%04X  ", address + i);

		fm25_read(&spi, address + i, buffer, 16);

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

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, NO485);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	/* Print system clock values */
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	fm25_init(&spi, SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK, SPI_CS);

	printf("\nWriting \"Test1234567890abcdefghij\" to FRAM address 5: ");
	fm25_write(&spi, 5, "Test1234567890abcdefghij", 24);

	printf("\nRead: ");
	fram_dump(0);

	printf("\nWriting \"9876543210fedcbaTestTest\" to FRAM address 5: ");
	fm25_write(&spi, 5, "9876543210fedcbaTestTest", 24);

	printf("\nRead: ");
	fram_dump(0);

	for (;;)
	{
	}
}
