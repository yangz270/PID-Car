#include "LineFollow.h"
#include "PID.h"

static PID_TypeDef Line_PID;
volatile LineFollow_StateTypeDef LineFollow_State;

void LineFollow_Init(void)
{
    // 先只启用 P；巡线的增益与轮速 PI 的增益含义不同
    PID_Init(&Line_PID, LINE_KP, 0.0f, LINE_KD, 0.0f, LINE_CORRECTION_LIMIT);

    LineFollow_State.BlackState = 0;
    LineFollow_State.Valid = 0;
    LineFollow_State.Position = 0;
    LineFollow_State.Correction = 0;
    LineFollow_State.LeftTarget = 0;
    LineFollow_State.RightTarget = 0;
}

uint8_t LineFollow_Update(uint8_t RawState)
{
    // 已确认：黑线输出 1，白底输出 0；OUT1 在左，OUT8 在右
    static const int8_t Weight[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
    uint8_t BlackState;
    uint8_t Count = 0;
    uint8_t i;
    int16_t Sum = 0;
    float Position;
    float Correction;

    BlackState = RawState;
    LineFollow_State.BlackState = BlackState;

    for (i = 0; i < 8; i++)
    {
        if (BlackState & (1U << i))
        {
            Sum += Weight[i];
            Count++;
        }
    }    
	LineFollow_State.BlackCount = Count;
    // ponytail: 普通黑线巡线；丢线或全黑直接停止，路口识别以后再加
    if (BlackState == 0 || BlackState == 0xFF)
    {
        PID_Reset(&Line_PID);
        LineFollow_State.Valid = 0;
        LineFollow_State.Position = 0;
        LineFollow_State.Correction = 0;
        LineFollow_State.LeftTarget = 0;
        LineFollow_State.RightTarget = 0;
        return 0;
    }


    Position = (float)Sum / Count;
    // PID 目标为中心位置 0：线在右侧时，PID 输出为负
    Correction = PID_Calculate(&Line_PID, Position);


	LineFollow_State.Position = Position;
    LineFollow_State.Correction = Correction;
    LineFollow_State.LeftTarget = LINE_BASE_SPEED - Correction;
    LineFollow_State.RightTarget = LINE_BASE_SPEED + Correction;
    LineFollow_State.Valid = 1;
    return 1;
}
