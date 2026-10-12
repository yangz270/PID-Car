#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Delay.h"
#include "Motor.h"
#include "Car.h"
#include "Timer.h"
#include "Speed.h"
#include "SpeedControl.h"
#include "USART.h"
#include "stdio.h"
#include "PID.h"
#include "Gray.h"
#include "LineFollow.h"
#include "Bluetooth.h"

//volatile int16_t test_LeftCount  = 0;
//volatile int16_t test_RightCount = 0;
//static volatile float   test_LeftSpeed  = 0;
//static volatile float   test_RightSpeed = 0;
//	uint8_t State;
//    uint8_t i;

int main(void)
{	
	LineFollow_StateTypeDef Sample;
    int LeftSpeed;
    int RightSpeed;
    uint32_t Primask;
	uint8_t BluetoothTicks = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//	MyGPIO_Init();
	Motor_Init();

    Encoder_Left_Init();
	Encoder_Right_Init();

    Speed_Init();
	USART1_Init();
	Gray_Init();
	SpeedControl_Init();
    LineFollow_Init();
//	SpeedControl_SetTarget(1000, 1000);
	Bluetooth_Init();
    Timer_Init();						//TIMer要放在最后的原因是，初始化后就会开启中断的周期，可以避免Speed还没初始化好，中断就开启了，导致数据异常

	while (1)

	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_0);
//		Delay_ms(500);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//		Delay_ms(500);
		
//		Motor_SetSpeed(50);
//		Delay_ms(1000);
//		
//		Motor_SetSpeed(0);
//		Delay_ms(1000);
//		
//		Motor_SetSpeed(-50);
//		Delay_ms(1000);
//		
//		Motor_SetSpeed(0);
//		Delay_ms(1000); 
//		Car_Forward(50);
//		Delay_ms(2000);
//		
//		Car_Stop();
//		Delay_ms(1000);
//		
//		Car_Backward(50);
//		Delay_ms(2000);
//		
//		Car_Stop();
//		Delay_ms(1000);
//		
//		Car_TurnLeft(50);
//		Delay_ms(2000);
//		
//		Car_Stop();
//		Delay_ms(1000);
//		
//		Car_TurnRight(50);
//		Delay_ms(2000);
//		
//		Car_Stop();
//		Delay_ms(1000);
//		Encoder_Left_GetCount();
//		Encoder_Right_GetCount();

//		Motor_Right_SetSpeed(60);

//		Delay_ms(1000);

//		test_LeftCount  = Encoder_Left_GetCount();
//		test_RightCount = Encoder_Right_GetCount();

//		Motor_Right_SetSpeed(0);

//		Delay_ms(1000);

//		printf("[L] L=%d R(dragged)=%d\r\n", test_LeftCount, test_RightCount);
//		Car_Forward(60);
//		Delay_ms(4000);
//		
//		test_LeftSpeed  = Speed_Left_Get();
//		test_RightSpeed = Speed_Right_Get();

//// 当前单位是“计数/10 ms”，先按整数打印
//		printf("%d,%d\r\n", (int)test_LeftSpeed, (int)test_RightSpeed);
//		
//		Car_Backward(60);
//		Delay_ms(4000);
//		
//		Car_TurnLeft(60);
//		Delay_ms(4000);
//		
//		Car_TurnRight(60);
//		Delay_ms(4000);
//	
//		Car_Stop();
//		Delay_ms(4000);
//		printf("%d,%d,%d\r\n", 1000, (int)Speed_Left_Get(), (int)Speed_Right_Get());
//		Delay_ms(50);
// 读取八路原始电平，每一位对应一路传感器
//        State = Gray_GetState();

//        // 按 OUT1～OUT8 的顺序发送八列数据
//        for (i = 0; i < 8; i++)
//        {
//            printf("%u", (unsigned int)((State >> i) & 1U));			//1U表示无符号的1，其实这里的强制转换可以省略

//            if (i < 7)
//            {
//                printf(",");
//            }
//            else
//            {
//                printf("\r\n");
//            }
//        }

//        Delay_ms(50);
// 短暂屏蔽中断，取得同一次控制周期的数据快照
		Bluetooth_Process();
		Primask = __get_PRIMASK();
		__disable_irq();
        Sample = LineFollow_State;
        LeftSpeed = (int)Speed_Left_Get();
        RightSpeed = (int)Speed_Right_Get();
        __set_PRIMASK(Primask);

		if (++BluetoothTicks >= 4)
{
    BluetoothTicks = 0;
    Bluetooth_SendTelemetry(Sample.Position, Sample.LeftTarget,
                            Sample.RightTarget, (float)LeftSpeed,
                            (float)RightSpeed);
}
        Delay_ms(50);  // 串口显示周期，不是 PID 控制周期
    
	}
}
