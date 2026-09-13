#include "delay.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

static void Delay_1ms(void)
{
    uint16_t i;
    for(i = 0; i < 1100; i++)
    {
        __NOP();
    }
}

// 外部调用：自由输入毫秒
void Delay_ms(uint32_t ms)
{
    while(ms--)
    {
        Delay_1ms();
    }
}