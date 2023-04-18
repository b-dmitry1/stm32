#include "board4xx.h"

#define LED	PORTD, 11

void wait(void)
{
	volatile int w;
	for (w = 0; w < 1000000; w++);
}

int main(void)
{
	board_init();

	pin_conf_output(LED);

	for (;;)
	{
		pin_low(LED);
		wait();
		pin_high(LED);
		wait();
	}
}
