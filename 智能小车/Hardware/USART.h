#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

void USART1_Init(void);
uint8_t USART1_GetByte(uint8_t *byte);
void USART1_SendByte(uint8_t byte);

#endif
