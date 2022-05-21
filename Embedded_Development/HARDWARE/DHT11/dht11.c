#include "dht11.h"
#include "delay.h"
/*
	测温驱动编写 2022-04-23
	作者：Jianxin Wu
	配置外设：GPIO用于DHT11 PA2
*/
DHT11_Data_TypeDef DHT11_Data;
void DHT11_Mode_OUT_PP(void)   // 配置PA2为输出模式
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	 // GPIO配置过程
	 // 第一步：开启时钟
	 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 // 第二步：给结构体成员赋值
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速率
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出模式
	
	 // 第三步：初始化 GPIOA
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 // 第四步：给管脚一个初始化状态
	 //GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	
}

unsigned char DHT11_ReadByte(void)
{
	unsigned i = 0;
	unsigned temp = 0;
	for(i=0;i<8;i++)
	{
		while(DHT11_IN == 0);  
		delay_us(40);
		
		if(DHT11_IN == 1)   // 意味着接收的比特为1       0000 0000 | 1000 0000
		{
			while(DHT11_IN == 1);
			temp |= (unsigned char) (0x01 << (7-i));
		}
		else                // 意味着接收的比特为0
		{
			temp &= (unsigned char)~(0x01 << (7-i));
		}
	}
	return temp;
}
	


void DHT11_Mode_IN_NP(void)    // 配置PA2为输入模式
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 推挽输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char DHT11_ReadData(DHT11_Data_TypeDef *DHT11_AData)
{
	DHT11_Mode_OUT_PP();  // 发送起始命令
	
	DHT11_OUT_0;
	delay_ms(22);   // 延时至少18ms
	DHT11_OUT_1;
	delay_us(30);
	
	// 准备读取数据
	DHT11_Mode_IN_NP();
	if(DHT11_IN == 0)
	{
		while(DHT11_IN == 0);   // 跳出while的条件是 PA2 变成高电平
			// 此处应增加强制退出，避免死循环
	
		while(DHT11_IN == 1);   // 跳出while的条件是 PA2 变成低电平
		
		// 读取数据
		DHT11_AData->humi_int  = DHT11_ReadByte();
		DHT11_AData->humi_deci = DHT11_ReadByte();
		DHT11_AData->temp_int  = DHT11_ReadByte();
		DHT11_AData->temp_deci = DHT11_ReadByte();
		DHT11_AData->check_sum = DHT11_ReadByte();
		
		DHT11_Mode_OUT_PP();
		DHT11_OUT_1;
		
		// 做一次校验
		
		if(DHT11_AData->check_sum == (DHT11_AData->humi_int+DHT11_AData->humi_deci+DHT11_AData->temp_int+DHT11_AData->temp_deci))
		{
			// 数据接收正确
			return 1;
		}
		else{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	
}

