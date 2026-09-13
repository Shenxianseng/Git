#include "stm32f10x.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

int main(void)
{
	uint8_t key_num;
	uint8_t action = 0;

	OLED_Init();
	Motor_Init();
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
					Motor_Stop();
					OLED_ShowString(2, 1, "STOP       ");
					break;
				case 1:
					Motor_Forward(100);
					OLED_ShowString(2, 1, "FORWARD    ");
					break;
				case 2:
					Motor_Backward(100);
					OLED_ShowString(2, 1, "BACKWARD   ");
					break;
				case 3:
					Motor_TurnLeft(100);
					OLED_ShowString(2, 1, "TURN LEFT  ");
					break;
				case 4:
					Motor_TurnRight(100);
					OLED_ShowString(2, 1, "TURN RIGHT ");
					break;
				default:
					Motor_Stop();
					break;
			}
		}
	}
}
