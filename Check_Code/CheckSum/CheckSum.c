#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char *getCheckSum(unsigned char *string, int length)
{
	int sum = 0;
	int i = 0;
	for (; i<length; i++)
	{
		sum += string[i];
	}
	unsigned short result = sum % 65536;
	result = result ^ 0xFFFF;
	result += 1;
	unsigned char checkSum[2];
	checkSum[0] = result >> 8;
	checkSum[1] = result;
	return checkSum;
}

int main()
{
	while (1){
		printf("---------------------\n");
		printf("����У���\n");
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
			unsigned char *a = getCheckSum(str, i);
			printf("CheckSum result is:%X %X\n\n", a[0], a[1]);
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
			printf("CheckSum result is: %X %X\n\n", getCheckSum(a, n)[0], getCheckSum(a, n)[1]);
		}
		else
		{
			printf("ѡ������\n");
		}
	}
	return 0;
}

