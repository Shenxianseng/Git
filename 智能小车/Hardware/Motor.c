#include "stm32f10x.h"
#include "PWM.h"
#include "Motor.h"

static int16_t Motor_LimitSpeed(int16_t speed)
{
	if (speed > 100)
	{
		return 100;
	}
	if (speed < -100)
	{
		return -100;
	}
	return speed;
}

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	PWM_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOTOR_LEFT_IN1_PIN | MOTOR_LEFT_IN2_PIN |
								  MOTOR_RIGHT_IN1_PIN | MOTOR_RIGHT_IN2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Motor_Stop();
}

void Motor_SetLeftSpeed(int16_t speed)
{
	speed = Motor_LimitSpeed(speed);
	if (speed >= 0)
	{
		GPIO_ResetBits(GPIOA, MOTOR_LEFT_IN1_PIN);
		GPIO_SetBits(GPIOA, MOTOR_LEFT_IN2_PIN);
		PWM_SetCompare3((uint16_t)speed);
	}
	else
	{
		GPIO_SetBits(GPIOA, MOTOR_LEFT_IN1_PIN);
		GPIO_ResetBits(GPIOA, MOTOR_LEFT_IN2_PIN);
		PWM_SetCompare3((uint16_t)(-speed));
	}
}

void Motor_SetRightSpeed(int16_t speed)
{
	speed = Motor_LimitSpeed(speed);
	if (speed >= 0)
	{
		GPIO_ResetBits(GPIOA, MOTOR_RIGHT_IN1_PIN);
		GPIO_SetBits(GPIOA, MOTOR_RIGHT_IN2_PIN);
		PWM_SetCompare4((uint16_t)speed);
	}
	else
	{
		GPIO_SetBits(GPIOA, MOTOR_RIGHT_IN1_PIN);
		GPIO_ResetBits(GPIOA, MOTOR_RIGHT_IN2_PIN);
		PWM_SetCompare4((uint16_t)(-speed));
	}
}

void Motor_SetSpeed(int16_t left_speed, int16_t right_speed)
{
	Motor_SetLeftSpeed(left_speed);
	Motor_SetRightSpeed(right_speed);
}

void Motor_Stop(void)
{
	PWM_SetCompare3(0);
	PWM_SetCompare4(0);
	GPIO_ResetBits(GPIOA, MOTOR_LEFT_IN1_PIN | MOTOR_LEFT_IN2_PIN |
				  MOTOR_RIGHT_IN1_PIN | MOTOR_RIGHT_IN2_PIN);
}

void Motor_Forward(uint16_t speed)
{
	Motor_SetSpeed((int16_t)speed, (int16_t)speed);
}

void Motor_Backward(uint16_t speed)
{
	Motor_SetSpeed(-(int16_t)speed, -(int16_t)speed);
}

void Motor_TurnLeft(uint16_t speed)
{
	Motor_SetSpeed(-(int16_t)speed, (int16_t)speed);
}

void Motor_TurnRight(uint16_t speed)
{
	Motor_SetSpeed((int16_t)speed, -(int16_t)speed);
}
