#include "LineFollow.h"
#include "PID.h"

static PID_TypeDef Line_PID;
static float LastCorrection;
static float LastPosition;
static int8_t LastDirection;
static uint16_t DirectionAge;
static uint8_t HadLine;

volatile LineFollow_StateTypeDef LineFollow_State;

// 两轮只改变速度差，目标平均值始终为 LINE_BASE_SPEED
static void LineFollow_SetCorrection(float Correction)
{
    float Change = Correction - LastCorrection;

    if (Change > LINE_CORRECTION_STEP)
    {
        Change = LINE_CORRECTION_STEP;
    }
    else if (Change < -LINE_CORRECTION_STEP)
    {
        Change = -LINE_CORRECTION_STEP;
    }

    LastCorrection += Change;
    LineFollow_State.Correction = LastCorrection;
    LineFollow_State.LeftTarget = LINE_BASE_SPEED - LastCorrection;
    LineFollow_State.RightTarget = LINE_BASE_SPEED + LastCorrection;
}

static void LineFollow_Stop(void)
{
    PID_Reset(&Line_PID);
    LastCorrection = 0;
    LineFollow_State.Correction = 0;
    LineFollow_State.LeftTarget = 0;
    LineFollow_State.RightTarget = 0;
    LineFollow_State.Mode = LINE_MODE_STOP;
}

void LineFollow_Init(void)
{
    PID_Init(&Line_PID, LINE_KP, 0.0f, LINE_KD, 0.0f,
             LINE_CORRECTION_LIMIT);

    LastCorrection = 0;
    LastPosition = 0;
    LastDirection = 0;
    DirectionAge = LINE_DIRECTION_MEMORY_MS / LINE_CONTROL_PERIOD_MS;
    HadLine = 0;

    LineFollow_State.BlackState = 0;
    LineFollow_State.BlackCount = 0;
    LineFollow_State.Valid = 0;
    LineFollow_State.Position = 0;
    LineFollow_State.Correction = 0;
    LineFollow_State.LeftTarget = 0;
    LineFollow_State.RightTarget = 0;
    LineFollow_State.Mode = LINE_MODE_STOP;
    LineFollow_State.LostTicks = 0;
    LineFollow_State.LostEvents = 0;
}

uint8_t LineFollow_Update(uint8_t RawState)
{
    static const int8_t Weight[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
    uint8_t Count = 0;
    uint8_t i;
    int16_t Sum = 0;
    float Position;
    float Correction;

    LineFollow_State.BlackState = RawState;
    for (i = 0; i < 8; i++)
    {
        if (RawState & (1U << i))
        {
            Sum += Weight[i];
            Count++;
        }
    }
    LineFollow_State.BlackCount = Count;
    LineFollow_State.Valid = (Count > 0 && Count < 8);

    // 全黑没有唯一的转向方向；当前版本按停车处理
    if (Count == 8)
    {
        HadLine = 0;
        LastDirection = 0;
        LastPosition = 0;
        DirectionAge = LINE_DIRECTION_MEMORY_MS / LINE_CONTROL_PERIOD_MS;
        LineFollow_State.Position = 0;
        LineFollow_State.LostTicks = 0;
        LineFollow_Stop();
        return 0;
    }

    if (Count == 0)
    {
        if (HadLine)
        {
            LineFollow_State.LostEvents++;
            HadLine = 0;
        }

        if (LineFollow_State.LostTicks <
            LINE_SEARCH_TIMEOUT_MS / LINE_CONTROL_PERIOD_MS)
        {
            LineFollow_State.LostTicks++;
        }
        LineFollow_State.Position = LastPosition;

        // 启动时没有方向、方向记忆过旧、找线超时，都要求停车
        if (LastDirection == 0 ||
            DirectionAge >= LINE_DIRECTION_MEMORY_MS / LINE_CONTROL_PERIOD_MS ||
            LineFollow_State.LostTicks >=
            LINE_SEARCH_TIMEOUT_MS / LINE_CONTROL_PERIOD_MS)
        {
            LineFollow_Stop();
            return 0;
        }

        // 黑线最后在右边：左轮加速、右轮减速，继续向右找线
        Correction = -LastDirection * LINE_SEARCH_CORRECTION;
        LineFollow_SetCorrection(Correction);
        LineFollow_State.Mode = LINE_MODE_SEARCH;
        return 1;
    }

    Position = (float)Sum / Count;

    // 上电首次压线、丢线后重见黑线：同步历史误差，避免 D 突跳
    if (!HadLine)
    {
        PID_Reset(&Line_PID);
        Line_PID.LastError = -Position;
    }
    HadLine = 1;
    LineFollow_State.LostTicks = 0;

    // 只记最近的偏离方向，避免沿很久以前的弯道方向盲目找线
    if (Position > 0.5f)
    {
        LastDirection = 1;
        DirectionAge = 0;
    }
    else if (Position < -0.5f)
    {
        LastDirection = -1;
        DirectionAge = 0;
    }
    else if (DirectionAge < LINE_DIRECTION_MEMORY_MS / LINE_CONTROL_PERIOD_MS)
    {
        DirectionAge++;
    }

    LastPosition = Position;
    LineFollow_State.Position = Position;
    Correction = PID_Calculate(&Line_PID, Position);
    LineFollow_SetCorrection(Correction);
    LineFollow_State.Mode = LINE_MODE_TRACK;
    return 1;
}
