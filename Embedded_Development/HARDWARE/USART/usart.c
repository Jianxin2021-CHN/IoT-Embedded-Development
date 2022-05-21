# include "usart.h"

/*
	����������д 2022-04-03
	���ߣ�Jianxin Wu
	�������裺PA9 TX PA10 RX
*/

unsigned char RecFlag = 0;
unsigned char RecDataCounter = 0;

int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(unsigned char) ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET) 
	{}
	return ch;
} 

Uart_Inf Uart1_Inf;

void UART_Init(unsigned int bound)
{
	// ��һ����IO�ڵĳ�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	// PA9 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                // �����������
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA10 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         // ��������
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*****************************************************/
	// �ڶ��������ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = bound;                   // ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // ��������λ����Ϊ1λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        // ����ֹͣλΪ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;           // ������żУ��λ ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // ����Ӳ������������ ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // ���ù���ģʽΪ�շ�ģʽ
	
	USART_Init(USART1, &USART_InitStructure);
	
	/*****************************************************/
	// �����������ڽ����жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�����ԽС���ȼ�Խ�ߣ������ȼ����Դ�ϵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4; // �����ȼ�
	
	NVIC_Init(&NVIC_InitStructure);                           // ��ʼ��NVIC�Ĵ���
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);            // ���������ж�
	USART_Cmd(USART1, ENABLE);                                // ����ʹ��

}

/*
	������д 2022-05-04
	���ߣ�Jianxin Wu
	�������� PA2 TX PA3 RX
*/
void UART2_Init(unsigned int bound)
{
	// ��һ����IO�ڵĳ�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	// PA2 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                // �����������
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA3 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         // ��������
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*****************************************************/
	// �ڶ��������ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = bound;                   // ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // ��������λ����Ϊ1λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        // ����ֹͣλΪ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;           // ������żУ��λ ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // ����Ӳ������������ ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // ���ù���ģʽΪ�շ�ģʽ
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);                                // ����ʹ��
}

// ����Э�� - ASCII ����
// ����    ����    ��ֵ
// ---------------------------------------------------------
// ֡ͷ     A     
// ֡����   1    1������Ʒ�������2�����ȡ��ʪ��
// ֡����   7  
// ����     1    Type - 1������������죬2���������������
// 							 Type - 1: �鿴�¶ȣ�    2���鿴ʪ��
// ����2    3 5  ����
// У��λ   0 2  ��֡ͷ������2����ֵ��ӣ�ȡ���ֽ��ۼӺ�У��
// ֡β     A

void USART1_IRQHandler(void)
{
	
	TIM_Cmd(TIM3, DISABLE);
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		Uart1_Inf.UartRec_Data[RecDataCounter++] = USART_ReceiveData(USART1);
		if(RecDataCounter== 0x07)
		{
			RecDataCounter = 0;
		}
		
	}
	TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ARRPreloadConfig(TIM3,DISABLE);
}



// ���������� - �����ж�
//unsigned char bluetoothdata[4];

//void USART1_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		bluetoothdata[RecDataCounter++] = USART_ReceiveData(USART1);
//		if(RecDataCounter== 0x04)
//		{
//			RecDataCounter = 0;
//			
//			// �������
//			RecFlag = 1;
//		}
//		
//		
//	}
//}

// ���������� - ��ʱ���ж�
//unsigned char bluetoothdata[4];

//void USART1_IRQHandler(void)
//{
//	TIM_Cmd(TIM3, DISABLE);
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		bluetoothdata[RecDataCounter++] = USART_ReceiveData(USART1);
//		if(RecDataCounter== 0x04)
//		{
//			RecDataCounter = 0;
//		}
//		
//		TIM_SetCounter(TIM3,0);
//    TIM_Cmd(TIM3, ENABLE);
//    TIM_ARRPreloadConfig(TIM3,DISABLE);
//	}
//}

