#include "stm32f10x.h"
#include "USART.h"
#include <stdio.h>

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ① 开时钟：GPIOA 在 APB2，USART1 也在 APB2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    /* ② PA9 = TX，复用推挽输出 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ③ PA10 = RX，浮空输入（或上拉输入）*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ④ 串口参数：115200, 8 位数据, 无校验, 1 位停止 */
    USART_InitStructure.USART_BaudRate            = 9600; 						// 波特率：9600 bit/s
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 			// 字长：8 位（当前无校验，即 8 位数据）
    USART_InitStructure.USART_StopBits            = USART_StopBits_1; 				// 停止位：1 位
    USART_InitStructure.USART_Parity              = USART_Parity_No; 				// 不使用奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用 RTS/CTS 硬件流控
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx; 	// 同时启用发送和接收
    USART_Init(USART1, &USART_InitStructure);									 	// 将以上配置写入 USART1

    USART_Cmd(USART1, ENABLE);
}

/* ============ printf 的"出口"在这里 ============ */
/* printf 每输出一个字符，就调用一次这个函数 */
int fputc(int ch, FILE *f)															//FILE *f 的作用就是：告诉输出函数：这个字符应该送到哪个输出流。我们自己规定：不管 f 指向哪里，只要 printf 调用 fputc，就把字符发送到 USART1。
{
    USART_SendData(USART1, (uint8_t)ch);											//fputc的意思是：输出一个字符，(uint8_t)ch强制转换为8位无符号，并且只保留第八位

    /* 等发送完成（等 TXE 标志置位）*/
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);					//TXE:发送数据寄存器为空标志，，当为0使，表示还有数据继续循环

    return ch;
}
