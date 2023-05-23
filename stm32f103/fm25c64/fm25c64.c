#include "board.h"
#include "fm25c64.h"

#define SPI			2
#define CS			PORTB, 12

static void select(void)
{
	pin_low(CS);
}

static void deselect(void)
{
	pin_high(CS);
}

static unsigned char spi(unsigned char byte)
{
	return spi_readwrite(SPI, byte);
}

void fm25_init(void)
{
	unsigned char temp[4];
	pin_conf_output(CS);
	pin_high(CS);
	spi_open(SPI, SPI_SPEED_PCLK_DIV8, SPI_DATABITS_8, SPI_MSB_FIRST, SPI_CPOL_MODE_0);
	fm25_read(0, temp, sizeof(temp));
}

void fm25_read(unsigned short addr, void *buffer, unsigned short count)
{
	int i;
	unsigned char *b = (unsigned char *)buffer;
	select();
	// 03 addrh addrl
	spi(0x03);
	spi(addr >> 8);
	spi(addr & 0xFF);
	for (i = 0; i < count; i++)
		b[i] = spi(0xFF);
	deselect();
}

void fm25_write(unsigned short addr, const void *buffer, unsigned short count)
{
	int i;
	const unsigned char *b = (const unsigned char *)buffer;
	select();
	spi(0x06);
	deselect();
	select();
	spi(0x02);
	spi(addr >> 8);
	spi(addr & 0xFF);
	for (i = 0; i < count; i++)
		spi(b[i]);
	deselect();
	select();
	spi(0x04);
	deselect();
}
