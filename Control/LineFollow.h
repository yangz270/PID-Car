#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

#include "stm32f10x.h"

// 0：只观察偏差和目标；装好传感器、确认方向后改为 1
#define LINE_RUN_ENABLE 1

// 单位沿用现有速度环：每次固定采样周期的编码器计数
#define LINE_BASE_SPEED 600.0f
#define LINE_CORRECTION_LIMIT 400.0f

// 巡线外环先调 P，出现左右摆动后再试 D；保持原控制周期
#define LINE_KP 45.0f
#define LINE_KD 0.0f

typedef struct
{
    uint8_t BlackState;  // 归一化后，1 表示检测到黑线
    uint8_t BlackCount;  // 0～8；实际压线数量，用于检查高度和线宽
    uint8_t Valid;       // 全白或全黑时为 0
    float Position;      // 黑线位置：左负、右正，范围 -7～7
    float Correction;    // PID 输出；左目标=基础速度-输出
    float LeftTarget;
    float RightTarget;
} LineFollow_StateTypeDef;

extern volatile LineFollow_StateTypeDef LineFollow_State;

void LineFollow_Init(void);
uint8_t LineFollow_Update(uint8_t RawState);

#endif
