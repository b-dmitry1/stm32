#include <string.h>
#include "stm32f4xx.h"
#include "board.h"

#define HSI_FREQ	16000000

#define M_MIN		2
#define M_MAX		63

#define N_MIN		192
#define N_MAX		432

#define P_MIN		2
#define P_MAX		8

#define Q_MIN		2
#define Q_MAX		15

#define N_SAI_MIN	49
#define N_SAI_MAX	432

#define R_SAI_MIN	2
#define R_SAI_MAX	7

#define Q_SAI_MIN	2
#define Q_SAI_MAX	15

#ifdef STM32F401xx
#define SYSCLK_MAX	84000000
#define AHB_MAX		84000000
#else
#define SYSCLK_MAX	168000000
#define AHB_MAX		168000000
#endif

#define APB1_MAX	42000000
#define APB2_MAX	84000000

#define VCO_IN_MIN	1000000
#define VCO_IN_MAX	2000000

#define VCO_MIN		192000000
#define VCO_MAX		432000000

#define HSE_MIN		4000000
#define HSE_MAX		26000000

#define TARGET_FREQ	SYSCLK_MAX

int sysclk = HSI_FREQ;
int ahbclk = HSI_FREQ;
int apb1clk = HSI_FREQ;
int apb2clk = HSI_FREQ;

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
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Switch system clock to PLL (2)
	RCC->CFGR = (RCC->CFGR & ~0x3) | 0x2;
	for (timeout = 0; timeout < 100000 && (RCC->CFGR & 0xC) != 0x8; timeout++);
	return (RCC->CFGR & 0xC) == 0x8;
}

