#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f4xx.h"

#include "board.h"

///////////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////////
#define CPU_CLK				168000000ul
#define APB1_CLK			(CPU_CLK / 4)
#define APB2_CLK			(CPU_CLK / 2)
#define AHB1_CLK			CPU_CLK
#define AHB2_CLK			CPU_CLK

///////////////////////////////////////////////////////////////////////////////
// UARTs
///////////////////////////////////////////////////////////////////////////////
#define UART1_RXD			PORTA, 10
#define UART1_TXD			PORTA, 9
#define UART2_RXD			PORTA, 3
#define UART2_TXD			PORTA, 2
#define UART3_RXD			PORTB, 11
#define UART3_TXD			PORTB, 10
#define UART4_RXD			PORTA, 1
#define UART4_TXD			PORTA, 0
#define UART5_RXD			PORTD, 2
#define UART5_TXD			PORTC, 12
#define UART6_RXD			PORTG, 9
#define UART6_TXD			PORTG, 14

#define UART1_RXD_AF			7
#define UART1_TXD_AF			7
#define UART2_RXD_AF			7
#define UART2_TXD_AF			7
#define UART3_RXD_AF			7
#define UART3_TXD_AF			7
#define UART4_RXD_AF			8
#define UART4_TXD_AF			8
#define UART5_RXD_AF			8
#define UART5_TXD_AF			8
#define UART6_RXD_AF			8
#define UART6_TXD_AF			8

// Buffer sizes
#define UART1_RX_SIZE			32
#define UART1_TX_SIZE			32
#define UART2_RX_SIZE			32
#define UART2_TX_SIZE			32
#define UART3_RX_SIZE			32
#define UART3_TX_SIZE			32
#define UART4_RX_SIZE			32
#define UART4_TX_SIZE			32
#define UART5_RX_SIZE			32
#define UART5_TX_SIZE			32
#define UART6_RX_SIZE			32
#define UART6_TX_SIZE			32

///////////////////////////////////////////////////////////////////////////////
// SPIs
///////////////////////////////////////////////////////////////////////////////
#define SPI1_MISO			PORTA, 6
#define SPI1_MOSI			PORTA, 7
#define SPI1_SCK			PORTA, 5

#define SPI1_MISO_AF			5
#define SPI1_MOSI_AF			5
#define SPI1_SCK_AF			5

#define SPI2_MISO			PORTC, 2
#define SPI2_MOSI			PORTC, 3
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

#endif
