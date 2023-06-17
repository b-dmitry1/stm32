#include <stdio.h>
#include <string.h>
#include "board.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

unsigned char *ram = (unsigned char *)0xC0000000u;

void sdram_configure_port(int port, int mask, int af, int type, int pull, int speed)
{
	int pin;

	for (pin = 0; pin < 16; pin++)
	{
		if (mask & (1 << pin))
		{
			pin_conf(port, pin, af, PIN_MODE_AF, type, pull, speed);
		}
	}
}

void configure_gpios(void)
{
	// WE, NE0, CKE0
	sdram_configure_port(PORTC, (1 << 3) | (1 << 2) | (1 << 0), 12, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_VERYHIGH);

	// D0 - D3, D13 - D15
	sdram_configure_port(PORTD, (1 << 15) | (1 << 14) | (1 << 10) | (1 << 9) | (1 << 8) | (1 << 1) | (1 << 0), 12, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_VERYHIGH);

	// NBL0 - NBL1, D4 - D12
	sdram_configure_port(PORTE, (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | (1 << 7) | (1 << 1) | (1 << 0), 12, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_VERYHIGH);

	// A0 - A9, RAS
	sdram_configure_port(PORTF, (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0), 12, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_VERYHIGH);
	
	// A10 - A12, BA0, BA1, CLK, CAS
	sdram_configure_port(PORTG, (1 << 15) | (1 << 8) | (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1) | (1 << 0), 12, PIN_TYPE_PUSHPULL, PIN_PULL_UP, PIN_SPEED_VERYHIGH);
}

void set_color(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char b1 = b;
	
	b &= 0xC0;
	
	if (r & 0x20)
		b |= 1 << 2;
	if (g & 0x20)
		b |= 1 << 3;
	if (b1 & 0x20)
		b |= 1 << 1;
	
	LTDC_Layer1->CLUTWR = (index << 24ul) | (r << 16ul) | (g << 8ul) | b;
}

void set_palette(void)
{
	int i;
	unsigned char r, g, b;

	set_color(0, 0, 0, 0);
	set_color(1, 0, 0, 128);
	set_color(2, 0, 96, 0);
	set_color(3, 0, 160, 96);
	set_color(4, 128, 0, 0);
	set_color(5, 128, 0, 160);
	set_color(6, 160, 96, 0);
	set_color(7, 160, 160, 160);
	set_color(8, 80, 80, 80);
	set_color(9, 0, 0, 255);
	set_color(10, 0, 255, 0);
	set_color(11, 0, 255, 255);
	set_color(12, 255, 0, 0);
	set_color(13, 255, 0, 255);
	set_color(14, 255, 255, 0);
	set_color(15, 255, 255, 255);

	for (i=16,r=0;r<6;r++)
		for (g=0;g<6;g++)
			for (b=0;b<6;b++)
				set_color(i++, r * 51, g * 51, b * 51);

	for (b=0;b<32;b++)
		set_color(224 + b, b * 8, b * 8, b * 8);
}

void draw_pixel(int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return;

	ram[x + y * SCREEN_WIDTH] = color;
}

void draw_rect(int x, int y, int w, int h, int color)
{
	int i, j;

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			draw_pixel(x + j, y + i, color);
}

int main(void)
{
	int x, y;

	rcc_high_performance(XTAL_16MHZ);

	configure_gpios();

	// CAS = 2, READ BURST, 16 bit, 4 banks, 13 rows, 9 cols
	sdram_init(2, 1, 16, 4, 13, 9);

	pin_conf(PORTA, 8, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // R6
	pin_conf(PORTG, 6, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // R7

	pin_conf(PORTC, 7, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // G6
	pin_conf(PORTD, 3, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // G7

	pin_conf(PORTG, 10, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // B2 / R5
	pin_conf(PORTG, 11, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // B3 / G5
	pin_conf(PORTG, 12, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // B1 / B5
	
	pin_conf(PORTB, 8, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // B6
	pin_conf(PORTB, 9, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // B7
	
	pin_conf(PORTC, 6, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // HSYNC
	pin_conf(PORTA, 4, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // VSYNC
	pin_conf(PORTF, 10, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // DE
	pin_conf(PORTG, 7, 14, PIN_MODE_AF, PIN_TYPE_PUSHPULL, PIN_PULL_FLOAT, PIN_SPEED_VERYHIGH); // CLK

	ltdc_init(&vga640x480x8, (unsigned int)ram);

	set_palette();

	for (y = 0; y < 16; y++)
		for (x = 0; x < 16; x++)
			draw_rect(x * 20, y * 20, 19, 19, x + y * 16);

	for (;;);
}
