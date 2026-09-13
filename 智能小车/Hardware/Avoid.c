#include "stm32f10x.h"
#include "Time.h"
#include "Motor.h"
#include "Servo.h"
#include "Ultrasonic.h"
#include "Avoid.h"

#define AVOID_FRONT_LIMIT_CM  25
#define AVOID_LEFT_ANGLE      30
#define AVOID_CENTER_ANGLE    90
#define AVOID_RIGHT_ANGLE     150
#define AVOID_SCAN_WAIT_MS    250
#define AVOID_TURN_TIME_MS    500
#define AVOID_FORWARD_SPEED   60
#define AVOID_TURN_SPEED      100
#define AVOID_NEAR_CONFIRM_COUNT 2

typedef enum
{
	AVOID_STATE_IDLE = 0,
	AVOID_STATE_MEASURE_FRONT,
	AVOID_STATE_WAIT_LEFT,
	AVOID_STATE_WAIT_RIGHT,
	AVOID_STATE_TURN,
	AVOID_STATE_FORWARD
} AvoidState_t;

static AvoidState_t avoid_state;
static uint8_t avoid_running;
static uint16_t avoid_left_distance;
static uint16_t avoid_right_distance;
static uint32_t avoid_deadline;
static uint8_t avoid_near_count;

static uint8_t Avoid_TimeReached(uint32_t deadline)
{
	return ((int32_t)(Time_GetMs() - deadline) >= 0);
}

void Avoid_Init(void)
{
	Servo_Init();
	Ultrasonic_Init();
	Servo_Center();
	avoid_state = AVOID_STATE_IDLE;
	avoid_running = 0;
	avoid_near_count = 0;
}

void Avoid_Start(void)
{
	Motor_Stop();
	Servo_SetAngle(AVOID_CENTER_ANGLE);
	avoid_running = 1;
	avoid_state = AVOID_STATE_MEASURE_FRONT;
	avoid_near_count = 0;
	avoid_deadline = Time_GetMs() + AVOID_SCAN_WAIT_MS;
}

void Avoid_Stop(void)
{
	avoid_running = 0;
	avoid_state = AVOID_STATE_IDLE;
	avoid_near_count = 0;
	Servo_Center();
	Motor_Stop();
}

uint8_t Avoid_IsRunning(void)
{
	return avoid_running;
}

void Avoid_Task(void)
{
	uint16_t front_distance;

	if (!avoid_running || !Avoid_TimeReached(avoid_deadline))
	{
		return;
	}

	switch (avoid_state)
	{
		case AVOID_STATE_MEASURE_FRONT:
			front_distance = Ultrasonic_GetDistanceCm();
			if (front_distance > AVOID_FRONT_LIMIT_CM)
			{
				avoid_near_count = 0;
				Motor_Forward(AVOID_FORWARD_SPEED);
				avoid_state = AVOID_STATE_FORWARD;
				avoid_deadline = Time_GetMs() + 60;
			}
			else
			{
				if (avoid_near_count < AVOID_NEAR_CONFIRM_COUNT)
				{
					avoid_near_count++;
				}

				if (avoid_near_count >= AVOID_NEAR_CONFIRM_COUNT)
				{
					Motor_Stop();
					Servo_SetAngle(AVOID_LEFT_ANGLE);
					avoid_state = AVOID_STATE_WAIT_LEFT;
					avoid_deadline = Time_GetMs() + AVOID_SCAN_WAIT_MS;
				}
				else
				{
					Motor_Forward(AVOID_FORWARD_SPEED);
					avoid_state = AVOID_STATE_FORWARD;
					avoid_deadline = Time_GetMs() + 60;
				}
			}
			break;

		case AVOID_STATE_FORWARD:
			avoid_state = AVOID_STATE_MEASURE_FRONT;
			avoid_deadline = Time_GetMs() + 60;
			break;

		case AVOID_STATE_WAIT_LEFT:
			avoid_left_distance = Ultrasonic_GetDistanceCm();
			Servo_SetAngle(AVOID_RIGHT_ANGLE);
			avoid_state = AVOID_STATE_WAIT_RIGHT;
			avoid_deadline = Time_GetMs() + AVOID_SCAN_WAIT_MS;
			break;

		case AVOID_STATE_WAIT_RIGHT:
			avoid_right_distance = Ultrasonic_GetDistanceCm();
			avoid_near_count = 0;
			Servo_SetAngle(AVOID_CENTER_ANGLE);
			if (avoid_left_distance >= avoid_right_distance)
			{
				Motor_TurnLeft(AVOID_TURN_SPEED);
			}
			else
			{
				Motor_TurnRight(AVOID_TURN_SPEED);
			}
			avoid_state = AVOID_STATE_TURN;
			avoid_deadline = Time_GetMs() + AVOID_TURN_TIME_MS;
			break;

		case AVOID_STATE_TURN:
			Motor_Forward(AVOID_FORWARD_SPEED);
			avoid_state = AVOID_STATE_FORWARD;
			avoid_deadline = Time_GetMs() + 100;
			break;

		default:
			Motor_Stop();
			avoid_state = AVOID_STATE_MEASURE_FRONT;
			avoid_deadline = Time_GetMs() + AVOID_SCAN_WAIT_MS;
			break;
	}
}
