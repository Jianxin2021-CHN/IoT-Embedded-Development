#include "ad.h"

/*
	AD采集驱动编写 2022-04-23
	作者：Jianxin Wu
	配置外设：ADC1 CH1 PA1
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
	
	// AD外设的配置
	
	// 采样时钟 56M 4 14M
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // 72M/6  转换时间 - 采样时间 + 12.5 周期
	
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;     // 独立工作模式 区分双AD工作
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                   // 单通道可以是disable 但是多通道必须是ENABLE
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // 多次循环采集 区别单次采集
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;// 外部触发
	ADC_InitStructure.ADC_NbrOfChannel       = 1;                        // AD采集通道数
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;      // 右对齐
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 55.5 以1.5为例，转换时间 = 1.5 + 12.5 = 14 周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);   
	
	// DMA 通道配置
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((unsigned long int)(0x4001244C));        // 外设基地址 存储数据的位置 
	DMA_InitStructure.DMA_MemoryBaseAddr     = (unsigned long int)ADC_Con;               // 数据存储位置
	DMA_InitStructure.DMA_BufferSize         = 1;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;                    // 定义AD外设作为数据来源
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                          // 关闭内存到内存的存储
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;                        // 循环存储
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High;                        // 设置选定的DMA通道 优先级
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;              // 半字存储
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;          // 数据宽度
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;                // 寄存器地址固定
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;                     // 设置内存地址递增
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);     // 复位 ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);    // 启动转换
	
}

