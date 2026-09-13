#include "stm32f10x.h"
#include "Ultrasonic.h"

#define ULTRASONIC_TRIG_PIN       GPIO_Pin_12
#define ULTRASONIC_ECHO_PIN       GPIO_Pin_13
#define ULTRASONIC_ECHO_TIMEOUT_US 30000

static uint8_t Ultrasonic_WaitEcho(uint8_t level, uint16_t timeout_us)
{
	uint16_t start_count;

	start_count = TIM_GetCounter(TIM3);
	while (GPIO_ReadInputDataBit(GPIOB, ULTRASONIC_ECHO_PIN) != level)
	{
		if ((uint16_t)(TIM_GetCounter(TIM3) - start_count) >= timeout_us)
		{
			return 0;
		}
	}
	return 1;
}

void Ultrasonic_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = ULTRASONIC_TRIG_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = ULTRASONIC_ECHO_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM3, ENABLE);

	GPIO_ResetBits(GPIOB, ULTRASONIC_TRIG_PIN);
}

uint16_t Ultrasonic_GetDistanceCm(void)
{
	uint16_t start_count;
	uint16_t echo_time_us;
	uint16_t distance_cm;

	TIM_SetCounter(TIM3, 0);
	GPIO_ResetBits(GPIOB, ULTRASONIC_TRIG_PIN);
	while (TIM_GetCounter(TIM3) < 2)
	{
	}

	GPIO_SetBits(GPIOB, ULTRASONIC_TRIG_PIN);
	TIM_SetCounter(TIM3, 0);
	while (TIM_GetCounter(TIM3) < 10)
	{
	}
	GPIO_ResetBits(GPIOB, ULTRASONIC_TRIG_PIN);

	//脉冲发送完成
	TIM_SetCounter(TIM3, 0);
	if (!Ultrasonic_WaitEcho(1, ULTRASONIC_ECHO_TIMEOUT_US))
	{
		return 400;
	}

	start_count = TIM_GetCounter(TIM3);
	if (!Ultrasonic_WaitEcho(0, ULTRASONIC_ECHO_TIMEOUT_US))
	{
		return 400;
	}

	echo_time_us = (uint16_t)(TIM_GetCounter(TIM3) - start_count);
	distance_cm = echo_time_us / 58;

	if (distance_cm > 400)
	{
		distance_cm = 400;
	}
	return distance_cm;
}
