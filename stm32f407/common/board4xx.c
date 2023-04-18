#include <string.h>
#include "stm32f4xx.h"
#include "board4xx.h"

void board_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_CCMDATARAMEN;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// SYSCFG
	RCC->APB2ENR |= (1 << 14);
	
	// Compensation Cell
	SYSCFG->CMPCR |= 0x01;
	while (!(SYSCFG->CMPCR & 0x100));
	
	memset((void *)uarts, 0, sizeof(uarts));
}
