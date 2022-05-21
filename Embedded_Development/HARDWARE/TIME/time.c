#include "time.h"
#include "beep.h"
#include "usart.h"
/*
	��ʱ���ж�������д 2022-03-27
	���ߣ�Jianxin Wu
	�������裺TIM3
*/

void TIM3_Init(unsigned int arr, unsigned int psc)   // arr: �Զ���װ�ڼĴ�����ֵ; psc: ��ʱ��Ԥ��Ƶ�Ĵ�����ֵ
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ��һ��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// �ڶ������ṹ�帳ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ʱ��72Mhz
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = arr;					 // 5000*0.1ms = 50ms
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;           // 72000K/(7200-1) = 10K; 1/10K = 0.1ms
	                                                         // ��ֵ���ܳ���65535
  // ����������ʼ��TIM3
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�����ԽС���ȼ�Խ�ߣ������ȼ����Դ�ϵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3; // �����ȼ�
	
	// ��ʼ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3,DISABLE);
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// ����жϱ�־
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		// BEEP =! BEEP;
		// ��ζ��һ֡�����Ѿ��������
		RecFlag = 1;
		RecDataCounter = 0;
		
		TIM_Cmd(TIM3, DISABLE);
		TIM_ARRPreloadConfig(TIM3,DISABLE);
		
	}
}












