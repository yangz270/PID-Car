#include "stm32f10x.h"                  // Device header
#include "Car.h"
#include "Motor.h"

void Car_Init(void)
{
		Motor_Init();					//初始化电机
}

void Car_Forward(int16_t Speed)
{
	Motor_Left_SetSpeed(Speed);
	Motor_Right_SetSpeed(Speed);
}

void Car_Backward(int16_t Speed)
{
	Motor_Left_SetSpeed(-Speed);
	Motor_Right_SetSpeed(-Speed);
}

void Car_TurnLeft(int16_t Speed)
{
	Motor_Left_SetSpeed(Speed);
	Motor_Right_SetSpeed(0);
}

void Car_TurnRight(int16_t Speed)
{
	Motor_Left_SetSpeed(0);
	Motor_Right_SetSpeed(Speed);
}

void Car_Stop(void)
{
	Motor_Left_SetSpeed(0);
	Motor_Right_SetSpeed(0);
}
