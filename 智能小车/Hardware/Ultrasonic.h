#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f10x.h"

void Ultrasonic_Init(void);
uint16_t Ultrasonic_GetDistanceCm(void);

#endif
