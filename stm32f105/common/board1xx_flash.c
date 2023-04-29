#include "stm32f10x.h"
#include "board1xx.h"

#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

void flash_unlock(void)
{
	FLASH->KEYR  = FLASH_KEY1;
	FLASH->KEYR  = FLASH_KEY2;
}

void flash_lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void flash_erase_page(unsigned long ofs)
{
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = FLASH_START + ofs;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR  &= ~FLASH_CR_PER;
}

int flash_write_page(unsigned long ofs, const void *data)
{
	const unsigned short *src = (const unsigned short *)data;
	unsigned short *dst = (unsigned short *)(FLASH_START + ofs);
	unsigned int count;
	
	for (count = 0; count < FLASH_PAGE_SIZE; count += 2)
	{
		FLASH->CR |= FLASH_CR_PG;

		*dst++ = *src++;
		while (FLASH->SR & FLASH_SR_BSY);

		FLASH->CR &= ~FLASH_CR_PG;
		if (FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR))
		{
			FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
			return 0;
		}
	}
	return 1;
}

void flash_read(unsigned long ofs, void *data, unsigned int count)
{
	unsigned int i;
	const unsigned char *src = (const unsigned char *)(FLASH_START + ofs);
	unsigned char *dst = (unsigned char *)data;

	for (i = 0; i < count; i++)
		*dst++ = *src++;
}
