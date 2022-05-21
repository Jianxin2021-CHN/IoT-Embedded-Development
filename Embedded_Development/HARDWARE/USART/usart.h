# ifndef __USART_H
# define __USART_H
# include "sys.h"
# include "stdio.h"

extern unsigned char RecFlag;
extern unsigned char RecDataCounter;
void UART_Init(unsigned int bound);
void UART2_Init(unsigned int bound);

extern unsigned char bluetoothdata[4];   // 蓝牙板块变量

// 结构体
// 结构体成员应包括 数据帧的各个部分

typedef struct
{
	unsigned char Head;
	unsigned char Type;
	unsigned char Length;
	unsigned char Data_High;   // 响不响 1代表响
	unsigned char Data_Low;
	unsigned char Jiaoyan;
	unsigned char Tall;
}Uart_Data;

typedef union
{
	unsigned char UartRec_Data[9];
	Uart_Data UartRec_Inf;
}Uart_Inf;

extern Uart_Inf Uart1_Inf;
/*
	UartRec_Data[0] = 0x01;
	UartRec_Data[1] = 0x02;
	
	Head = 0x01;
  Type = 0x02;
*/





# endif

