#include "stm32f4xx.h"
#include "board.h"

#ifdef STM32F429_439xx

const videomode_t vga640x480x8 =
{
	96, 48, 640, 16,
	2, 33, 480, 10,
	VMODE_256COLOR | VMODE_PALETTE
};

void ltdc_set_palette_reg(int index, int rgb)
{
	LTDC_Layer1->CLUTWR = ((index & 0xFF) << 24u) | (rgb & 0xFFFFFFu);
}

void ltdc_init(const videomode_t *mode, unsigned int base_address)
{
	int i, r, g, b;

	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;

	LTDC->SSCR = ((mode->hsync_width - 1) << 16ul) + (mode->vsync_width - 1);
	LTDC->BPCR = ((mode->hsync_width + mode->h_back - 1) << 16ul) + (mode->vsync_width + mode->v_back - 1);
	LTDC->AWCR = ((mode->hsync_width + mode->h_back + mode->h_active - 1) << 16ul) + (mode->vsync_width + mode->v_back + mode->v_active - 1);
	LTDC->TWCR = ((mode->hsync_width + mode->h_back + mode->h_active + mode->h_front - 1) << 16ul) + (mode->vsync_width + mode->v_back + mode->v_active + mode->v_front - 1);
	LTDC->BCCR = 0;

	ltdc_set_palette_reg(0, RGB24(0, 0, 0));
	ltdc_set_palette_reg(1, RGB24(0, 0, 128));
	ltdc_set_palette_reg(2, RGB24(0, 96, 0));
	ltdc_set_palette_reg(3, RGB24(0, 160, 96));
	ltdc_set_palette_reg(4, RGB24(128, 0, 0));
	ltdc_set_palette_reg(5, RGB24(128, 0, 160));
	ltdc_set_palette_reg(6, RGB24(160, 96, 0));
	ltdc_set_palette_reg(7, RGB24(160, 160, 160));
	ltdc_set_palette_reg(8, RGB24(80, 80, 80));
	ltdc_set_palette_reg(9, RGB24(0, 0, 255));
	ltdc_set_palette_reg(10, RGB24(0, 255, 0));
	ltdc_set_palette_reg(11, RGB24(0, 255, 255));
	ltdc_set_palette_reg(12, RGB24(255, 0, 0));
	ltdc_set_palette_reg(13, RGB24(255, 0, 255));
	ltdc_set_palette_reg(14, RGB24(255, 255, 0));
	ltdc_set_palette_reg(15, RGB24(255, 255, 255));

	for (i=16,r=0;r<6;r++)
		for (g=0;g<6;g++)
			for (b=0;b<6;b++)
				ltdc_set_palette_reg(i++, RGB24(r * 51, g * 51, b * 51));

	for (b=0;b<32;b++)
		ltdc_set_palette_reg(224 + b, RGB24(b * 8, b * 8, b * 8));

	LTDC_Layer1->WHPCR = (mode->hsync_width + mode->h_back) | ((mode->hsync_width + mode->h_back + mode->h_active) << 16ul);
	LTDC_Layer1->WVPCR = (mode->vsync_width + mode->v_back) | ((mode->vsync_width + mode->v_back + mode->v_active) << 16ul);
	LTDC_Layer1->PFCR = mode->flags & 0x07;
	LTDC_Layer1->CFBAR = base_address;
	LTDC_Layer1->CFBLR = (mode->h_active << 16ul) | (mode->h_active + 4);
	LTDC_Layer1->CFBLNR = mode->v_active;
	LTDC_Layer1->CR |= (mode->flags & VMODE_PALETTE) | 0x01;
	
	LTDC->SRCR |= 1;

	LTDC->GCR = (mode->flags & 0xF0000000u) | 0x2221u;
}

#endif
