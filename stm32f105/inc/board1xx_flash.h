#ifndef BOARD1XX_FLASH_H
#define BOARD1XX_FLASH_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

void flash_unlock(void);
void flash_lock(void);
void flash_erase_page(unsigned long ofs);
int flash_write_page(unsigned long ofs, const void *data);
void flash_read(unsigned long ofs, void *data, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif
