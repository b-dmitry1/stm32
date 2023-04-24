#include <string.h>
#include "stm32f10x.h"
#include "board1xx.h"

void board_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	
	PWR->CR |= PWR_CR_DBP;
	
	// Disable JTAG
	AFIO->MAPR &= ~(7 << 24ul);
	AFIO->MAPR |= (2 << 24ul);

	memset((void *)uarts, 0, sizeof(uarts));
}
