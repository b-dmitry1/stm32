#include "stm32f4xx.h"
#include "board.h"

static GPIO_TypeDef *gpio(int port)
{
	return (GPIO_TypeDef *)(((unsigned long)GPIOA) + port * (((unsigned long)GPIOB) - ((unsigned long)GPIOA)));
}

void pin_conf(int port, int pin, int func, int mode, int type, int pull, int speed)
{
	GPIO_TypeDef *g = gpio(port);
	RCC->AHB1ENR |= 1 << port;
	if (pin < 8)
		g->AFR[0] = (g->AFR[0] & (~(15ul << (pin * 4)))) | (func << (pin * 4ul));
	else
		g->AFR[1] = (g->AFR[1] & (~(15ul << ((pin - 8) * 4)))) | (func << ((pin - 8) * 4ul));
	g->OTYPER = (g->OTYPER & (~(1ul << pin))) | (type << pin);
	g->OSPEEDR = (g->PUPDR & (~(3ul << (pin * 2)))) | (speed << (pin * 2));
	g->MODER = (g->MODER & (~(3ul << (pin * 2)))) | (mode << (pin * 2));
	g->PUPDR = (g->PUPDR & (~(3ul << (pin * 2)))) | (pull << (pin * 2));
}

void pin_conf_port(int port, int pinmask, int func, int mode, int type, int pull, int speed)
{
	int pin;
	for (pin = 0; pin < 16; pin++)
		if (pinmask & (1 << pin))
			pin_conf(port, pin, func, mode, type, pull, speed);
}

void pin_conf_output(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_OUTPUT, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_HIGH);
}

void pin_conf_opendrain(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_OUTPUT, PIN_TYPE_OPENDRAIN, PIN_PULL_FLOAT, PIN_SPEED_HIGH);
}

void pin_conf_input_pullup(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_LOW);
}

void pin_conf_input_float(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_LOW);
}

void pin_conf_input_pulldown(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_INPUT, PIN_TYPE_PUSHPULL, PIN_PULL_DOWN, PIN_SPEED_LOW);
}

void pin_conf_analog(int port, int pin)
{
	pin_conf(port, pin, 0, PIN_MODE_ANALOG, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_LOW);
}

void pin_dir_output(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	g->MODER = (g->MODER & (~(3ul << (pin * 2)))) | (PIN_MODE_OUTPUT << (pin * 2));
}

void pin_dir_input(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	g->MODER = (g->MODER & (~(3ul << (pin * 2)))) | (PIN_MODE_INPUT << (pin * 2));
}

void pin_select_alternate_fn(int port, int pin, int func)
{
	GPIO_TypeDef *g = gpio(port);
	if (pin < 8)
		g->AFR[0] = (g->AFR[0] & (~(15ul << (pin * 4)))) | (func << (pin * 4ul));
	else
		g->AFR[1] = (g->AFR[1] & (~(15ul << ((pin - 8) * 4)))) | (func << ((pin - 8) * 4ul));
	g->MODER = (g->MODER & (~(3ul << (pin * 2)))) | (PIN_MODE_AF << (pin * 2));
}

void pin_high(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	volatile unsigned long *bsrr = (volatile unsigned long *)&g->BSRRL;
	*bsrr = 1 << pin;
}

void pin_low(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	volatile unsigned long *bsrr = (volatile unsigned long *)&g->BSRRL;
	*bsrr = 1ul << (pin + 16);
}

void pin_set(int port, int pin, int value)
{
	GPIO_TypeDef *g = gpio(port);
	volatile unsigned long *bsrr = (volatile unsigned long *)&g->BSRRL;
	if (value)
		*bsrr = 1 << pin;
	else
		*bsrr = 1ul << (pin + 16);
}

int pin_get(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	return (g->IDR & (1 << pin)) != 0;
}
