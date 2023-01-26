#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stm32f10x.h>
#include "board103.h"

unsigned char uart1buf[UART1_RX_SIZE];
int uart1_rp = 0, uart1_wp = 0, uart1_n = 0;
unsigned char uart2buf[UART2_RX_SIZE];
int uart2_rp = 0, uart2_wp = 0, uart2_n = 0;
unsigned char uart3buf[UART3_RX_SIZE];
int uart3_rp = 0, uart3_wp = 0, uart3_n = 0;

void set_pin_mode(GPIO_TypeDef *base, int pin, int mode)
{
	if (pin < 8)
		base->CRL &= ~(0x0F << (pin * 4ul));
	else
		base->CRH &= ~(0x0F << ((pin - 8) * 4ul));
		
	switch (mode)
	{
		case INPUT:
			if (pin < 8)
				base->CRL |= (4 << (pin * 4ul));
			else
				base->CRH |= (4 << ((pin - 8) * 4ul));
			break;
		case INPUT_PULLUP:
			if (pin < 8)
				base->CRL |= (8 << (pin * 4ul));
			else
				base->CRH |= (8 << ((pin - 8) * 4ul));
			base->BSRR = (1u << pin);
			break;
		case INPUT_PULLDOWN:
			if (pin < 8)
				base->CRL |= (8 << (pin * 4ul));
			else
				base->CRH |= (8 << ((pin - 8) * 4ul));
			base->BSRR = (1u << (pin + 16));
			break;
		case OUTPUT:
			if (pin < 8)
				base->CRL |= (3 << (pin * 4ul));
			else
				base->CRH |= (3 << ((pin - 8) * 4ul));
			break;
		case OPENDRAIN:
			if (pin < 8)
				base->CRL |= (7 << (pin * 4ul));
			else
				base->CRH |= (7 << ((pin - 8) * 4ul));
			break;
		case ALTERNATE:
			if (pin < 8)
				base->CRL |= (11 << (pin * 4ul));
			else
				base->CRH |= (11 << ((pin - 8) * 4ul));
			break;
	}
}

void set_pin(GPIO_TypeDef *base, int pin)
{
	base->BSRR = (1u << pin);
}

void clr_pin(GPIO_TypeDef *base, int pin)
{
	base->BSRR = ((1u << 16) << pin);
}

void set_pin_to(GPIO_TypeDef *base, int pin, int value)
{
	if (value)
		set_pin(base, pin);
	else
		clr_pin(base, pin);
}

int get_pin(GPIO_TypeDef *base, int pin)
{
	return (base->IDR & (1u << pin)) ? 1 : 0;
}

USART_TypeDef *uart_base(int number)
{
	if (number == 1)
		return USART1;
	if (number == 2)
		return USART2;
	return USART3;
}

void uart_irq_control(int number, int on)
{
	if (number == 1)
	{
		if (on)
			NVIC_EnableIRQ(USART1_IRQn);
		else
			NVIC_DisableIRQ(USART1_IRQn);
	}
	else if (number == 2)
	{
		if (on)
			NVIC_EnableIRQ(USART2_IRQn);
		else
			NVIC_DisableIRQ(USART2_IRQn);
	}
	else
	{
		if (on)
			NVIC_EnableIRQ(USART3_IRQn);
		else
			NVIC_DisableIRQ(USART3_IRQn);
	}
}

void uart_open(int number, int speed, int config)
{
	USART_TypeDef *base = uart_base(number);
	unsigned long clk = HCLK;
	unsigned char parity = config & PARITYMASK;
	unsigned char stopbits = config & STOPBITSMASK;
	unsigned char bytesize = config & 0x0F;
	unsigned long coeff, fr;

	switch (number)
	{
		case 1:
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			uart1_n = 0;
			uart1_rp = 0;
			uart1_wp = 0;
			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			clk /= 2;
			uart2_n = 0;
			uart2_rp = 0;
			uart2_wp = 0;
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
			clk /= 2;
			uart3_n = 0;
			uart3_rp = 0;
			uart3_wp = 0;
			break;
	}
	
	base->CR1 = 0;
	base->CR2 = 0;
	base->CR3 = 0;
	
	switch (parity)
	{
		case EVENPARITY:
			base->CR1 |= (1 << 10);
			if (bytesize == 8)
				base->CR1 |= (1 << 12);
			break;
		case ODDPARITY:
			base->CR1 |= (1 << 10) | (1 << 9);
			if (bytesize == 8)
				base->CR1 |= (1 << 12);
			break;
	}
	
	switch (stopbits)
	{
		case TWOSTOPBITS:
			base->CR2 |= (2 << 12);
			break;
	}
	
	coeff = clk / 16ul;
	coeff = coeff * 100ul / speed;
	
	fr = 16 * (coeff % 100);
	if (fr % 100 >= 50)
		fr = fr / 100 + 1;
	else
		fr = fr / 100;
	
	fr = fr & 0x0F;
	
	base->BRR = ((coeff / 100) << 4) + fr;

	base->CR1 |= (1 << 5) | (1 << 3) | (1 << 2);
	
	switch (number)
	{
		case 1:
			set_pin_mode(GPIOA, 9, ALTERNATE);
			set_pin_mode(GPIOA, 10, INPUT_PULLUP);
			break;
		case 2:
			set_pin_mode(GPIOA, 2, ALTERNATE);
			set_pin_mode(GPIOA, 3, INPUT_PULLUP);
			break;
		case 3:
			set_pin_mode(GPIOB, 10, ALTERNATE);
			set_pin_mode(GPIOB, 11, INPUT_PULLUP);
			break;
	}
	
	base->CR1 |= (1 << 13);

	switch (number)
	{
		case 1:
			NVIC_EnableIRQ(USART1_IRQn);
			break;
		case 2:
			NVIC_EnableIRQ(USART2_IRQn);
			break;
		case 3:
			NVIC_EnableIRQ(USART3_IRQn);
			break;
	}
}

