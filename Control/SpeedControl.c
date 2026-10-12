#include "stm32f10x.h"                  // Device header
#include "SpeedControl.h"				//管理电机速度控制流程，任务是获取速度，PID计算，控制电机，设计函数
#include "Speed.h"
#include "PID.h"
#include "Motor.h"

PID_TypeDef Left_PID;
PID_TypeDef Right_PID;

void SpeedControl_Init(void)
{
	PID_Init(&Left_PID, 0.04f, 0.003, 0, 20000, 60);
	
	PID_Init(&Right_PID, 0.04f, 0.003, 0, 20000, 60);

}

void SpeedControl_SetTarget(float LeftTarget, float RightTarget)
{
    Left_PID.Target = LeftTarget;
	
	Right_PID.Target = RightTarget;
}

void SpeedControl_Update(void)
{
	int16_t LeftSpeed;
	int16_t RightSpeed;
    float LeftPWM;
    float RightPWM;

    LeftSpeed = Speed_Left_Get();    
	RightSpeed = Speed_Right_Get();

    LeftPWM = PID_Calculate(&Left_PID, LeftSpeed);  
	RightPWM = PID_Calculate(&Right_PID, RightSpeed);

    Motor_Left_SetSpeed(LeftPWM);  
	Motor_Right_SetSpeed(RightPWM);
}
// 丢线或观察模式下，清除速度环积分并直接停止电机
void SpeedControl_Stop(void)
{
    PID_Reset(&Left_PID);
    PID_Reset(&Right_PID);
    Motor_Left_SetSpeed(0);
    Motor_Right_SetSpeed(0);
}

uint8_t SpeedControl_SetGains(float Kp, float Ki)
{
    uint32_t Primask;

    if (!(Kp >= 0.0f && Kp <= 1.0f &&
          Ki >= 0.0f && Ki <= 0.1f))
    {
        return 0;
    }

    Primask = __get_PRIMASK();
    __disable_irq();
    Left_PID.Kp = Kp;
    Left_PID.Ki = Ki;
    Right_PID.Kp = Kp;
    Right_PID.Ki = Ki;
    __set_PRIMASK(Primask);
    return 1;
}
