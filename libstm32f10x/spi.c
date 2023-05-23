#include "stm32f10x.h"
#include "board.h"

static SPI_TypeDef *spi_regs(int port)
{
	switch (port)
	{
		case 1: if (RCC->APB2ENR & RCC_APB2ENR_SPI1EN) return SPI1;
		case 2: if (RCC->APB1ENR & RCC_APB1ENR_SPI2EN) return SPI2;
	}
	return (SPI_TypeDef *)0;
}

void spi_open(int port, int rate, int databits, int msb, int cpolmode)
{
	SPI_TypeDef *spi;

	switch (port)
	{
		case 1:
			pin_conf(SPI1_MISO, SPI1_MISO_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			pin_conf(SPI1_MOSI, SPI1_MOSI_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			pin_conf(SPI1_SCK, SPI1_SCK_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			pin_conf(SPI2_MISO, SPI2_MISO_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			pin_conf(SPI2_MOSI, SPI2_MOSI_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			pin_conf(SPI2_SCK, SPI2_SCK_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
			break;
	}

	spi = spi_regs(port);
	if (!spi)
	{
		return;
	}

	spi->CR1 = ((databits & 1) << 11) | (1 << 9) | (1 << 8) | ((msb & 1) << 7) | (0 << 6) | ((rate & 7) << 3) | (1 << 2) | (cpolmode & 3);
	spi->CR1 = ((databits & 1) << 11) | (1 << 9) | (1 << 8) | ((msb & 1) << 7) | (1 << 6) | ((rate & 7) << 3) | (1 << 2) | (cpolmode & 3);
}

int spi_readwrite(int port, unsigned short data)
{
	SPI_TypeDef *spi;
	volatile int temp;

	spi = spi_regs(port);
	if (!spi)
	{
		return -1;
	}

	temp = spi->DR;
	(void)temp;
	spi->DR = data;
	while (!(spi->SR & SPI_SR_RXNE));
	return spi->DR;
}

void spi_send(int port, const void *buffer, int count)
{
	const unsigned char *data = (const unsigned char *)buffer;
	int i;
	SPI_TypeDef *spi;
	volatile int temp;

	spi = spi_regs(port);
	if (!spi)
	{
		return;
	}

	temp = spi->DR;
	(void)temp;
	for (i = 0; i < count; i++)
	{
		temp = spi->DR;
		(void)temp;
		spi->DR = *data++;
		while (!(spi->SR & SPI_SR_RXNE));
	}
}

void spi_recv(int port, void *buffer, int count)
{
	unsigned char *data = (unsigned char *)buffer;
	int i;
	SPI_TypeDef *spi;
	volatile int temp;

	spi = spi_regs(port);
	if (!spi)
	{
		return;
	}

	temp = spi->DR;
	(void)temp;
	for (i = 0; i < count; i++)
	{
		spi->DR = 0xFF;
		while (!(spi->SR & SPI_SR_RXNE));
		*data++ = spi->DR;
	}
}

void spi_close(int port)
{
	SPI_TypeDef *spi;

	spi = spi_regs(port);
	if (!spi)
	{
		return;
	}

	spi->CR1 = 0;

	switch (port)
	{
		case 1:
			RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
			break;
	}
}
