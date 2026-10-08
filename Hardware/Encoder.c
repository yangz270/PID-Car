#include "stm32f10x.h"                  // Device header
#include "Encoder.h"					//测量速度

void Encoder_Left_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef Tim_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;							//把定义结构体放在一起

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//配置时钟，打开输出口和时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					//Input Pull Up,上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//配置GPIO
	
	Tim_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//分频，不额外分频
	Tim_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//定时器计数模式，向上计数
	Tim_TimeBaseStructure.TIM_Period = 65535;						//ARR自动重装值
	Tim_TimeBaseStructure.TIM_Prescaler = 0; 						//预分频器，不进行预分频
	Tim_TimeBaseStructure.TIM_RepetitionCounter = 0;				//重复计数器
	
	TIM_TimeBaseInit(TIM3, &Tim_TimeBaseStructure);					//配置时钟TIM3
	
	TIM_ICStructInit(&TIM_ICInitStructure);							//将输入捕获设为默认值，要向编码器捕获车轮的转速和方向
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入捕获数字滤波器，滤波时间15，表示开启较强的数字滤波
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel =  TIM_Channel_2;						
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);			//配置编码器接口模式，参数（定时器，TI12使用输入通道1、2，CH1的极性按上升沿判断，CH2的极性按上升沿判断）
	
	TIM_SetCounter(TIM3, 0);										//清零计数器
	
	TIM_Cmd(TIM3, ENABLE);											//启动计数器
}

void Encoder_Right_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef Tim_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;						

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	Tim_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		
	Tim_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	Tim_TimeBaseStructure.TIM_Period = 65535;					
	Tim_TimeBaseStructure.TIM_Prescaler = 0; 				
	Tim_TimeBaseStructure.TIM_RepetitionCounter = 0;		
	
	TIM_TimeBaseInit(TIM4, &Tim_TimeBaseStructure);			
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	
	
	TIM_SetCounter(TIM4, 0);										
	
	TIM_Cmd(TIM4, ENABLE);										
}

int16_t Encoder_Left_GetCount(void)									//表示这个函数最终返回一个16为有符号的整数，获取编码器这一段时间产生的计数，不需要传参数
{
	int16_t Count;													//创建一个Count变量，用于暂时保存编码器计数
	
	Count = TIM_GetCounter(TIM3);									//读取TIM3当前CNT寄存器的值
	
	TIM_SetCounter(TIM3, 0);										//读取完成后马上清零，为了得到单位时间内产生的计数
	
	return Count;													//返回值
}

int16_t Encoder_Right_GetCount(void)
{
    int16_t Count;

    Count = TIM_GetCounter(TIM4);

    TIM_SetCounter(TIM4, 0);

    return Count;
}
