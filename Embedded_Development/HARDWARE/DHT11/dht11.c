#include "dht11.h"
#include "delay.h"
/*
	����������д 2022-04-23
	���ߣ�Jianxin Wu
	�������裺GPIO����DHT11 PA2
*/
DHT11_Data_TypeDef DHT11_Data;
void DHT11_Mode_OUT_PP(void)   // ����PA2Ϊ���ģʽ
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	 // GPIO���ù���
	 // ��һ��������ʱ��
	 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 // �ڶ��������ṹ���Ա��ֵ
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // �������
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // �������ģʽ
	
	 // ����������ʼ�� GPIOA
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 // ���Ĳ������ܽ�һ����ʼ��״̬
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
		
		if(DHT11_IN == 1)   // ��ζ�Ž��յı���Ϊ1       0000 0000 | 1000 0000
		{
			while(DHT11_IN == 1);
			temp |= (unsigned char) (0x01 << (7-i));
		}
		else                // ��ζ�Ž��յı���Ϊ0
		{
			temp &= (unsigned char)~(0x01 << (7-i));
		}
	}
	return temp;
}
	


void DHT11_Mode_IN_NP(void)    // ����PA2Ϊ����ģʽ
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // �������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char DHT11_ReadData(DHT11_Data_TypeDef *DHT11_AData)
{
	DHT11_Mode_OUT_PP();  // ������ʼ����
	
	DHT11_OUT_0;
	delay_ms(22);   // ��ʱ����18ms
	DHT11_OUT_1;
	delay_us(30);
	
	// ׼����ȡ����
	DHT11_Mode_IN_NP();
	if(DHT11_IN == 0)
	{
		while(DHT11_IN == 0);   // ����while�������� PA2 ��ɸߵ�ƽ
			// �˴�Ӧ����ǿ���˳���������ѭ��
	
		while(DHT11_IN == 1);   // ����while�������� PA2 ��ɵ͵�ƽ
		
		// ��ȡ����
		DHT11_AData->humi_int  = DHT11_ReadByte();
		DHT11_AData->humi_deci = DHT11_ReadByte();
		DHT11_AData->temp_int  = DHT11_ReadByte();
		DHT11_AData->temp_deci = DHT11_ReadByte();
		DHT11_AData->check_sum = DHT11_ReadByte();
		
		DHT11_Mode_OUT_PP();
		DHT11_OUT_1;
		
		// ��һ��У��
		
		if(DHT11_AData->check_sum == (DHT11_AData->humi_int+DHT11_AData->humi_deci+DHT11_AData->temp_int+DHT11_AData->temp_deci))
		{
			// ���ݽ�����ȷ
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

