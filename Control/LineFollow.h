#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

#include "stm32f10x.h"

// 0：架空观察目标和找线状态；确认方向后设为 1
#define LINE_RUN_ENABLE 1

// 正常巡线和短时找线：左右目标的平均值固定为 800
// 单位：每个 10 ms 采样周期的编码器计数，不是 PWM 或 RPM
#define LINE_BASE_SPEED 800.0f
#define LINE_CORRECTION_LIMIT 800.0f
#define LINE_KP 110.0f
#define LINE_KD 20.0f

// 每个 10 ms 周期，修正量最多变化 100，减小目标速度突跳
#define LINE_CORRECTION_STEP 100.0f

// 找线时修正量为 900：两轮目标为 1700 和 -100
// 允许内侧轮短暂反转；实际能否达到目标，需要在地面上验证
#define LINE_SEARCH_CORRECTION 900.0f
#define LINE_SEARCH_TIMEOUT_MS 300U
#define LINE_DIRECTION_MEMORY_MS 200U
#define LINE_CONTROL_PERIOD_MS 10U

// Mode：0 停车；1 正常巡线；2 丢线后按最近方向找线
#define LINE_MODE_STOP 0U
#define LINE_MODE_TRACK 1U
#define LINE_MODE_SEARCH 2U

typedef struct
{
    uint8_t BlackState;  // OUT1 在左，OUT8 在右；1 表示黑线
    uint8_t BlackCount;
    uint8_t Valid;       // 当前确实检测到黑线为 1；找线时为 0
    float Position;      // 左负右正；找线时保留最后有效位置
    float Correction;
    float LeftTarget;
    float RightTarget;
    uint8_t Mode;
    uint16_t LostTicks;  // 当前连续全白的周期数，每次 10 ms
    uint32_t LostEvents; // 从有效黑线变成全白的累计次数
} LineFollow_StateTypeDef;

extern volatile LineFollow_StateTypeDef LineFollow_State;

void LineFollow_Init(void);
// 返回 1：有可执行的轮速目标（含找线）；返回 0：要求停车
uint8_t LineFollow_Update(uint8_t RawState);

#endif
