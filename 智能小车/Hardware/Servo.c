#include "stm32f10x.h"
#include "Servo.h"

#define SERVO_MIN_PULSE_US  500
#define SERVO_MAX_PULSE_US  2500

static uint8_t servo_test_step;

void Servo_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	servo_test_step = 0;
}

void Servo_SetAngle(uint8_t angle)
{
	uint16_t pulse;

	if (angle > 180)
	{
		angle = 180;
	}

	pulse = SERVO_MIN_PULSE_US +
			(uint16_t)(((uint32_t)(SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) *
						angle) / 180);
	TIM_SetCompare1(TIM1, pulse);
}

void Servo_Center(void)
{
	Servo_SetAngle(90);
}

void Servo_TestNext(void)
{
	servo_test_step++;
	if (servo_test_step > 3)
	{
		servo_test_step = 0;
	}

	switch (servo_test_step)
	{
		case 0:
			Servo_SetAngle(90);
			break;
		case 1:
			Servo_SetAngle(30);
			break;
		case 2:
			Servo_SetAngle(150);
			break;
		case 3:
			Servo_SetAngle(90);
			break;
		default:
			Servo_Center();
			break;
	}
}
