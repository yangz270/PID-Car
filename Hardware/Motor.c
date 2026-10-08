#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "PWM.h"

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11);				//初始化后让电机停止，恢复默认值

	PWM_Init();													//声明/定义/调用的写法一定要区分开，这里是调用，意思就是按下了启动按钮
}
	
void Motor_Left_SetSpeed(int16_t Speed)
	{
		if(Speed > MOTOR_MAX_SPEED)
		{
			Speed = MOTOR_MAX_SPEED;
		}
		else if(Speed < -MOTOR_MAX_SPEED)
		{
			Speed = -MOTOR_MAX_SPEED;										//限定范围
		}
		
		if(Speed > 0)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			
			PWM_SetCompare1((uint16_t)Speed * 10);				//含义是：得到一个数speed，将它强制转换成uint型，10是为了将可调参数变成100份
		}
		else if(Speed < 0)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
			
			PWM_SetCompare1((uint16_t)(-Speed) * 10);			//先计算-speed，再强制转换
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
			
			PWM_SetCompare1(0);
		}
	}
void Motor_Right_SetSpeed(int16_t Speed)
	{
		if(Speed > MOTOR_MAX_SPEED)
		{
			Speed = MOTOR_MAX_SPEED;
		}
		else if(Speed < -MOTOR_MAX_SPEED)
		{
			Speed = -MOTOR_MAX_SPEED;										
		}
		
		if(Speed > 0)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			GPIO_ResetBits(GPIOB, GPIO_Pin_11);
			
			PWM_SetCompare2((uint16_t)Speed * 10);			
		}
		else if(Speed < 0)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			GPIO_SetBits(GPIOB, GPIO_Pin_11);
			
			PWM_SetCompare2((uint16_t)(-Speed) * 10);
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
			
			PWM_SetCompare2(0);
		}
	}
