#pragma once

void fm25_init();
void fm25_read(unsigned short addr, void *buffer, unsigned short count);
void fm25_write(unsigned short addr, const void *buffer, unsigned short count);
