#include "Motor.h"
#include "Avoid.h"
#include "Servo.h"
#include "CarControl.h"

void CarControl_Init(void)
{
	Motor_Init();
	Avoid_Init();
}

void CarControl_Process(uint8_t command)
{
	switch (command)
	{
		case 'A':
		case 'a':
			Avoid_Start();
			break;
		case 'T':
		case 't':
			Avoid_Stop();
			Servo_TestNext();
			break;
		case 'F':
		case 'f':
			Avoid_Stop();
			Motor_Forward(CAR_SPEED);
			break;
		case 'B':
		case 'b':
			Avoid_Stop();
			Motor_Backward(CAR_SPEED);
			break;
		case 'L':
		case 'l':
			Avoid_Stop();
			Motor_TurnLeft(CAR_SPEED);
			break;
		case 'R':
		case 'r':
			Avoid_Stop();
			Motor_TurnRight(CAR_SPEED);
			break;
		case 'S':
		case 's':
		case '0':
			Avoid_Stop();
			Motor_Stop();
			break;
		default:
			break;
	}
}

void CarControl_Task(void)
{
	Avoid_Task();
}
