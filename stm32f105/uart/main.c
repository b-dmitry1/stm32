#include <stdio.h>
#include "board1xx.h"

int main(void)
{
	int a, b;
	char op;

	rcc_high_performance(XTAL_16MHZ);

	pin_conf_output(PORTA, 12);
	pin_low(PORTA, 12);

	uart_open(2, 115200, 8, NOPARITY, ONESTOPBIT, NO485);

	select_stdio_uart(2);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	for (;;)
	{
		printf("\nEnter expression (a op b): ");
		scanf("%d %c %d", &a, &op, &b);

		printf("\r\n%d %c %d = ", a, op, b);
		switch (op)
		{
			case '+': printf("%d\n", a + b); break;
			case '-': printf("%d\n", a - b); break;
			case '*': printf("%d\n", a * b); break;
			case '/': printf("%d\n", a / b); break;
			default: printf("error\n");
		}
	}
}
