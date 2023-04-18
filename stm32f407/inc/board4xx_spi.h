#ifndef BOARD4XX_SPI_H
#define BOARD4XX_SPI_H

#include "stm32f4xx.h"

#define SPI_SPEED_PCLK_DIV2		0
#define SPI_SPEED_PCLK_DIV4		1
#define SPI_SPEED_PCLK_DIV8		2
#define SPI_SPEED_PCLK_DIV16		3
#define SPI_SPEED_PCLK_DIV32		4
#define SPI_SPEED_PCLK_DIV64		5
#define SPI_SPEED_PCLK_DIV128		6
#define SPI_SPEED_PCLK_DIV256		7

#define SPI_DATABITS_8			0
#define SPI_DATABITS_16			1

#define SPI_MSB_FIRST			0
#define SPI_LSB_FIRST			1

#define SPI_CPOL_MODE_0			0
#define SPI_CPOL_MODE_1			1
#define SPI_CPOL_MODE_2			2
#define SPI_CPOL_MODE_3			3

#ifdef __cplusplus
extern "C" {
#endif

void spi_open(int port, int rate, int databits, int msb, int cpolmode);
int  spi_readwrite(int port, unsigned short data);
void spi_send(int port, const void *buffer, int count);
void spi_recv(int port, void *buffer, int count);
void spi_close(int port);

#ifdef __cplusplus
}
#endif

#endif
