#include "stm32f10x.h"                  // Device header
#include "PID.h"						//计算修正量

void PID_Init(PID_TypeDef *PID, float Kp, float Ki, float Kd, float IntegralLimit, float OutputLimit)
{
    PID->Kp = Kp;								//PID在这里是指向结构体的指针， -> 是通过指针访问结构体里面的成员
    PID->Ki = Ki;
    PID->Kd = Kd;
	PID->IntegralLimit = IntegralLimit;
	PID->OutputLimit = OutputLimit;
	
    PID_Reset(PID);								//因为这里本来就是取的指针所以不需要&
}

void PID_Reset(PID_TypeDef *PID)				//将PID写成指针的意义就在于：使结构体里面的变量是可改变的
{
    PID->Target = 0;
    PID->Actual = 0;

    PID->Error = 0;
    PID->LastError = 0;

    PID->Integral = 0;

    PID->Output = 0;
	
}												//开启新一轮的运行前要清除过去旧的状态

float PID_Calculate(PID_TypeDef *PID, float Actual)
{
    PID->Actual = Actual;

    PID->Error = PID->Target - PID->Actual;

    PID->Integral += PID->Error;
	
	if(PID->Integral > PID->IntegralLimit)
	{
	PID->Integral = PID->IntegralLimit;
	}
	
	if(PID->Integral < -PID->IntegralLimit)
	{
	PID->Integral = -PID->IntegralLimit;									//积分限幅
	}

    PID->Output = PID->Kp * PID->Error + PID->Ki * PID->Integral + PID->Kd * (PID->Error - PID->LastError);
	
	if(PID->Output > PID->OutputLimit)
{
    PID->Output = PID->OutputLimit;
}

	if(PID->Output < -PID->OutputLimit)
{
    PID->Output = -PID->OutputLimit;										//输出限幅
}

    PID->LastError = PID->Error;

    return PID->Output;
}
