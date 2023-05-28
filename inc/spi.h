#ifndef SPI_H
#define SPI_H

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

#define SPI_NO_CS			-1, -1

typedef struct
{
	int cs_port;
	int cs_pin;
	SPI_TypeDef *regs;
} spi_t;

#ifdef __cplusplus
extern "C" {
#endif

void spi_open(spi_t *spi, int spi_port, int rate, int databits, int msb, int cpolmode, int mosi_port, int mosi_pin, int miso_port, int miso_pin, int sck_port, int sck_pin, int cs_port, int cs_pin);
void spi_start(spi_t *spi);
void spi_stop(spi_t *spi);
int  spi_readwrite(spi_t *spi, unsigned short data);
void spi_send(spi_t *spi, const void *buffer, int count);
void spi_recv(spi_t *spi, void *buffer, int count);
void spi_close(spi_t *spi);

#ifdef __cplusplus
}
#endif

#endif
