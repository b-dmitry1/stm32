#ifndef FLASH_H
#define FLASH_H

#define FLASH_START		0x8000000u

#ifdef __cplusplus
extern "C" {
#endif

unsigned int get_flash_size(void);
void flash_unlock(void);
void flash_lock(void);
void flash_erase_user(unsigned int offset, unsigned int size);
int flash_write(unsigned int offset, const void *data, unsigned int size);
void flash_read(unsigned int offset, void *data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
