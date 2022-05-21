#ifndef __DHT11_H
#define __DHT11_H
#include "sys.h"

#define DHT11_OUT_1 GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define DHT11_OUT_0 GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define DHT11_IN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

typedef struct
{
	unsigned char humi_int;
	unsigned char humi_deci;
	unsigned char temp_int;
	unsigned char temp_deci;
	unsigned char check_sum;
	
}DHT11_Data_TypeDef;
extern DHT11_Data_TypeDef DHT11_Data;
unsigned char DHT11_ReadData(DHT11_Data_TypeDef *DHT11_AData);

#endif

