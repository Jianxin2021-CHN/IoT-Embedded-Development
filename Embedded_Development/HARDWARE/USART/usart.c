# include "usart.h"

/*
	串口驱动编写 2022-04-03
	作者：Jianxin Wu
	配置外设：PA9 TX PA10 RX
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
	// 第一步：IO口的初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	// PA9 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                // 复用推挽输出
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA10 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         // 浮空输入
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*****************************************************/
	// 第二步：串口参数初始化
	USART_InitStructure.USART_BaudRate = bound;                   // 设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // 设置数据位长度为1位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        // 设置停止位为1位
	USART_InitStructure.USART_Parity = USART_Parity_No;           // 设置奇偶校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 设置硬件数据流控制 无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // 设置工作模式为收发模式
	
	USART_Init(USART1, &USART_InitStructure);
	
	/*****************************************************/
	// 第三步：串口接受中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 先占优先级，数越小优先级越高，高优先级可以打断低优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4; // 从优先级
	
	NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC寄存器
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);            // 开启接收中断
	USART_Cmd(USART1, ENABLE);                                // 串口使能

}

/*
	蓝牙编写 2022-05-04
	作者：Jianxin Wu
	配置外设 PA2 TX PA3 RX
*/
void UART2_Init(unsigned int bound)
{
	// 第一步：IO口的初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	// PA2 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                // 复用推挽输出
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA3 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         // 浮空输入
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*****************************************************/
	// 第二步：串口参数初始化
	USART_InitStructure.USART_BaudRate = bound;                   // 设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // 设置数据位长度为1位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        // 设置停止位为1位
	USART_InitStructure.USART_Parity = USART_Parity_No;           // 设置奇偶校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 设置硬件数据流控制 无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // 设置工作模式为收发模式
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);                                // 串口使能
}

// 建立协议 - ASCII 发送
// 名称    含义    数值
// ---------------------------------------------------------
// 帧头     A     
// 帧类型   1    1代表控制蜂鸣器，2代表读取温湿度
// 帧长度   7  
// 数据     1    Type - 1：代表蜂鸣器响，2：代表蜂鸣器不响
// 							 Type - 1: 查看温度，    2：查看湿度
// 数据2    3 5  待定
// 校验位   0 2  从帧头到数据2的数值相加，取低字节累加和校验
// 帧尾     A

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



// 蓝牙板块测试 - 串口中断
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
//			// 接受完毕
//			RecFlag = 1;
//		}
//		
//		
//	}
//}

// 蓝牙板块测试 - 定时器中断
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

