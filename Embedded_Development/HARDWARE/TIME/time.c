#include "time.h"
#include "beep.h"
#include "usart.h"
/*
	定时器中断驱动编写 2022-03-27
	作者：Jianxin Wu
	配置外设：TIM3
*/

void TIM3_Init(unsigned int arr, unsigned int psc)   // arr: 自动重装在寄存器数值; psc: 定时器预分频寄存器数值
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 第一步：启动时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// 第二步：结构体赋值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟72Mhz
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = arr;					 // 5000*0.1ms = 50ms
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;           // 72000K/(7200-1) = 10K; 1/10K = 0.1ms
	                                                         // 数值不能超过65535
  // 第三步：初始化TIM3
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 先占优先级，数越小优先级越高，高优先级可以打断低优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3; // 从优先级
	
	// 初始化中断
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3,DISABLE);
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// 清楚中断标志
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		// BEEP =! BEEP;
		// 意味着一帧数据已经接受完毕
		RecFlag = 1;
		RecDataCounter = 0;
		
		TIM_Cmd(TIM3, DISABLE);
		TIM_ARRPreloadConfig(TIM3,DISABLE);
		
	}
}












