#include "stm32f10x.h"
#include "OLED.h"
#include "CarControl.h"
#include "Command.h"
#include "Key.h"

int main(void)
{
	uint8_t key_num;
	uint8_t action = 0;

	OLED_Init();
	CarControl_Init();
	Command_Init();
	Key_Init();

	OLED_ShowString(1, 1, "CAR TEST");
	OLED_ShowString(2, 1, "STOP       ");

	while (1)
	{
		key_num = Key_GetNum();
		if (key_num == 1)
		{
			action++;
			if (action > 4)
			{
				action = 0;
			}

			switch (action)
			{
				case 0:
					CarControl_Process('S');
					OLED_ShowString(2, 1, "STOP       ");
					break;
				case 1:
					CarControl_Process('F');
					OLED_ShowString(2, 1, "FORWARD    ");
					break;
				case 2:
					CarControl_Process('B');
					OLED_ShowString(2, 1, "BACKWARD   ");
					break;
				case 3:
					CarControl_Process('L');
					OLED_ShowString(2, 1, "TURN LEFT  ");
					break;
				case 4:
					CarControl_Process('R');
					OLED_ShowString(2, 1, "TURN RIGHT ");
					break;
				default:
					CarControl_Process('S');
					break;
			}
		}

		Command_Task();
	}
}
