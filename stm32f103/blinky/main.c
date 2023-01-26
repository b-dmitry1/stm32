#include "board103.h"

void wait(void)
{
	volatile int w;
	for (w = 0; w < 1000000; w++);
}

int main(void)
{
	board_init();

	set_pin_mode(GPIOC, 13, OUTPUT);

	for (;;)
	{
		clr_pin(GPIOC, 13);
		wait();
		set_pin(GPIOC, 13);
		wait();
	}
}
