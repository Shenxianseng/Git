#include "Motor.h"
#include "CarControl.h"

void CarControl_Init(void)
{
	Motor_Init();
}

void CarControl_Process(uint8_t command)
{
	switch (command)
	{
		case 'F':
		case 'f':
			Motor_Forward(CAR_SPEED);
			break;
		case 'B':
		case 'b':
			Motor_Backward(CAR_SPEED);
			break;
		case 'L':
		case 'l':
			Motor_TurnLeft(CAR_SPEED);
			break;
		case 'R':
		case 'r':
			Motor_TurnRight(CAR_SPEED);
			break;
		case 'S':
		case 's':
		case '0':
			Motor_Stop();
			break;
		default:
			break;
	}
}
