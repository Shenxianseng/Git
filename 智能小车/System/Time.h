#ifndef __TIME_H
#define __TIME_H

#include "stm32f10x.h"

void Time_Init(void);
uint32_t Time_GetMs(void);
void Time_IncMs(void);

#endif
