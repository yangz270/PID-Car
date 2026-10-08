#ifndef PID_H
#define PID_H

typedef struct									//本来这里是要给struct一个名字“PID”，但是这里我们要创建匿名结构体，因为创建了也无意义，我们已经为这个结构体取了新的名字PID_TypeDef
{
    float Kp;									//格式：取别名 创建结构体 {数据成员} 结构体的别名
    float Ki;
    float Kd;

    float Target;
    float Actual;

    float Error;
    float LastError;

    float Integral;

    float Output;
	
	float IntegralLimit;
	float OutputLimit;

} PID_TypeDef;									//放在头文件里，因为别的文件也需要调用


void PID_Init(PID_TypeDef *PID, float Kp, float Ki, float Kd, float IntegralLimit, float OutputLimit);			//初始化设置固定参数
float PID_Calculate(PID_TypeDef *PID, float Actual);					//计算目标速度，需要向这个函数里面引入实际速度，第一个float表示返回一个float值
void PID_Reset(PID_TypeDef *PID);

#endif
