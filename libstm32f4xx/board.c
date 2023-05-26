#include <string.h>
#include "stm32f4xx.h"
#include "board.h"

void board_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_CCMDATARAMEN;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Prefetcher and caches
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	
	// Compensation Cell
	SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
	while (!(SYSCFG->CMPCR & SYSCFG_CMPCR_READY));
	
	memset((void *)uarts, 0, sizeof(uarts));
}
