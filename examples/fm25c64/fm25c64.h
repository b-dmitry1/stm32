#pragma once

void fm25_init(spi_t *spi, int spi_port, int mosi_port, int mosi_pin, int miso_port, int miso_pin, int sck_port, int sck_pin, int cs_port, int cs_pin);
void fm25_read(spi_t *spi, unsigned short addr, void *buffer, unsigned short count);
void fm25_write(spi_t *spi, unsigned short addr, const void *buffer, unsigned short count);
