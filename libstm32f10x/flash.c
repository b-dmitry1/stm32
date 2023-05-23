#include "stm32f10x.h"
#include "board.h"

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

void flash_erase_user_pages(unsigned int user_start_kb, unsigned int user_size_kb, unsigned int flash_page_size_kb)
{
	unsigned int i, ptr;
	unsigned int user_start = user_start_kb * 1024;
	unsigned int user_end = user_size_kb * 1024;
	const unsigned int *user;
	int erased;

	for (ptr = user_start; ptr < user_end; ptr += flash_page_size_kb * 1024)
	{
		user = (const unsigned int *)(FLASH_START + ptr);
		erased = 1;
		for (i = 0; i < flash_page_size_kb * 1024 / sizeof(user[0]); i++)
		{
			if (*user++ != 0xFFFFFFFFu)
			{
				erased = 0;
				break;
			}
		}

		if (erased)
		{
			continue;
		}

		flash_erase_page(ptr);
	}
}

void flash_erase_user(void)
{
	flash_erase_user_pages(USER_START / 1024, USER_SIZE / 1024, FLASH_PAGE_SIZE / 1024);
}

int flash_write(unsigned int ofs, const void *data, unsigned int size)
{
	const unsigned short *src = (const unsigned short *)data;
	unsigned short *dst = (unsigned short *)(FLASH_START + ofs);
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

void flash_read(unsigned int ofs, void *data, unsigned int size)
{
	unsigned int i;
	const unsigned char *src = (const unsigned char *)(FLASH_START + ofs);
	unsigned char *dst = (unsigned char *)data;

	for (i = 0; i < size; i++)
		*dst++ = *src++;
}
