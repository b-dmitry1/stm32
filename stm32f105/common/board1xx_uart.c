#include "stm32f10x.h"
#include "board1xx.h"

static unsigned int uarts_open = 0;

static const char *hex = "0123456789ABCDEF";

volatile uart_struct_t uarts[NUM_UARTS];

volatile unsigned char uart1_rx[UART1_RX_SIZE];
volatile unsigned char uart1_tx[UART1_TX_SIZE];
volatile unsigned char uart2_rx[UART2_RX_SIZE];
volatile unsigned char uart2_tx[UART2_TX_SIZE];
volatile unsigned char uart3_rx[UART3_RX_SIZE];
volatile unsigned char uart3_tx[UART3_TX_SIZE];

int stdio_uart = 1;

void select_stdio_uart(int port)
{
	stdio_uart = port >= 1 && port <= NUM_UARTS ? port : stdio_uart;
}

static void uart_irq(int port)
{
	volatile uart_struct_t *u = &uarts[port];
	unsigned long sr = u->usart->SR;
	unsigned char data;

	if (sr & ((1 << 5) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))
	{
		u->rxn++;
		while (u->rxn > u->rxsize)
		{
			u->rxrp++;
			u->rxrp %= u->rxsize;
			u->rxn--;
		}
		data = u->usart->DR & u->datamask;
#ifndef NO_GOBOOT
		// Restart on "GoBoot" sequence
		switch (data)
		{
			case 'G': u->goboot = 1; break;
			case 'o': if ((u->goboot == 1) || (u->goboot == 3) || (u->goboot == 4)) u->goboot++; else u->goboot = 0; break;
			case 'B': if (u->goboot == 2) u->goboot++; else u->goboot = 0; break;
			case 't': if (u->goboot == 5) { NVIC_SystemReset(); } break;
			default: u->goboot = 0; break;
		}
#endif
		u->rxbuf[u->rxwp++] = data;
		u->rxwp %= u->rxsize;
	}
	if (sr & (1 << 6))
	{
		if (u->txn > 0)
		{
			if (u->rs485_port >= 0)
			{
				u->usart->CR1 &= ~(1 << 2);
				pin_high(u->rs485_port, u->rs485_pin);
			}
			u->usart->DR = u->txbuf[u->txrp++] & u->datamask;
			u->txrp %= u->txsize;
			u->txn--;
		} else
		{
			u->usart->CR1 &= ~(1 << 6);
			if (u->rs485_port >= 0)
			{
				u->usart->CR1 |= (1 << 2);
				pin_low(u->rs485_port, u->rs485_pin);
			}
		}
	}
}

void USART1_IRQHandler(void)
{
	uart_irq(0);
}

void USART2_IRQHandler(void)
{
	uart_irq(1);
}

void USART3_IRQHandler(void)
{
	uart_irq(2);
}

void uart_disable_irq(int port)
{
	switch (port)
	{
		case 1: NVIC_DisableIRQ(USART1_IRQn); break;
		case 2: NVIC_DisableIRQ(USART2_IRQn); break;
		case 3: NVIC_DisableIRQ(USART3_IRQn); break;
	}
}

void uart_enable_irq(int port)
{
	switch (port)
	{
		case 1: NVIC_EnableIRQ(USART1_IRQn); break;
		case 2: NVIC_EnableIRQ(USART2_IRQn); break;
		case 3: NVIC_EnableIRQ(USART3_IRQn); break;
	}
}

