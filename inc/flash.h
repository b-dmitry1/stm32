#ifndef FLASH_H
#define FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

void flash_unlock(void);
void flash_lock(void);
void flash_erase_user_pages(unsigned int user_start_kb, unsigned int user_size_kb, unsigned int flash_page_size_kb);
void flash_erase_user(void);
int flash_write(unsigned int ofs, const void *data, unsigned int size);
void flash_read(unsigned int ofs, void *data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
