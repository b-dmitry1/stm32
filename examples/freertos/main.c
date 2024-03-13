#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>
#include "board.h"

#define RXD1	PORTA, 10
#define TXD1	PORTA, 9

static StaticTask_t task1tcb;
static StackType_t  task1stack[4096];
static StaticTask_t task2tcb;
static StackType_t  task2stack[4096];

void dbg(const char *msg)
{
	uart_send_str(1, msg);
}

void task1(void *parameters)
{
	(void)parameters;

	for (;;)
	{
		printf("first ");
		vTaskDelay(1000);
	}
}

void task2(void *parameters)
{
	(void)parameters;

	vTaskDelay(250);

	for (;;)
	{
		printf("second ");
		vTaskDelay(500);
	}
}

int main(void)
{
	rcc_high_performance(XTAL_16MHZ);

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, NO485);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	xTaskCreateStatic(task1, "task1", sizeof(task1stack) / sizeof(task1stack[0]), 
		NULL, configMAX_PRIORITIES - 1, task1stack, &task1tcb);

	xTaskCreateStatic(task2, "task2", sizeof(task2stack) / sizeof(task2stack[0]), 
		NULL, configMAX_PRIORITIES - 1, task2stack, &task2tcb);

	vTaskStartScheduler();

	for (;;)
	{
	}
}
