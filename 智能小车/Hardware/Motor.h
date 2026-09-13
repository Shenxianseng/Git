#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

/* 左轮：PA2 为 PWM，PA4/PA5 为方向 */
#define MOTOR_LEFT_IN1_PIN   GPIO_Pin_4
#define MOTOR_LEFT_IN2_PIN   GPIO_Pin_5

/* 右轮：PA3 为 PWM，PA6/PA7 为方向 */
#define MOTOR_RIGHT_IN1_PIN  GPIO_Pin_6
#define MOTOR_RIGHT_IN2_PIN  GPIO_Pin_7

void Motor_Init(void);
void Motor_SetLeftSpeed(int16_t speed);
void Motor_SetRightSpeed(int16_t speed);
void Motor_SetSpeed(int16_t left_speed, int16_t right_speed);
void Motor_Stop(void);
void Motor_Forward(uint16_t speed);
void Motor_Backward(uint16_t speed);
void Motor_TurnLeft(uint16_t speed);
void Motor_TurnRight(uint16_t speed);

#endif

