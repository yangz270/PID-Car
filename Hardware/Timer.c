#include "stm32f10x.h"                  // Device headers             
#include "Timer.h"

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 99;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				//设定为一个周期10ms
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);						//清除 TIM1 的更新中断标志，清除过去/上一次操作的痕迹
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						//配置TIM1时钟源，开启它的中断更新，使能；即要求CNT在每次计数完成产生更新事件时，触发更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;				//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//开启这个中断通道
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);											//启动 TIM1 计数器
}
