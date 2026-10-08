#include "stm32f10x.h"                  // Device header
#include "Speed.h"
#include "Encoder.h"

static volatile int16_t LeftSpeed;					//封装思想，如果没有static，其他的文件用extern也能访问，设为局部变量是为了让Speed只能由Speed_Get读取
static volatile int16_t RightSpeed;

void Speed_Init(void)
{
    LeftSpeed = 0;							//使用前先清零
	RightSpeed = 0;	
}

void Speed_Update(void)
{
    LeftSpeed = Encoder_Left_GetCount();  		//获取编码器的CNT
	RightSpeed = Encoder_Right_GetCount();
}


float	Speed_Left_Get(void)
{
    return LeftSpeed;						//返回速度值
}

float Speed_Right_Get(void)
{
    return RightSpeed;						//返回速度值
}
