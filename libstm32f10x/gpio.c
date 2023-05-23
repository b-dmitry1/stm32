#include "stm32f10x.h"
#include "board.h"

static GPIO_TypeDef *gpio(int port)
{
	return (GPIO_TypeDef *)(((unsigned long)GPIOA) + port * (((unsigned long)GPIOB) - ((unsigned long)GPIOA)));
}

void pin_conf(int port, int pin, int func, int mode, int type, int pull, int speed)
{
	GPIO_TypeDef *base = gpio(port);

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN << port;

	if (pin < 8)
		base->CRL &= ~(0x0F << (pin * 4ul));
	else
		base->CRH &= ~(0x0F << ((pin - 8) * 4ul));
		
	switch (mode)
	{
		case PIN_MODE_INPUT:
			switch (pull)
			{
				case PIN_PULL_FLOAT:
					if (pin < 8)
						base->CRL |= (4 << (pin * 4ul));
					else
						base->CRH |= (4 << ((pin - 8) * 4ul));
					break;
				case PIN_PULL_UP:
					if (pin < 8)
						base->CRL |= (8 << (pin * 4ul));
					else
						base->CRH |= (8 << ((pin - 8) * 4ul));
					base->BSRR = (1u << pin);
					break;
				case PIN_PULL_DOWN:
					if (pin < 8)
						base->CRL |= (8 << (pin * 4ul));
					else
						base->CRH |= (8 << ((pin - 8) * 4ul));
					base->BSRR = (1u << (pin + 16));
					break;
			}
			break;
		case PIN_MODE_OUTPUT:
			switch (type)
			{
				case PIN_TYPE_PUSHPULL:
					if (pin < 8)
						base->CRL |= (3 << (pin * 4ul));
					else
						base->CRH |= (3 << ((pin - 8) * 4ul));
					break;
				case PIN_TYPE_OPENDRAIN:
					if (pin < 8)
						base->CRL |= (7 << (pin * 4ul));
					else
						base->CRH |= (7 << ((pin - 8) * 4ul));
					break;
			}
			break;
		case PIN_MODE_AF:
			if (pin < 8)
				base->CRL |= (11 << (pin * 4ul));
			else
				base->CRH |= (11 << ((pin - 8) * 4ul));
			break;
		case PIN_MODE_ANALOG:
			break;
	}
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
	if (pin < 8)
		g->CRL = (g->CRL & ~(0x0F << (pin * 4ul))) | (3 << (pin * 4ul));
	else
		g->CRH = (g->CRH & ~(0x0F << ((pin - 8) * 4ul))) | (3 << ((pin - 8) * 4ul));
}

void pin_dir_input(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	if (pin < 8)
		g->CRL = (g->CRL & ~(0x0F << (pin * 4ul))) | (8 << (pin * 4ul));
	else
		g->CRH = (g->CRH & ~(0x0F << ((pin - 8) * 4ul))) | (8 << ((pin - 8) * 4ul));
}

void pin_select_alternate_fn(int port, int pin, int func)
{
	GPIO_TypeDef *g = gpio(port);
	if (pin < 8)
		g->CRL = (g->CRL & ~(0x0F << (pin * 4ul))) | (11 << (pin * 4ul));
	else
		g->CRH = (g->CRH & ~(0x0F << ((pin - 8) * 4ul))) | (11 << ((pin - 8) * 4ul));
}

void pin_high(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	g->BSRR = (1u << pin);
}

void pin_low(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	g->BSRR = ((1u << 16) << pin);
}

void pin_set(int port, int pin, int value)
{
	GPIO_TypeDef *g = gpio(port);
	if (value)
		g->BSRR = (1u << pin);
	else
		g->BSRR = ((1u << 16) << pin);
}

int pin_get(int port, int pin)
{
	GPIO_TypeDef *g = gpio(port);
	return (g->IDR & (1u << pin)) ? 1 : 0;
}