void uart_open(int port, unsigned long speed, int datasize, int parity, int stopbits, int rs485_port, int rs485_pin)
{
	volatile uart_struct_t *u;
	unsigned long clk = apb1clk;
	unsigned long coeff, fr;
	if (port < 1 || port > NUM_UARTS)
		return;
	u = &uarts[port - 1];

	uart_disable_irq(port);
	
	switch (port)
	{
		case 1: USART1->CR1 = 0; break;
		case 2: USART2->CR1 = 0; break;
		case 3: USART3->CR1 = 0; break;
	}

	u->datamask = 0xFF;
	if (datasize == 7)
		u->datamask = 0x7F;
	
	switch (port)
	{
		case 1:
			pin_conf(UART1_RXD, UART1_RXD_AF, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_MEDIUM);
			pin_conf(UART1_TXD, UART1_TXD_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_MEDIUM);
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			u->rxbuf = uart1_rx;
			u->rxsize = sizeof(uart1_rx);
			u->txbuf = uart1_tx;
			u->txsize = sizeof(uart1_tx);
			u->usart = USART1;
			clk = apb2clk;
			break;
		case 2:
			pin_conf(UART2_RXD, UART2_RXD_AF, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_MEDIUM);
			pin_conf(UART2_TXD, UART2_TXD_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_MEDIUM);
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			u->rxbuf = uart2_rx;
			u->rxsize = sizeof(uart2_rx);
			u->txbuf = uart2_tx;
			u->txsize = sizeof(uart2_tx);
			u->usart = USART2;
			break;
		case 3:
			pin_conf(UART3_RXD, UART3_RXD_AF, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_MEDIUM);
			pin_conf(UART3_TXD, UART3_TXD_AF, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_MEDIUM);
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
			u->rxbuf = uart3_rx;
			u->rxsize = sizeof(uart3_rx);
			u->txbuf = uart3_tx;
			u->txsize = sizeof(uart3_tx);
			u->usart = USART3;
			break;
		default:
			return;
	}
	if (rs485_port >= 0)
	{
		pin_conf_output(rs485_port, rs485_pin);
		pin_low(rs485_port, rs485_pin);
	}
	u->rxtimeout = 100;
	u->rxinterval = 100;
	u->txtimeout = 1000;
	u->rxn = 0;
	u->rxrp = 0;
	u->rxwp = 0;
	u->txn = 0;
	u->txrp = 0;
	u->txwp = 0;
	u->rs485_port = rs485_port;
	u->rs485_pin = rs485_pin;

	coeff = clk / 16ul;
	coeff = coeff * 100ul / speed;
	
	fr = 16 * (coeff % 100);
	if (fr % 100 >= 50)
		fr = fr / 100 + 1;
	else
		fr = fr / 100;
	
	fr = fr & 0x0F;
	
	u->usart->CR1 = 0;
	u->usart->CR2 = (stopbits << 12);
	u->usart->CR3 = 0;
	u->usart->BRR = ((coeff / 100) << 4) + fr;

	switch (parity)
	{
		case ODDPARITY:
			if (datasize == 8)
				u->usart->CR1 |= (1 << 12);
			u->usart->CR1 |= (1 << 10) | (1 << 9);
			break;
		case EVENPARITY:
			if (datasize == 8)
				u->usart->CR1 |= (1 << 12);
			u->usart->CR1 |= (1 << 10);
			break;
	}

	u->usart->CR1 |= (1 << 5);
	u->usart->CR1 |= (1 << 3) | (1 << 2);
	u->usart->CR1 |= (1 << 13);

	uarts_open |= 1 << port;

	u->usart->DR = '>';
	
	uart_enable_irq(port);
}

void uart_close(int port)
{
	if (port < 1 || port > NUM_UARTS)
		return;

	uarts_open &= ~(1 << port);

	uart_disable_irq(port);
	
	switch (port)
	{
		case 1: USART1->CR1 = 0; break;
		case 2: USART2->CR1 = 0; break;
		case 3: USART3->CR1 = 0; break;
	}
}

void uart_set_timeouts(int port, int rxtimeout, int rxinterval, int txtimeout)
{
	if (port < 1 || port > NUM_UARTS)
		return;
	uarts[port - 1].rxtimeout = rxtimeout;
	uarts[port - 1].rxinterval = rxinterval;
	uarts[port - 1].txtimeout = txtimeout;
}

