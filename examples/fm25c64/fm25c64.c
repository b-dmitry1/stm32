#include "board.h"
#include "fm25c64.h"

void fm25_init(spi_t *spi, int spi_port, int mosi_port, int mosi_pin, int miso_port, int miso_pin, int sck_port, int sck_pin, int cs_port, int cs_pin)
{
	unsigned char temp[4];
	spi_open(spi, spi_port, SPI_SPEED_PCLK_DIV8, SPI_DATABITS_8, SPI_MSB_FIRST, SPI_CPOL_MODE_0,
		mosi_port, mosi_pin, miso_port, miso_pin, sck_port, sck_pin, cs_port, cs_pin);
	fm25_read(spi, 0, temp, sizeof(temp));
}

void fm25_read(spi_t *spi, unsigned short addr, void *buffer, unsigned short count)
{
	int i;
	unsigned char *b = (unsigned char *)buffer;
	spi_start(spi);
	// 03 addrh addrl
	spi_readwrite(spi, 0x03);
	spi_readwrite(spi, addr >> 8);
	spi_readwrite(spi, addr & 0xFF);
	for (i = 0; i < count; i++)
		b[i] = spi_readwrite(spi, 0xFF);
	spi_stop(spi);
}

void fm25_write(spi_t *spi, unsigned short addr, const void *buffer, unsigned short count)
{
	int i;
	const unsigned char *b = (const unsigned char *)buffer;
	spi_start(spi);
	spi_readwrite(spi, 0x06);
	spi_stop(spi);
	spi_start(spi);
	spi_readwrite(spi, 0x02);
	spi_readwrite(spi, addr >> 8);
	spi_readwrite(spi, addr & 0xFF);
	for (i = 0; i < count; i++)
		spi_readwrite(spi, b[i]);
	spi_stop(spi);
	spi_start(spi);
	spi_readwrite(spi, 0x04);
	spi_stop(spi);
}
