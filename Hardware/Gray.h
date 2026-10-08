#ifndef __GRAY_H
#define __GRAY_H

void Gray_Init(void);							//负责开启GPIO时钟，配置8个输入引脚
uint8_t Gray_GetState(void);					//负责读取8路灰度状态

#endif
