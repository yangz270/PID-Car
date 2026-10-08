#ifndef ENCODER_H
#define ENCODER_H

void Encoder_Left_Init(void);						//负责：初始化编码器硬件
void Encoder_Right_Init(void);
int16_t Encoder_Left_GetCount(void);				//负责：获取当前编码器的计数值
int16_t Encoder_Right_GetCount(void);

#endif
