#ifndef BOARD4XX_UART_H
#define BOARD4XX_UART_H

#include "stm32f4xx.h"
	
#define ONESTOPBIT			0
#define HALFSTOPBIT			1
#define TWOSTOPBITS			2
#define ONE5STOPBITS			3

#define NOPARITY			0
#define ODDPARITY			1
#define EVENPARITY			2

#define NO485				-1, -1

typedef struct
{
	volatile unsigned char *rxbuf;
	int rxrp, rxwp, rxn, rxsize;
	volatile unsigned char *txbuf;
	int txrp, txwp, txn, txsize;
	int rs485_port, rs485_pin;
	USART_TypeDef *usart;
	unsigned long rxtimeout;
	unsigned long rxinterval;
	unsigned long txtimeout;
	int datamask;
	int goboot;
} uart_struct_t;

extern volatile uart_struct_t uarts[6];

#ifdef __cplusplus
extern "C" {
#endif

// port: 1 - UART1, 2 - UART2, ...
void uart_open(int port, unsigned long speed, int datasize, int parity, int stopbits, int rs485_port, int rs485_pin);
void uart_close(int port);
void uart_set_timeouts(int port, unsigned long rxtimeout, unsigned long rxinterval, unsigned long txtimeout);
void uart_send_byte(int port, unsigned char ch);
void uart_send(int port, const void *data, int size);
void uart_send_ln(int port, const char *s);
void uart_send_str(int port, const char *s);
void uart_send_int(int port, int value);
int  uart_rx_count(int port);
int  uart_recv_byte(int port);
int  uart_recv(int port, void *data, int count);

void uart_disable_irq(int port);
void uart_enable_irq(int port);

#ifdef __cplusplus
}
#endif

#endif
