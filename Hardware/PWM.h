#ifndef PWM_H
#define PWM_H

void PWM_Init(void);						//调用完成PWM的初始化
void PWM_SetCompare1(uint16_t Compare);		//这个函数用来改变PWM的占空比
void PWM_SetCompare2(uint16_t Compare);		//PWM_H层负责：怎么产生PWM

#endif
