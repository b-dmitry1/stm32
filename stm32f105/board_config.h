#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "board1xx.h"


#define LED	PORTB, 12


///////////////////////////////////////////////////////////////////////////////
// UARTs
///////////////////////////////////////////////////////////////////////////////
#define UART1_RXD			PORTA, 10
#define UART1_TXD			PORTA, 9
#define UART2_RXD			PORTA, 3
#define UART2_TXD			PORTA, 2
#define UART3_RXD			PORTB, 11
#define UART3_TXD			PORTB, 10

#define UART1_RXD_AF			7
#define UART1_TXD_AF			7
#define UART2_RXD_AF			7
#define UART2_TXD_AF			7
#define UART3_RXD_AF			7
#define UART3_TXD_AF			7

// Buffer sizes
#define UART1_RX_SIZE			32
#define UART1_TX_SIZE			32
#define UART2_RX_SIZE			32
#define UART2_TX_SIZE			32
#define UART3_RX_SIZE			32
#define UART3_TX_SIZE			32

///////////////////////////////////////////////////////////////////////////////
// SPIs
///////////////////////////////////////////////////////////////////////////////
#define SPI1_MISO			PORTA, 6
#define SPI1_MOSI			PORTA, 7
#define SPI1_SCK			PORTA, 5

#define SPI1_MISO_AF			5
#define SPI1_MOSI_AF			5
#define SPI1_SCK_AF			5

#define SPI2_MISO			PORTB, 14
#define SPI2_MOSI			PORTB, 15
#define SPI2_SCK			PORTB, 13

#define SPI2_MISO_AF			5
#define SPI2_MOSI_AF			5
#define SPI2_SCK_AF			5

#define SPI3_MISO			PORTB, 4
#define SPI3_MOSI			PORTB, 5
#define SPI3_SCK			PORTB, 3

#define SPI3_MISO_AF			6
#define SPI3_MOSI_AF			6
#define SPI3_SCK_AF			6

///////////////////////////////////////////////////////////////////////////////
// Flash
///////////////////////////////////////////////////////////////////////////////
#define FLASH_START			0x8000000ul
#define FLASH_PAGE_SIZE			2048
#define FLASH_NUM_PAGES			256


#endif
