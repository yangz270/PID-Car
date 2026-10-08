#ifndef MOTOR_H
#define MOTOR_H
#define MOTOR_MAX_SPEED 100

void Motor_Init(void);
void Motor_Left_SetSpeed(int16_t Speed);			//Motor层负责：某个电机怎么转，需要设计成双电机接口
void Motor_Right_SetSpeed(int16_t Speed);	

#endif
