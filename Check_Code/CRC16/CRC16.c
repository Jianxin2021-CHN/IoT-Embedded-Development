#include <stdio.h>
#include <stdlib.h>
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
//倒换byte的顺序，比如原来是10110101,交换后是10101101
{
	int i;
	unsigned char tmp[4];
	tmp[0] = 0;
	for (i = 0; i< 8; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (7 - i);
	}
	dBuf[0] = tmp[0];
}
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
	int i;
	unsigned short tmp[4];
	tmp[0] = 0;
	for (i = 0; i< 16; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}
unsigned char* CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
//MODBUS协议传输时高位在后低位在前
{
	unsigned short wCRCin = 0xFFFF;
	//初始寄存器使用全1
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;
	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		//printf("this is:%d\n",wChar);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		int i = 0;
		for (; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}

	InvertUint16(&wCRCin, &wCRCin);
	unsigned char result[2];
	result[1] = wCRCin >> 8;
	result[0] = wCRCin;
	return result;
};

int main()
{
	while (1){
		printf("---------------------\n");
		printf("生成CRC16校验码\n");
		printf("---------------------\n");
		printf("选择要校验字符的格式\n");
		printf("1、普通字符串校验\n");
		printf("2、16进制字符串校验\n");
		printf("---------------------\n");
		printf("请选择1或者2:");
		int flag;
		scanf_s("%d", &flag);

		if (flag == 1)
		{
			printf("\t《普通字符串校验！》\n");
			unsigned char str[1024];
			int i = 0;
			getchar();
			printf("请输入需要校验的字符串，以回车结束:\n");
			while ((str[i] = getchar()) != '\n')
			{
				i++;
			}
			unsigned char *a = CRC16_MODBUS(str, i);
			printf("CRC16 result is:%X %X\n\n", a[0], a[1]);
		}
		else if (flag == 2)
		{
			printf("\t《16进制字符串校验！》\n");
			unsigned char a[100];
			int n, i;
			printf("请输入字节的数量: ");
			scanf_s("%d", &n);//输入个数
			printf("请输入需要校验的字节，以空格分开，回车结束：\n");
			for (i = 0; i<n; i++)
				scanf_s("%x", &a[i]);
			printf("CRC16 result is: %X %X\n\n", CRC16_MODBUS(a, n)[0], CRC16_MODBUS(a, n)[1]);
		}
		else
		{
			printf("选择有误\n");
		}
	}
	return 0;
}

