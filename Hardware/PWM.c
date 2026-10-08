#include "stm32f10x.h"                  // Device header
#include "PWM.h"						//控制电机

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//AF复用，表示这不是普通的GPIO，这和引脚要交给定时器TIM控制
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//开启并配置输出口
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//分频设置，表示不额外进行分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式，向上计数
	TIM_TimeBaseStructure.TIM_Period = 999;					//ARR，设置周期
	TIM_TimeBaseStructure.TIM_Prescaler = 71;				//PSC，设置分频
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//不开启重复计数
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//定时器配置表，配置要输出的是什么
	
	TIM_OCInitTypeDef TIM_OCInitStructure;					//OC：Output Compare，输出比较
	
	TIM_OCStructInit(&TIM_OCInitStructure);					//这样写可以先给里面所有变量附上0值，设为默认
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//允许输出
	TIM_OCInitStructure.TIM_Pulse = 0;						//CRR设置占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//采用高电平作为有效输出极性
															//PWM通道配置表
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);				//对应CH1通道1
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);				//对应CH2通道2
	
	TIM_Cmd(TIM2, ENABLE);									//开启定时器
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}
