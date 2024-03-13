#include "FreeRTOS.h"
#include "task.h"

void dbg(const char *msg);

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void) pxTask;
	dbg("stack overflow in ");
	dbg((char *)pcTaskName);
	taskDISABLE_INTERRUPTS();
	for(;;);
}

void vApplicationMallocFailedHook(void)
{
	dbg("malloc failed");
	taskDISABLE_INTERRUPTS();
	for(;;);
}

static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
