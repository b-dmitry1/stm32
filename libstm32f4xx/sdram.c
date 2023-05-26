#include "stm32f4xx.h"
#include "board.h"

#ifdef STM32F429_439xx

#define REFRESH_RATE	600

static void sdram_cmd(unsigned int mrd, unsigned int nrfs, unsigned int mode)
{
	FMC_Bank5_6->SDCMR = (mrd << 9ul) | (nrfs << 5ul) | (3 << 3ul) | (mode << 0ul);
	
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
}

static void sdram_wait(void)
{
	volatile int w;
	for (w = 0; w < 100000; w++);
}

void sdram_init(int cas_latency, int burst_read, int data_width, int num_banks, int rows, int cols)
{
	unsigned int sdcr0, sdram_conf;

	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	sdram_wait();

	// Read pipe 1 clk, 2 HCLK div
	sdcr0 = (0 << 13ul) | (2 << 10ul);

	switch (cas_latency)
	{
		case 2: sdcr0 |= 2 << 7; break;
		default: sdcr0 |= 3 << 7; break;
	}

	if (burst_read)
	{
		sdcr0 |= 1 << 12;
	}

	switch (data_width)
	{
		case 8: sdcr0 |= 0 << 4; break;
		case 32: sdcr0 |= 2 << 4; break;
		default: sdcr0 |= 1 << 4; break;
	}

	switch (num_banks)
	{
		case 2: sdcr0 |= 0 << 6; break;
		default: sdcr0 |= 1 << 7; break;
	}

	switch (rows)
	{
		case 11: sdcr0 |= 0 << 2; break;
		case 12: sdcr0 |= 1 << 2; break;
		default: sdcr0 |= 2 << 2; break;
	}

	switch (cols)
	{
		case 8: sdcr0 |= 0 << 0; break;
		case 10: sdcr0 |= 2 << 0; break;
		case 11: sdcr0 |= 3 << 0; break;
		default: sdcr0 |= 1 << 0; break;
	}

	FMC_Bank5_6->SDCR[0] = sdcr0;

	FMC_Bank5_6->SDTR[0] =
		(1 << 24ul) |	// RCD 2
		(1 << 20ul) |	// RP 2
		(1 << 16ul) |	// WR 2
		(7 << 12ul) |	// RC 6
		(3 << 8ul) |	// SRT 4
		(6 << 4ul) |	// XSR 7
		(1 << 0ul);	// TMRD 2
	
	sdram_cmd(0, 1, 1);
	sdram_wait();
	
	sdram_cmd(0, 1, 2);
	sdram_wait();
	
	sdram_cmd(0, 8, 3);
	sdram_wait();
	
	// Write burst single
	sdram_conf = 1 << 9;

	sdram_conf |= cas_latency == 2 ? 2 << 4 : 3 << 4;

	sdram_cmd(sdram_conf, 1, 4);
		
	FMC_Bank5_6->SDRTR = (REFRESH_RATE << 1ul) | 1ul;
	
	sdram_wait();
}

#endif
