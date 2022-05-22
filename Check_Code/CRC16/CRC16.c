#include <stdio.h>
#include <stdlib.h>
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
//����byte��˳�򣬱���ԭ����10110101,��������10101101
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
//MODBUSЭ�鴫��ʱ��λ�ں��λ��ǰ
{
	unsigned short wCRCin = 0xFFFF;
	//��ʼ�Ĵ���ʹ��ȫ1
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
		printf("����CRC16У����\n");
		printf("---------------------\n");
		printf("ѡ��ҪУ���ַ��ĸ�ʽ\n");
		printf("1����ͨ�ַ���У��\n");
		printf("2��16�����ַ���У��\n");
		printf("---------------------\n");
		printf("��ѡ��1����2:");
		int flag;
		scanf_s("%d", &flag);

		if (flag == 1)
		{
			printf("\t����ͨ�ַ���У�飡��\n");
			unsigned char str[1024];
			int i = 0;
			getchar();
			printf("��������ҪУ����ַ������Իس�����:\n");
			while ((str[i] = getchar()) != '\n')
			{
				i++;
			}
			unsigned char *a = CRC16_MODBUS(str, i);
			printf("CRC16 result is:%X %X\n\n", a[0], a[1]);
		}
		else if (flag == 2)
		{
			printf("\t��16�����ַ���У�飡��\n");
			unsigned char a[100];
			int n, i;
			printf("�������ֽڵ�����: ");
			scanf_s("%d", &n);//�������
			printf("��������ҪУ����ֽڣ��Կո�ֿ����س�������\n");
			for (i = 0; i<n; i++)
				scanf_s("%x", &a[i]);
			printf("CRC16 result is: %X %X\n\n", CRC16_MODBUS(a, n)[0], CRC16_MODBUS(a, n)[1]);
		}
		else
		{
			printf("ѡ������\n");
		}
	}
	return 0;
}

