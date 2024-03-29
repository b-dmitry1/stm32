#include "stm32f10x.h"
#include "board.h"

#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

unsigned int get_flash_size(void)
{
	const unsigned short *flash_size = (const unsigned short *)0x1FFFF7E0u;

	return *flash_size * 1024u;
}

void flash_unlock(void)
{
	FLASH->KEYR  = FLASH_KEY1;
	FLASH->KEYR  = FLASH_KEY2;
}

void flash_lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

static void flash_erase_page(unsigned int ofs)
{
	flash_unlock();

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = FLASH_START + ofs;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR  &= ~FLASH_CR_PER;

	flash_lock();
}

void flash_erase_user(unsigned int offset, unsigned int size)
{
	unsigned int ofs;
	const unsigned int *user = (const unsigned int *)(FLASH_START + offset);

	// We don't know page size but we can erase it from the middle
	// So let's check the range and erase all the dirty bytes
	for (ofs = offset; ofs < offset + size; ofs += 4)
	{
		if (*user++ != 0xFFFFFFFFu)
		{
			flash_erase_page(ofs);
		}
	}
}

int flash_write(unsigned int offset, const void *data, unsigned int size)
{
	const unsigned short *src = (const unsigned short *)data;
	unsigned short *dst = (unsigned short *)(FLASH_START + offset);
	unsigned int i, count;
	int same = 1, clean = 1;

	for (i = 0; i < size / 2; i++)
	{
		if (dst[i] != 0xFFFF)
		{
			clean = 0;
		}

		if (dst[i] != src[i])
		{
			same = 0;
			break;
		}
	}

	if (same)
	{
		return 1;
	}

	if (!clean)
	{
		return 0;
	}

	flash_unlock();
	
	for (count = 0; count < size; count += 2)
	{
		FLASH->CR |= FLASH_CR_PG;

		*dst++ = *src++;
		while (FLASH->SR & FLASH_SR_BSY);

		FLASH->CR &= ~FLASH_CR_PG;
		if (FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR))
		{
			FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;

			flash_lock();

			return 0;
		}
	}

	flash_lock();

	return 1;
}

void flash_read(unsigned int offset, void *data, unsigned int size)
{
	unsigned int i;
	const unsigned char *src = (const unsigned char *)(FLASH_START + offset);
	unsigned char *dst = (unsigned char *)data;

	for (i = 0; i < size; i++)
		*dst++ = *src++;
}
