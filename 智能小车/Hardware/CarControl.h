#ifndef __CAR_CONTROL_H
#define __CAR_CONTROL_H

#include "stm32f10x.h"

#define CAR_SPEED 100

void CarControl_Init(void);
void CarControl_Process(uint8_t command);
void CarControl_Task(void);

#endif
