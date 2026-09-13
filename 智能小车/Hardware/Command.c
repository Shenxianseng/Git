#include "USART.h"
#include "CarControl.h"
#include "Command.h"

void Command_Init(void)
{
	USART1_Init();
}

void Command_Task(void)
{
	uint8_t command;

	if (USART1_GetByte(&command))
	{
		CarControl_Process(command);
	}
}
