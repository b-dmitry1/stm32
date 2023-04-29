#include <string.h>
#include "stm32f10x.h"
#include "board1xx.h"

int sysclk = 8000000;
int ahbclk = 8000000;
int apb1clk = 8000000;
int apb2clk = 8000000;

void rcc_use_hsi_as_system_clock(void)
{
	// Turn HSI on
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY));

	// Switch system clock to HSI (0)
	RCC->CFGR = (RCC->CFGR & ~0x3) | 0x0;
	while ((RCC->CFGR & 0xC) != 0x0);
}

static int rcc_use_hse_as_system_clock(void)
{
	int timeout;

	// Switch system clock to HSE (1)
	RCC->CFGR = (RCC->CFGR & ~0x3) | 0x1;
	for (timeout = 0; timeout < 100000 && (RCC->CFGR & 0xC) != 0x4; timeout++);
	return (RCC->CFGR & 0xC) == 0x4;
}

static int rcc_use_pll_as_system_clock(void)
{
	int timeout;

	RCC->CR |= RCC_CR_PLLON | RCC_CR_CSSON;
	// while (!(RCC->CR & RCC_CR_PLLRDY));

	// Switch system clock to PLL (2)
	RCC->CFGR = (RCC->CFGR & ~0x3) | 0x2;
	for (timeout = 0; timeout < 100000 && (RCC->CFGR & 0xC) != 0x8; timeout++);
	return (RCC->CFGR & 0xC) == 0x8;
}

