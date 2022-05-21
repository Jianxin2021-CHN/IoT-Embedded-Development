#include "beep.h"

/*
    实验一： 蜂鸣器 2022-03-27
		配置管脚： PB8
*/

void BEEP_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 // GPIO配置过程
	 // 第一步：开启时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	 // 第二步：给结构体成员赋值
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;          // 9腿
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速率
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出模式
	
	 // 第三步：初始化 GPIOB
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 // 第四步：给管脚一个初始化状态
	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
}





