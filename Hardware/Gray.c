#include "stm32f10x.h"
#include "Gray.h"

void Gray_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 模块主动输出数字电平，STM32 配置为输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // OUT1~OUT6：PB8、PB9、PB12、PB13、PB14、PB15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // OUT7、OUT8：PA8、PA11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Gray_GetState(void)
{
    uint16_t PortA;
    uint16_t PortB;
    uint8_t State = 0;

    // 一次读取每个端口，保留原始电平：高为 1，低为 0
    PortA = GPIO_ReadInputData(GPIOA);
    PortB = GPIO_ReadInputData(GPIOB);

    if (PortB & GPIO_Pin_8)  
		State |= 0x01; // OUT1 -> bit0
    if (PortB & GPIO_Pin_9)  
		State |= 0x02; // OUT2 -> bit1
    if (PortB & GPIO_Pin_12) 
		State |= 0x04; // OUT3 -> bit2
    if (PortB & GPIO_Pin_13) 
		State |= 0x08; // OUT4 -> bit3
    if (PortB & GPIO_Pin_14) 
		State |= 0x10; // OUT5 -> bit4
    if (PortB & GPIO_Pin_15) 
		State |= 0x20; // OUT6 -> bit5
    if (PortA & GPIO_Pin_8)  
		State |= 0x40; // OUT7 -> bit6
    if (PortA & GPIO_Pin_11) 
		State |= 0x80; // OUT8 -> bit7

    return State;
}