static void rcc_set_flash_latency(void)
{
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	// Settings for 2.7 - 3.3 V
	// Use 24 MHz step for 2.4 V
	// 22 MHz step for 2.1 V
	// 20 MHz step for 1.8 V MCU power supply
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	if (sysclk <= 30000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
	else if (sysclk <= 60000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_1WS;
	else if (sysclk <= 90000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	else if (sysclk <= 120000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	else if (sysclk <= 150000000)
		FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
	else
		FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
}

static void rcc_update_frequences(int hse_freq, int input_hse, int m, int n, int p, int q, int ahb_div, int apb1_div, int apb2_div)
{
	sysclk = 16000000;

	if (input_hse)
	{
		sysclk = hse_freq;
	}

	sysclk = sysclk * (long long)n / m / p;

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

	apb1clk = ahbclk;
	switch (apb1_div)
	{
		case 2:
		case 4:
		case 8:
		case 16:
			apb1clk /= apb1_div;
			break;
	}

	apb2clk = ahbclk;
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

#include <stdio.h>

int rcc_configure_pllsai(int n, int r, int q)
{
	// Turn off PLL
	RCC->CR &= ~(RCC_CR_PLLSAION);
	while (RCC->CR & RCC_CR_PLLSAIRDY);

	// Set PLL R
	RCC->PLLSAICFGR &= ~(0x7 << 28u);
	if (r < R_SAI_MIN || r > R_SAI_MAX)
	{
		r = R_SAI_MAX;
	}
	RCC->PLLSAICFGR |= r << 28u;

	// Set PLL Q
	RCC->PLLSAICFGR &= ~(0xF << 24u);
	if (q < Q_SAI_MIN || q > Q_SAI_MAX)
	{
		q = Q_SAI_MAX;
	}
	RCC->PLLSAICFGR |= q << 24u;

	// Set PLL N
	RCC->PLLSAICFGR &= ~(0x1FF << 6u);
	if (n < N_SAI_MIN || n > N_SAI_MAX)
	{
		n = N_SAI_MIN;
	}
	RCC->PLLSAICFGR |= n << 6u;

	RCC->CR |= RCC_CR_PLLSAION;
	while (!(RCC->CR & RCC_CR_PLLSAIRDY));

	return 1;
}

int rcc_configure_pll(int hse_freq, int input_hse, int m, int n, int p, int q, int ahb_div, int apb1_div, int apb2_div)
{
        int enable_pll = m > 0;

/*	
	printf("hse_freq = %d\ninput_hse = %d\nm = %d\nn = %d\np = %d\nq = %d\nahb_div = %d\napb1_div = %d\napb2_div = %d\n",
		hse_freq, input_hse, m, n, p, q, ahb_div, apb1_div, apb2_div);

	return 1;
*/

	rcc_use_hsi_as_system_clock();

	// Turn off PLL
	RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_PLLSAION);
	while (RCC->CR & (RCC_CR_PLLRDY | RCC_CR_PLLSAIRDY));

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
	RCC->CFGR = RCC->CFGR & ~(0x7 << 10);
	switch (apb1_div)
	{
		case 2: RCC->CFGR |= 4 << 10; break;
		case 4: RCC->CFGR |= 5 << 10; break;
		case 8: RCC->CFGR |= 6 << 10; break;
		case 16: RCC->CFGR |= 7 << 10; break;
	}
	
	// Set APB2 prescaler
	RCC->CFGR = RCC->CFGR & ~(0x7 << 13);
	switch (apb2_div)
	{
		case 2: RCC->CFGR |= 4 << 13; break;
		case 4: RCC->CFGR |= 5 << 13; break;
		case 8: RCC->CFGR |= 6 << 13; break;
		case 16: RCC->CFGR |= 7 << 13; break;
	}

	// Set PLL Q
	RCC->PLLCFGR &= ~(0xF << 24u);
	if (q < Q_MIN || q > Q_MAX)
	{
		q = Q_MAX;
	}
	RCC->PLLCFGR |= q << 24u;

	// Set PLL P
	RCC->PLLCFGR &= ~(0x3 << 16u);
	switch (p)
	{
		case 2: RCC->PLLCFGR |= 0 << 16u; break;
		case 4: RCC->PLLCFGR |= 1 << 16u; break;
		case 6: RCC->PLLCFGR |= 2 << 16u; break;
		default: RCC->PLLCFGR |= 3 << 16u; break;
	}

	// Set PLL N
	RCC->PLLCFGR &= ~(0x1FF << 6u);
	if (n < N_MIN || n > N_MAX)
	{
		n = N_MIN;
	}
	RCC->PLLCFGR |= n << 6u;

	// Set PLL M
	RCC->PLLCFGR &= ~(0x3F << 0u);
	if (m < M_MIN || m > M_MAX)
	{
		m = M_MAX;
	}
	RCC->PLLCFGR |= m << 0u;

	if (input_hse)
	{
		// Turn on HSE
		RCC->CR |= RCC_CR_HSEON;
		while (!(RCC->CR & RCC_CR_HSERDY));
	}

	// Select PLL source (HSI or HSE PREDIV)
	if (input_hse)
	{
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
	}
	else
	{
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	}

	// Final steps
	rcc_update_frequences(hse_freq, input_hse, m, n, p, q, ahb_div, apb1_div, apb2_div);

	rcc_set_flash_latency();
	
	// Select system clock source
	if (input_hse)
	{
		// HSE or PLL
		if (enable_pll)
			rcc_use_pll_as_system_clock();
		else
			rcc_use_hse_as_system_clock();
	}
	else
	{
		// HSI or PLL
		if (enable_pll)
			rcc_use_pll_as_system_clock();
		else
			rcc_use_hsi_as_system_clock();
	}

#ifndef STM32F401xx
	rcc_configure_pllsai(n, 7, 15);
#endif

	clock_changed();

	return 1;
}

void rcc_high_performance(int hse_freq)
{
	int m, n, p, in_freq, out_freq, freq, best_freq = 0, best_m = M_MAX, best_n = N_MIN, best_p = 8;
	int ahb_div, apb1_div, apb2_div;

	int target_freq = TARGET_FREQ;

	if (hse_freq < HSE_MIN || hse_freq > HSE_MAX)
	{
		// Out of range - use HSI and no PLL
		rcc_configure_pll(0, 0, 0, 0, 0, 0, 1, 1, 1);
	}
	else
	{
		// Find best prediv (M value)
		for (m = M_MIN; m <= M_MAX; m++)
		{
			in_freq = hse_freq / m;
			if (in_freq < VCO_IN_MIN || in_freq > VCO_IN_MAX)
			{
				// Should be between 1 and 2 MHz
				// Accoring to datasheet, 2 MHz is better
				continue;
			}

			// Find best multiplier (N value)
			for (n = N_MIN; n <= N_MAX; n++)
			{
				out_freq = in_freq * n;

				if (out_freq < VCO_MIN || out_freq > VCO_MAX)
				{
					// Should be between 192 and 432 MHz
					continue;
				}

				// Find best Main PLL output div (P value)
				for (p = P_MIN; p <= P_MAX; p += 2)
				{
					freq = out_freq / p;

					if (freq > SYSCLK_MAX)
					{
						// Should not exceed 168 MHz
						continue;
					}

					if (freq > best_freq && freq <= target_freq)
					{
						best_freq = freq;
						best_m = m;
						best_n = n;
						best_p = p;
					}
				}
			}
		}

		// Find best bus div values
		ahb_div = 1;

		freq = best_freq / ahb_div;

		for (apb1_div = 2; apb1_div <= 8; apb1_div *= 2)
		{
			if (freq / apb1_div <= APB1_MAX)
			{
				break;
			}
		}
		
		for (apb2_div = 1; apb2_div <= 8; apb2_div *= 2)
		{
			if (freq / apb2_div <= APB2_MAX)
			{
				break;
			}
		}

		rcc_configure_pll(hse_freq, 1, best_m, best_n, best_p, 15, ahb_div, apb1_div, apb2_div);
	}
}
