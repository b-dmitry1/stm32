#include "stm32f10x.h"
#include "board.h"

static void (*timer_callbacks[10])(void) = { 0 };

static TIM_TypeDef *tim_regs(int number)
{
	switch (number)
	{
		case 2: if (RCC->APB1ENR & RCC_APB1ENR_TIM2EN) return TIM2; break;
		case 3: if (RCC->APB1ENR & RCC_APB1ENR_TIM3EN) return TIM3; break;
		case 4: if (RCC->APB1ENR & RCC_APB1ENR_TIM4EN) return TIM4; break;
	}
	return (TIM_TypeDef *)0;
}

static void timer_irq(int number)
{
	if (timer_callbacks[number])
		timer_callbacks[number]();
}

void TIM2_IRQHandler(void)
{
	timer_irq(2);
	TIM2->SR = 0;
}

void TIM3_IRQHandler(void)
{
	timer_irq(3);
	TIM3->SR = 0;
}

void TIM4_IRQHandler(void)
{
	timer_irq(4);
	TIM4->SR = 0;
}

void timer_enable_irq(int number)
{
	switch (number)
	{
		case 2: NVIC_EnableIRQ(TIM2_IRQn); break;
		case 3: NVIC_EnableIRQ(TIM3_IRQn); break;
		case 4: NVIC_EnableIRQ(TIM4_IRQn); break;
	}
}

void timer_disable_irq(int number)
{
	switch (number)
	{
		case 2: NVIC_DisableIRQ(TIM2_IRQn); break;
		case 3: NVIC_DisableIRQ(TIM3_IRQn); break;
		case 4: NVIC_DisableIRQ(TIM4_IRQn); break;
	}
}

void timer_power_on(int number)
{
	switch (number)
	{
		case 2: RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; break;
		case 3: RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; break;
		case 4: RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; break;
	}
}

void timer_power_off(int number)
{
	switch (number)
	{
		case 2: RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN; break;
		case 3: RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN; break;
		case 4: RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN; break;
	}
}

void timer_init_periodic_ns(int number, unsigned long long period_ns, void (*callback)(void))
{
	TIM_TypeDef *regs;
	unsigned int clk = (unsigned int)apb1clk;
	unsigned int v;
	unsigned int psc = 1;
	
	timer_disable_irq(number);

	timer_power_on(number);

	/* If APB1 prescaler != 1 then TIMCLK = clk x 2 */
	if (RCC->CFGR & (0x7 << 8))
		clk *= 2;

	regs = tim_regs(number);
	if (regs == (TIM_TypeDef *)0)
	{
		return;
	}

	/* Stop timer and set default values */
	regs->CR1 &= ~0x03FF;
	regs->CR2 &= ~0x00F8;
	regs->SMCR = 0x0000;
	regs->DIER &= ~0x5FAF;
	regs->CCMR1 = 0x0000;
	regs->CCMR2 = 0x0000;
	regs->CCER &= ~0x3333;

	/* Do not start the timer if period_ns == 0 */
	if (period_ns == 0)
	{
		return;
	}

	/* Timer increment is APB1CLK / (PSC + 1) */
	v = period_ns * clk / 1000000000ull;

	/* Calc prescaler by dividing v by 10000 rounding up */
        psc = (v + 9999) / 10000;

	/* Calc ARR value */
	v /= psc;

	/* Set callback */
	timer_callbacks[number] = callback;

	/* Program and enable timer */
	regs->PSC = psc - 1;
	regs->ARR = v;
	regs->DIER |= TIM_DIER_UIE;
	regs->CR1 |= TIM_CR1_CEN;

	if (callback != 0)
		timer_enable_irq(number);
}

void timer_disable(int number)
{
	TIM_TypeDef *regs;
	
	timer_disable_irq(number);

	regs = tim_regs(number);
	if (regs == (TIM_TypeDef *)0)
	{
		return;
	}

	regs->CR1 &= ~0x03FF;

	timer_power_off(number);
}
