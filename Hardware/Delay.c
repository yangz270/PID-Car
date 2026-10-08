#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Delay_ms(uint32_t ms)
{
	while(ms--)
	{
	SysTick ->LOAD = SystemCoreClock / 1000 - 1;		//设置系统时钟周期
	SysTick ->VAL = 0;									//将当前计数器清零
	SysTick ->CTRL = 5;									//开启时钟并使用处理器作为时钟源
	
	while((SysTick ->CTRL & 0x00010000) == 0);			//读取CTRL控制寄存器，十六进制表示方法并只关心第十六位（按位与），这一位是count flag标志位，只要他还是0，就一直等，知道变成1，代表计时结束
	
	SysTick ->CTRL = 0;									//关闭系统时钟
	}
}
