#ifndef __SPEEDCONTROL_H
#define __SPEEDCONTROL_H

void SpeedControl_Init(void);				//初始化
void SpeedControl_Update(void);				//固定周期执行
void SpeedControl_SetTarget(float LeftTarget, float RightTarget);
void SpeedControl_Stop(void);
uint8_t SpeedControl_SetGains(float Kp, float Ki);


#endif