static void rcc_set_flash_latency(void)
{
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	if (sysclk <= 24000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_0;
	else if (sysclk <= 48000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_1;
	else
		FLASH->ACR |= FLASH_ACR_LATENCY_2;
}

static void rcc_update_frequences(int hse_freq, int input_hse, int hse_div, int pll_mul, int ahb_div, int apb1_div, int apb2_div)
{
	sysclk = 8000000;

	if (input_hse)
	{
		sysclk = hse_freq;
		if (hse_div >= 1 && hse_div <= 16)
			sysclk /= hse_div;
	}
	else
	{
		if (pll_mul >= 4 && pll_mul <= 9)
			sysclk /= 2;
	}

	if (pll_mul >= 4 && pll_mul <= 9)
		sysclk *= pll_mul;

	ahbclk = sysclk;
	switch (ahb_div)
	{
		case 2:
		case 4:
		case 8:
		case 16:
		case 64:
		case 128:
		case 256:
		case 512:
			ahbclk /= ahb_div;
			break;
	}

	apb1clk = sysclk;
	switch (apb1_div)
	{
		case 2:
		case 4:
		case 8:
		case 16:
			apb1clk /= apb1_div;
			break;
	}

	apb2clk = sysclk;
	switch (apb2_div)
	{
		case 2:
		case 4:
		case 8:
		case 16:
			apb2clk /= apb2_div;
			break;
	}
}

int rcc_configure_pll(int hse_freq, int input_hse, int hse_div, int pll_mul, int ahb_div, int apb1_div, int apb2_div)
{
	rcc_use_hsi_as_system_clock();

	if (hse_div < 1)
		hse_div = 1;

#ifndef STM32F10X_CL
	if (hse_div > 2)
		hse_div = 2;
#endif

	// Turn off PLL
	RCC->CR &= ~(RCC_CR_PLLON);
	while (RCC->CR & RCC_CR_PLLRDY);

	// Turn off HSE
	RCC->CR &= ~RCC_CR_HSEON;
	while (RCC->CR & RCC_CR_HSERDY);

	// Set AHB prescaler
	RCC->CFGR = RCC->CFGR & ~(0xF << 4);
	switch (ahb_div)
	{
		case 2: RCC->CFGR |= 8 << 4; break;
		case 4: RCC->CFGR |= 9 << 4; break;
		case 8: RCC->CFGR |= 10 << 4; break;
		case 16: RCC->CFGR |= 11 << 4; break;
		case 64: RCC->CFGR |= 12 << 4; break;
		case 128: RCC->CFGR |= 13 << 4; break;
		case 256: RCC->CFGR |= 14 << 4; break;
		case 512: RCC->CFGR |= 15 << 4; break;
	}

	// Set APB1 prescaler
	RCC->CFGR = RCC->CFGR & ~(0x7 << 8);
	switch (apb1_div)
	{
		case 2: RCC->CFGR |= 4 << 8; break;
		case 4: RCC->CFGR |= 5 << 8; break;
		case 8: RCC->CFGR |= 6 << 8; break;
		case 16: RCC->CFGR |= 7 << 8; break;
	}
	
	// Set APB2 prescaler
	RCC->CFGR = RCC->CFGR & ~(0x7 << 11);
	switch (apb2_div)
	{
		case 2: RCC->CFGR |= 4 << 11; break;
		case 4: RCC->CFGR |= 5 << 11; break;
		case 8: RCC->CFGR |= 6 << 11; break;
		case 16: RCC->CFGR |= 7 << 11; break;
	}

#ifdef STM32F10X_CL
	if (hse_div >= 1 && hse_div <= 16)
	{
		// Set HSE's PREDIV value
		RCC->CFGR2 = (RCC->CFGR2 & ~0xFu) | (hse_div - 1);
	}
#else
	if (hse_div == 2)
	{
		RCC->CFGR |= RCC_CFGR_PLLXTPRE;
	}
	else
	{
		RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
	}
#endif
	
	// Set PLL multiplier
	RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (2 << 18);
	switch (pll_mul)
	{
		case 0: break;
		case 1: break;
		case 4: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (2 << 18); break;
		case 5: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (3 << 18); break;
		case 6: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (4 << 18); break;
		case 7: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (5 << 18); break;
		case 8: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (6 << 18); break;
		case 9: RCC->CFGR = (RCC->CFGR & ~(0xF << 18u)) | (7 << 18); break;
		default: return 0;
	}

	if (input_hse)
	{
		// Turn on HSE
		RCC->CR |= RCC_CR_HSEON;
		while (!(RCC->CR & RCC_CR_HSERDY));
	}

	// Select PLL source (HSI or HSE PREDIV)
	if (input_hse)
	{
		RCC->CFGR |= RCC_CFGR_PLLSRC;
	}
	else
	{
		RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	}

	// Final steps
	rcc_update_frequences(hse_freq, input_hse, hse_div, pll_mul, ahb_div, apb1_div, apb2_div);

	rcc_set_flash_latency();
	
	// Select system clock source
	if (input_hse)
	{
		// HSE or PLL
		if (pll_mul <= 1)
			rcc_use_hse_as_system_clock();
		else
			rcc_use_pll_as_system_clock();
	}
	else
	{
		// HSI or PLL
		if (pll_mul <= 1)
			rcc_use_hsi_as_system_clock();
		else
			rcc_use_pll_as_system_clock();
	}

	return sysclk == input_hse ? hse_freq / hse_div * pll_mul : pll_mul >= 4 ? 4000000 * pll_mul : 8000000;
}

void rcc_high_performance(int hse_freq)
{
	int mul, div = 1, best_freq = 0, best_mul = 1, best_div = 1;

	if (hse_freq < 3000000 || hse_freq > 25000000)
	{
		rcc_configure_pll(0, 0, 1, 9, 1, 1, 1);
	}
	else
	{
		// Find best PLL's div and mul for this XTAL
#ifdef STM32F10X_CL
		for (div = 1; div <= 4; div++)
#else
		for (div = 1; div <= 2; div++)
#endif
		{
			for (mul = 9; mul >= 4; mul--)
			{
				if (hse_freq / div * mul <= 72000000)
				{
					if (hse_freq / div * mul > best_freq)
					{
						best_freq = hse_freq / div * mul;
						best_mul = mul;
						best_div = div;
					}
				}
			}
		}
		rcc_configure_pll(hse_freq, 1, best_div, best_mul, 1, best_freq > 36000000 ? 2 : 1, 1);
	}
}
