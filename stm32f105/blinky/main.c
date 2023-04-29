#include "board1xx.h"

void wait(void)
{
	volatile int w;
	for (w = 0; w < 1000000; w++);
}

int main(void)
{
	rcc_high_performance(XTAL_16MHZ);

	pin_conf_output(LED);

	for (;;)
	{
		pin_low(LED);
		wait();
		pin_high(LED);
		wait();
	}
}
