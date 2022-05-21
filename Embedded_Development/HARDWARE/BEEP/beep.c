#include "beep.h"

/*
    ʵ��һ�� ������ 2022-03-27
		���ùܽţ� PB8
*/

void BEEP_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 // GPIO���ù���
	 // ��һ��������ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	 // �ڶ��������ṹ���Ա��ֵ
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;          // 9��
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // �������
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // �������ģʽ
	
	 // ����������ʼ�� GPIOB
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 // ���Ĳ������ܽ�һ����ʼ��״̬
	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
}





