#ifndef __AVOID_H
#define __AVOID_H

#include "stm32f10x.h"

void Avoid_Init(void);
void Avoid_Start(void);
void Avoid_Stop(void);
void Avoid_Task(void);
uint8_t Avoid_IsRunning(void);

#endif