void uart_send_byte(int port, unsigned char ch)
{
	volatile uart_struct_t *u;
	if (port < 1 || port > NUM_UARTS)
		return;
	if (!(uarts_open & (1 << port)))
		return;

	u = &uarts[port - 1];
	while (u->txn >= u->txsize);
	if (u->rs485_port >= 0)
		pin_high(u->rs485_port, u->rs485_pin);
	uart_disable_irq(port);
	u->txbuf[u->txwp++] = ch;
	u->txwp %= u->txsize;
	u->txn++;
	u->usart->CR1 |= (1 << 6);
	uart_enable_irq(port);
}

void uart_send(int port, const void *data, int size)
{
	const unsigned char *p = (const unsigned char *)data;
	while (size --> 0)
		uart_send_byte(port, *p++);
}

void uart_send_str(int port, const char *s)
{
	while (*s)
		uart_send_byte(port, *s++);
}

void uart_send_ln(int port, const char *s)
{
	while (*s)
		uart_send_byte(port, *s++);
	uart_send_byte(port, '\r');
	uart_send_byte(port, '\n');
}

void uart_send_int(int port, int value)
{
	int i, v;
	char s[12];

	if (value < 0)
	{
		uart_send_byte(port, '-');
		value = -value;
	}

	for (i = 0, v = value; i < 11; i++, v /= 10)
		s[i] = hex[v % 10];

	for (i = 0; i < 10 && s[10 - i] == '0'; i++);

	for (; i < 11; i++)
		uart_send_byte(port, s[10 - i]);
}

int uart_rx_count(int port)
{
	if (port < 1 || port > NUM_UARTS)
		return 0;
	if (!(uarts_open & (1 << port)))
		return -1;
	return uarts[port - 1].rxn;
}

int uart_blocking_recv_byte(int port)
{
	int res, n;
	volatile uart_struct_t *u;
	if (port < 1 || port > NUM_UARTS)
		return -1;
	if (!(uarts_open & (1 << port)))
		return -1;
	u = &uarts[port - 1];
	while (1)
	{
		uart_disable_irq(port);
		n = u->rxn;
		uart_enable_irq(port);
		if (n != 0)
			break;
		msleep(0);
	}
	uart_disable_irq(port);
	res = u->rxbuf[u->rxrp++];
	u->rxrp %= u->rxsize;
	u->rxn--;
	uart_enable_irq(port);
	return res;
}

int uart_recv_byte_timeout(int port, int timeout)
{
	int res, n;
	volatile uart_struct_t *u;
	int t;
	if (port < 1 || port > NUM_UARTS)
		return -1;
	if (!(uarts_open & (1 << port)))
		return -1;
	u = &uarts[port - 1];
	if (timeout == -1)
		timeout = u->rxtimeout;
	else if (timeout == -2)
		timeout = u->rxinterval;
	if (u->rs485_port >= 0)
	{
		/* Wait while RS485 transmit buffer is not empty */
		for (;;)
		{
			uart_disable_irq(port);
			n = u->txn;
			uart_enable_irq(port);
			if (n == 0)
				break;
		}
	}
	for (t = 0; ; t++)
	{
		uart_disable_irq(port);
		n = u->rxn;
		uart_enable_irq(port);
		if (n > 0)
			break;
		if (t >= timeout)
			return -1;
		msleep(1);
	}
	uart_disable_irq(port);
	res = u->rxbuf[u->rxrp++];
	u->rxrp %= u->rxsize;
	u->rxn--;
	uart_enable_irq(port);
	return res;
}

int uart_recv_byte(int port)
{
	return uart_recv_byte_timeout(port, -1);
}

int uart_recv(int port, void *buffer, int count)
{
	unsigned char *p = (unsigned char *)buffer;
	int ch, n = 0;

	if (count <= 0)
		return 0;

	ch = uart_recv_byte(port);
	if (ch < 0)
		return 0;

	*p++ = ch;
	count--;
	n++;

	while (count --> 0)
	{
		ch = uart_recv_byte_timeout(port, -2);
		if (ch < 0)
			return n;
		*p++ = ch;
		n++;
	}

	return n;
}
