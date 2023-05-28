#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "board.h"

void spi_start(spi_t *spi)
{
	if (spi == NULL)
	{
		return;
	}

	if (spi->cs_port >= 0)
	{
		pin_low(spi->cs_port, spi->cs_pin);
	}
}

void spi_stop(spi_t *spi)
{
	if (spi == NULL)
	{
		return;
	}

	if (spi->cs_port >= 0)
	{
		pin_high(spi->cs_port, spi->cs_pin);
	}
}

void spi_open(spi_t *spi, int spi_port, int rate, int databits, int msb, int cpolmode, int mosi_port, int mosi_pin, int miso_port, int miso_pin, int sck_port, int sck_pin, int cs_port, int cs_pin)
{
	if (spi == NULL)
	{
		return;
	}

	memset(spi, 0, sizeof(*spi));

	spi->cs_port = cs_port;
	spi->cs_pin = cs_pin;

	switch (spi_port)
	{
		case 1:
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
			spi->regs = SPI1;
			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
			spi->regs = SPI2;
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
			spi->regs = SPI3;
			break;
		default:
			return;
	}

	if (cs_port >= 0)
	{
		pin_conf_output(cs_port, cs_pin);
		pin_high(cs_port, cs_pin);
	}

	pin_conf(miso_port, miso_pin, 5, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
	pin_conf(mosi_port, mosi_pin, 5, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);
	pin_conf(sck_port, sck_pin, 5, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_HIGH);

	spi->regs->CR1 = ((databits & 1) << 11) | (1 << 9) | (1 << 8) | ((msb & 1) << 7) | (0 << 6) | ((rate & 7) << 3) | (1 << 2) | (cpolmode & 3);
	spi->regs->CR1 = ((databits & 1) << 11) | (1 << 9) | (1 << 8) | ((msb & 1) << 7) | (1 << 6) | ((rate & 7) << 3) | (1 << 2) | (cpolmode & 3);
}

int spi_readwrite(spi_t *spi, unsigned short data)
{
	volatile int temp;

	if (spi == NULL || spi->regs == NULL)
	{
		return 0xFF;
	}

	temp = spi->regs->DR;
	(void)temp;
	spi->regs->DR = data;
	while (!(spi->regs->SR & SPI_SR_RXNE));
	return spi->regs->DR;
}

void spi_send(spi_t *spi, const void *buffer, int count)
{
	const unsigned char *data = (const unsigned char *)buffer;
	int i;
	volatile int temp;

	if (spi == NULL || spi->regs == NULL)
	{
		return;
	}

	for (i = 0; i < count; i++)
	{
		temp = spi->regs->DR;
		(void)temp;
		spi->regs->DR = *data++;
		while (!(spi->regs->SR & SPI_SR_RXNE));
	}
}

void spi_recv(spi_t *spi, void *buffer, int count)
{
	unsigned char *data = (unsigned char *)buffer;
	int i;
	volatile int temp;

	if (spi == NULL || spi->regs == NULL)
	{
		return;
	}

	temp = spi->regs->DR;
	(void)temp;
	for (i = 0; i < count; i++)
	{
		spi->regs->DR = 0xFF;
		while (!(spi->regs->SR & SPI_SR_RXNE));
		*data++ = spi->regs->DR;
	}
}

void spi_close(spi_t *spi)
{
	if (spi == NULL || spi->regs == NULL)
	{
		return;
	}

	spi->regs->CR1 = 0;

	if (spi->regs == SPI1)
		RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
	else if (spi->regs == SPI2)
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
	else if (spi->regs == SPI3)
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;

	spi->regs = NULL;
}
