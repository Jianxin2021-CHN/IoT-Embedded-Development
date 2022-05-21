#include "ad.h"

/*
	AD�ɼ�������д 2022-04-23
	���ߣ�Jianxin Wu
	�������裺ADC1 CH1 PA1
*/
unsigned int ADC_Con[1];

void ADC_MyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// AD���������
	
	// ����ʱ�� 56M 4 14M
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // 72M/6  ת��ʱ�� - ����ʱ�� + 12.5 ����
	
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;     // ��������ģʽ ����˫AD����
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                   // ��ͨ��������disable ���Ƕ�ͨ��������ENABLE
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // ���ѭ���ɼ� ���𵥴βɼ�
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;// �ⲿ����
	ADC_InitStructure.ADC_NbrOfChannel       = 1;                        // AD�ɼ�ͨ����
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;      // �Ҷ���
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 55.5 ��1.5Ϊ����ת��ʱ�� = 1.5 + 12.5 = 14 ����
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);   
	
	// DMA ͨ������
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((unsigned long int)(0x4001244C));        // �������ַ �洢���ݵ�λ�� 
	DMA_InitStructure.DMA_MemoryBaseAddr     = (unsigned long int)ADC_Con;               // ���ݴ洢λ��
	DMA_InitStructure.DMA_BufferSize         = 1;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;                    // ����AD������Ϊ������Դ
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                          // �ر��ڴ浽�ڴ�Ĵ洢
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;                        // ѭ���洢
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High;                        // ����ѡ����DMAͨ�� ���ȼ�
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;              // ���ִ洢
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;          // ���ݿ��
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;                // �Ĵ�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;                     // �����ڴ��ַ����
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);     // ��λ ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);    // ����ת��
	
}

