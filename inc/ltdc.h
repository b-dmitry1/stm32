#ifndef LTDC_H
#define LTDC_H

#ifdef STM32F429_439xx

#define VMODE_HSYNC_HIGH	(1 << 31u)
#define VMODE_VSYNC_HIGH	(1 << 30u)
#define VMODE_DE_HIGH		(1 << 29u)
#define VMODE_DCLK_INV		(1 << 28u)
#define VMODE_DITHER		(1 << 16u)

#define VMODE_PALETTE		(1 << 4u)

#define VMODE_ARGB8888		(0 << 0)
#define VMODE_RGB888		(1 << 0)
#define VMODE_RGB565		(2 << 0)
#define VMODE_ARGB1555		(3 << 0)
#define VMODE_ARGB4444		(4 << 0)
#define VMODE_256COLOR		(5 << 0)

#define RGB24(r, g, b)		((((r) & 0xFF) << 16u) | (((g) & 0xFF) << 8u) | (((b) & 0xFF) << 0u))

typedef struct
{
	int hsync_width;
	int h_back;
	int h_active;
	int h_front;
	int vsync_width;
	int v_back;
	int v_active;
	int v_front;
	int flags;
} videomode_t;

extern const videomode_t vga640x480x8;

#ifdef __cplusplus
extern "C" {
#endif

void ltdc_set_palette_reg(int index, int rgb);
void ltdc_init(const videomode_t *mode, unsigned int base_address);

#ifdef __cplusplus
}
#endif

#endif

#endif
