#include "Bluetooth.h"
#include "LineFollow.h"
#include "SpeedControl.h"
#include <stdio.h>
#include <string.h>

#define BLUETOOTH_RX_SIZE 64U

static volatile char RxBuffer[BLUETOOTH_RX_SIZE];
static volatile uint8_t RxLength;
static volatile uint8_t RxReady;
static volatile uint8_t RxDiscard;
static volatile uint8_t RxError;

void Bluetooth_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* main 先调用 USART1_Init()，复用 PA9/PA10、9600/8N1 配置。 */
    RxLength = 0;
    RxReady = 0;
    RxDiscard = 0;
    RxError = 0;

    /* main 先配置 PriorityGroup_2；TIM1 抢占优先级为 1。 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void Bluetooth_SendString(const char *Text)
{
    /* 原有 fputc 已把 printf 重定向到 USART1。 */
    printf("%s", Text);
}

void USART1_IRQHandler(void)
{
    uint16_t Status;
    uint8_t Data;

    Status = USART1->SR;
    if (!(Status & (USART_SR_RXNE | USART_SR_ORE |
                    USART_SR_FE | USART_SR_NE | USART_SR_PE)))
    {
        return;
    }

    /* 先读 SR、再读 DR，清除 RXNE 及串口错误标志。 */
    Data = (uint8_t)USART1->DR;
    if (Status & (USART_SR_ORE | USART_SR_FE |
                  USART_SR_NE | USART_SR_PE))
    {
        RxLength = 0;
        RxDiscard = (Data != '\n');
        RxError = 1;
        return;
    }

    /* ponytail: 单条命令邮箱；上位机等待回复再发下一条，连续流需改环形队列。 */
    if (RxReady)
    {
        if (Data != '\r')
        {
            RxDiscard = (Data != '\n');
        }
        RxError = 1;
        return;
    }
    if (RxDiscard)
    {
        if (Data == '\n')
        {
            RxDiscard = 0;
            RxLength = 0;
        }
        return;
    }
    if (Data == '\r')
    {
        return;
    }
    if (Data == '\n')
    {
        if (RxLength > 0)
        {
            RxBuffer[RxLength] = '\0';
            RxReady = 1;
            RxLength = 0;
        }
        return;
    }
    if (Data < 0x20U || Data > 0x7EU ||
        RxLength >= BLUETOOTH_RX_SIZE - 1U)
    {
        RxLength = 0;
        RxDiscard = 1;
        RxError = 1;
        return;
    }

    RxBuffer[RxLength++] = (char)Data;
}

void Bluetooth_Process(void)
{
    char Line[BLUETOOTH_RX_SIZE];
    char Command[8];
    char Extra;
    float Kp;
    float SecondGain;
    uint32_t Primask;
    uint8_t Ready;
    uint8_t Error;
    uint8_t Accepted;
    uint8_t i;

    /* 只在复制邮箱时短暂关中断，解析及发送都在恢复中断后进行。 */
    Primask = __get_PRIMASK();
    __disable_irq();
    Ready = RxReady;
    Error = RxError;
    RxError = 0;
    if (Ready)
    {
        for (i = 0; i < BLUETOOTH_RX_SIZE; i++)
        {
            Line[i] = RxBuffer[i];
            if (Line[i] == '\0')
            {
                break;
            }
        }
        RxReady = 0;
    }
    __set_PRIMASK(Primask);

    if (Error)
    {
        Bluetooth_SendString("ERR,RX\r\n");
    }
    if (!Ready)
    {
        return;
    }
    if (strcmp(Line, "PING") == 0)
    {
        Bluetooth_SendString("PONG\r\n");
        return;
    }

    /* LKP、LKD 只带一个数值；%c 检出多余尾巴。 */
    if (sscanf(Line, "%7[^,],%f%c", Command, &Kp, &Extra) == 2 &&
        (strcmp(Command, "LKP") == 0 || strcmp(Command, "LKD") == 0))
    {
        if (strcmp(Command, "LKP") == 0)
        {
            Accepted = LineFollow_SetKp(Kp);
        }
        else
        {
            Accepted = LineFollow_SetKd(Kp);
        }
    }
    else
    {
        /* 保留原 LINE、SPEED 命令：同时更新一对参数。 */
        if (sscanf(Line, "%7[^,],%f,%f%c", Command,
                   &Kp, &SecondGain, &Extra) != 3)
        {
            Bluetooth_SendString("ERR,FORMAT\r\n");
            return;
        }
        if (strcmp(Command, "LINE") == 0)
        {
            Accepted = LineFollow_SetGains(Kp, SecondGain);
        }
        else if (strcmp(Command, "SPEED") == 0)
        {
            Accepted = SpeedControl_SetGains(Kp, SecondGain);
        }
        else
        {
            Bluetooth_SendString("ERR,COMMAND\r\n");
            return;
        }
    }
    if (!Accepted)
    {
        Bluetooth_SendString("ERR,RANGE\r\n");
        return;
    }

    Bluetooth_SendString("OK,");
    Bluetooth_SendString(Line);
    Bluetooth_SendString("\r\n");
}

void Bluetooth_SendTelemetry(float Position, float LeftTarget,
                             float RightTarget, float LeftSpeed,
                             float RightSpeed)
{
    char Text[80];

    snprintf(Text, sizeof(Text), "bt:%.2f,%.0f,%.0f,%.0f,%.0f\r\n",
             (double)Position, (double)LeftTarget, (double)RightTarget,
             (double)LeftSpeed, (double)RightSpeed);
    Bluetooth_SendString(Text);
}