void uart_send_byte(int number, int data)
{
	USART_TypeDef *base = uart_base(number);
	base->SR &= ~(1u << 6);
	base->DR = data;
	while ((base->SR & (1u << 6)) == 0);
}

void uart_send_str(int number, const char *str)
{
	for (; *str; str++)
	{
		uart_send_byte(number, *str);
	}
}

int uart_recv_byte(int number)
{
	int res = -1;
	uart_irq_control(number, 0);
	if (number == 1)
	{
		if (uart1_n > 0)
		{
			uart1_n--;
			res = uart1buf[uart1_rp++];
			uart1_rp %= sizeof(uart1buf);
		}
	}
	else if (number == 2)
	{
		if (uart2_n > 0)
		{
			uart2_n--;
			res = uart2buf[uart2_rp++];
			uart2_rp %= sizeof(uart2buf);
		}
	}
	else
	{
		if (uart3_n > 0)
		{
			uart3_n--;
			res = uart3buf[uart3_rp++];
			uart3_rp %= sizeof(uart3buf);
		}
	}
	uart_irq_control(number, 1);
	return res;
}

int uart_recv(int number, void *buffer, int count)
{
	int i, ch;
	unsigned char *b = (unsigned char *)buffer;

	for (i = 0; i < count; i++)
	{
		ch = uart_recv_byte(number);
		if (ch < 0)
		{
			return i;
		}
		*b++ = ch;
	}

	return count;
}

void USART1_IRQHandler(void)
{
	volatile unsigned char dr;
	if (USART1->SR & 0x3F)
	{
		dr = USART1->DR;
		if (uart1_n >= sizeof(uart1buf))
		{
			uart1_n--;
			uart1_rp++;
			uart1_rp %= sizeof(uart1buf);
		}
		uart1_n++;
		uart1buf[uart1_wp++] = dr;
		uart1_wp %= sizeof(uart1buf);
	}
}

void USART2_IRQHandler(void)
{
	volatile unsigned char dr;
	if (USART2->SR & 0x3F)
	{
		dr = USART2->DR;
		if (uart2_n >= sizeof(uart2buf))
		{
			uart2_n--;
			uart2_rp++;
			uart2_rp %= sizeof(uart2buf);
		}
		uart2_n++;
		uart2buf[uart2_wp++] = dr;
		uart2_wp %= sizeof(uart2buf);
	}
}

void USART3_IRQHandler(void)
{
	volatile unsigned char dr;
	if (USART3->SR & 0x3F)
	{
		dr = USART3->DR;
		if (uart3_n >= sizeof(uart3buf))
		{
			uart3_n--;
			uart3_rp++;
			uart3_rp %= sizeof(uart3buf);
		}
		uart3_n++;
		uart3buf[uart3_wp++] = dr;
		uart3_wp %= sizeof(uart3buf);
	}
}

SPI_TypeDef *spi_base(int number)
{
	if (number == 1)
		return SPI1;
	return SPI2;
}

void spi_open(int number, int clkdiv)
{
	SPI_TypeDef *base = spi_base(number);
	
	switch (number)
	{
		case 1:
			set_pin_mode(GPIOA, 5, ALTERNATE);
			set_pin_mode(GPIOA, 6, ALTERNATE);
			set_pin_mode(GPIOA, 7, ALTERNATE);
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			set_pin_mode(GPIOB, 13, ALTERNATE);
			set_pin_mode(GPIOB, 14, ALTERNATE);
			set_pin_mode(GPIOB, 15, ALTERNATE);
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
			break;
	}
	base->CR1 = 0;
	base->CR1 = (1 << 9) | (1 << 8) | ((clkdiv & 7) << 3) | (1 << 2);
	base->CR1 |= (1 << 6);
}

int spi_read_write(int number, int data)
{
	SPI_TypeDef *base = spi_base(number);
	
	base->DR = data;
	while (base->SR & 0x80);
	
	return base->DR;
}

void board_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	
	PWR->CR |= PWR_CR_DBP;
	
	// Disable JTAG
	AFIO->MAPR &= ~(7 << 24ul);
	AFIO->MAPR |= (2 << 24ul);
}

void sendchar(int ch)
{
	uart_send_byte(1, ch);
}
