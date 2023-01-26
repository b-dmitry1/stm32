#ifndef BOARD103_H
#define BOARD103_H

#include <stm32f10x.h>

#define HCLK				72000000

// Config
#define UART1_RX_SIZE		32
#define UART2_RX_SIZE		128
#define UART3_RX_SIZE		32

// Defines
#define INPUT_PULLUP		0
#define INPUT_PULLDOWN		1
#define INPUT				2
#define OUTPUT				3
#define OPENDRAIN			4
#define ALTERNATE			5
#define ANALOG				6

#define STOPBITSMASK		0x30
#define ONESTOPBIT			(0 << 4)
#define TWOSTOPBITS			(2 << 4)

#define PARITYMASK			0xC0
#define NOPARITY			(0 << 6)
#define EVENPARITY			(1 << 6)
#define ODDPARITY			(2 << 6)

#define SERIAL_8N1			(8 | ONESTOPBIT | NOPARITY)
#define SERIAL_8E1			(8 | ONESTOPBIT | EVENPARITY)
#define SERIAL_8O1			(8 | ONESTOPBIT | ODDPARITY)
#define SERIAL_8N2			(8 | TWOSTOPBIT | NOPARITY)
#define SERIAL_8E2			(8 | TWOSTOPBIT | EVENPARITY)
#define SERIAL_8O2			(8 | TWOSTOPBIT | ODDPARITY)
#define SERIAL_7N1			(7 | ONESTOPBIT | NOPARITY)
#define SERIAL_7E1			(7 | ONESTOPBIT | EVENPARITY)
#define SERIAL_7O1			(7 | ONESTOPBIT | ODDPARITY)
#define SERIAL_7N2			(7 | TWOSTOPBIT | NOPARITY)
#define SERIAL_7E2			(7 | TWOSTOPBIT | EVENPARITY)
#define SERIAL_7O2			(7 | TWOSTOPBIT | ODDPARITY)

#define MSBFIRST			0
#define LSBFIRST			1
#define SPI_MODE0			0
#define SPI_MODE1			1
#define SPI_MODE2			2
#define SPI_MODE3			3
#define SPI_CLOCK_DIV2		0
#define SPI_CLOCK_DIV4		1
#define SPI_CLOCK_DIV8		2
#define SPI_CLOCK_DIV16		3
#define SPI_CLOCK_DIV32		4
#define SPI_CLOCK_DIV64		5
#define SPI_CLOCK_DIV128	6

void set_pin_mode(GPIO_TypeDef *base, int pin, int mode);
void set_pin(GPIO_TypeDef *base, int pin);
void clr_pin(GPIO_TypeDef *base, int pin);
void set_pin_to(GPIO_TypeDef *base, int pin, int value);
int get_pin(GPIO_TypeDef *base, int pin);

void uart_open(int number, int speed, int config);
void uart_send_byte(int number, int data);
void uart_send_str(int number, const char *str);
int uart_recv_byte(int number);
int uart_recv(int number, void *buffer, int count);

void spi_open(int number, int clkdiv);
int spi_read_write(int number, int data);

void board_init(void);

#endif
