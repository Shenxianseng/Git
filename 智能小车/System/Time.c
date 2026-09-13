#include "stm32f10x.h"
#include "Time.h"

static volatile uint32_t time_ms;

void Time_Init(void)
{
	time_ms = 0;
	SysTick_Config(SystemCoreClock / 1000);
}

uint32_t Time_GetMs(void)
{
	return time_ms;
}

void Time_IncMs(void)
{
	time_ms++;
}
