#include "stm32f4xx.h"
#include "board.h"

#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

unsigned int get_flash_size(void)
{
	const unsigned short *flash_size = (const unsigned short *)0x1FFF7A22u;

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

static void flash_erase_sector(unsigned int ofs)
{
	int sector = 0;

	if      (ofs >= 0xE0000) sector = 11;
	else if (ofs >= 0xC0000) sector = 10;
	else if (ofs >= 0xA0000) sector = 9;
	else if (ofs >= 0x80000) sector = 8;
	else if (ofs >= 0x60000) sector = 7;
	else if (ofs >= 0x40000) sector = 6;
	else if (ofs >= 0x20000) sector = 5;
	else if (ofs >= 0x10000) sector = 4;
	else if (ofs >= 0x0C000) sector = 3;
	else if (ofs >= 0x08000) sector = 2;
	else if (ofs >= 0x04000) sector = 1;

	flash_unlock();

	// x32 programming and sector number
	FLASH->CR = (2 << 8) | (sector << 3);
	// Sector erase
	FLASH->CR |= FLASH_CR_SER;
	// Start
	FLASH->CR |= FLASH_CR_STRT;
	// Wait
	while (FLASH->SR & FLASH_SR_BSY);
	// Done
	FLASH->CR = (2 << 8);

	flash_lock();
}

void flash_erase_user(unsigned int offset, unsigned int size)
{
	unsigned int ofs;
	const unsigned int *user = (const unsigned int *)(FLASH_START + offset);

	// Check the range and erase all the dirty bytes
	for (ofs = offset; ofs < offset + size; ofs += 4)
	{
		if (*user++ != 0xFFFFFFFFu)
		{
			flash_erase_sector(ofs);
		}
	}
}

int flash_write(unsigned int offset, const void *data, unsigned int size)
{
	const unsigned int *src = (const unsigned int *)data;
	unsigned int *dst = (unsigned int *)(FLASH_START + offset);
	unsigned int i, count;
	int same = 1, clean = 1;

	for (i = 0; i < size / 4; i++)
	{
		if (dst[i] != 0xFFFFFFFFu)
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

	// x32 programming
	FLASH->CR = (2 << 8);

	// Start write
	FLASH->CR |= FLASH_CR_PG;
	for (count = 0; count < size; count += 4)
	{
		*dst++ = *src++;
		while (FLASH->SR & FLASH_SR_BSY);
	}
	FLASH->CR &= ~FLASH_CR_PG;

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
