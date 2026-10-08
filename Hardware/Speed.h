#ifndef SPEED_H
#define SPEED_H

void Speed_Init(void);						//初始化测速模块
void Speed_Update(void);					//每更新一次速度值的频率，初始化更新模块
float Speed_Left_Get(void);					//获取并返回当前速度
float Speed_Right_Get(void);

#endif
