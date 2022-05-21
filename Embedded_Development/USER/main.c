#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "beep.h"
#include "time.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"
#include "dht11.h"
#include "stdio.h"
#include "ad.h"

unsigned char TempData[3];
// ���İ�PA9 �ӵ�ģ���RXD
// ���İ�PA10�ӵ�ģ���TXD
// ���İ�G   �ӵ�ģ���GND

 int main(void)
 {
	unsigned char Result_Flag = 0;
	 
	delay_init();	    	// ��ʱ������ʼ��	  
  LED_Init();		  	 	// ��ʼ����LED���ӵ�Ӳ���ӿ�
  BEEP_Init();        // �������ܽų�ʼ��
	UART_Init(9600);
	 
	UART2_Init(9600);   // ���������ź�   
	 
	ADC_MyInit();
	TIM3_Init(19,7199);        // 4999 7299 500ms 0.5s
	OLED_Init();
	while(1)
	{
		delay_ms(100);
		Result_Flag = DHT11_ReadData(&DHT11_Data);
		if(Result_Flag ==1)
		{
			RecFlag = 0;
			OLED_Refresh();
			delay_ms(100);
			// OLED_Clear();
			
			OLED_ShowChinese(18,0,0,16);      // ��
			OLED_ShowChinese(36,0,1,16);      // ��
			OLED_ShowChinese(18,16,2,16);     // ʪ
			OLED_ShowChinese(36,16,3,16);     // ��
			
			OLED_ShowNum(54,0,DHT11_Data.temp_int,2,16);
			OLED_ShowString(72,0,".",16);
			OLED_ShowNum(80,0,DHT11_Data.temp_deci,1,16);
			OLED_ShowChinese(88,0,4,16);  // ���϶� ��
			
			if(DHT11_Data.temp_int >26)
			{
				BEEP = 1;
				printf("Tempeture High!\n");
				Uart1_Inf.UartRec_Inf.Head = 'B';
			}
			else{
				BEEP = 0;
				
			}
			delay_ms(100);
			
			OLED_ShowNum(54,16,DHT11_Data.humi_int,2,16);
			OLED_ShowString(72,16,".",16);
			OLED_ShowNum(80,16,DHT11_Data.humi_deci,1,16);
			OLED_ShowString(88,16,"RH",16);  // ���϶� ��
			
			if(DHT11_Data.humi_int > 80)
			{
				BEEP = 1;
				printf("Humidity High\n");
				Uart1_Inf.UartRec_Inf.Head = 'B';
			}
			else{
				BEEP = 0;
				
			}
			delay_ms(100);
			
			if((Uart1_Inf.UartRec_Inf.Head == 'A') && (Uart1_Inf.UartRec_Inf.Tall == 'A') && (Uart1_Inf.UartRec_Inf.Length == '7'))
			{
				delay_ms(100);
				if(Uart1_Inf.UartRec_Inf.Type == '1' && Uart1_Inf.UartRec_Inf.Data_High == '1')
				{
					BEEP = 1;
					printf("Beep is singing.\n");
				}
				
				if(Uart1_Inf.UartRec_Inf.Type == '1' && Uart1_Inf.UartRec_Inf.Data_High == '2')
				{
					printf("Beep is not singing.\n");
					BEEP = 0;
				}
				
				if(Uart1_Inf.UartRec_Inf.Type == '2' && Uart1_Inf.UartRec_Inf.Data_High == '1')
				{
					printf("Temperature: %d.%d��\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci);
				}
				
				if(Uart1_Inf.UartRec_Inf.Type == '2' && Uart1_Inf.UartRec_Inf.Data_High == '2')
				{
					printf("Humidity: %d.%dRH\r\n",DHT11_Data.humi_int,DHT11_Data.humi_deci);
				}
				
			}
		}
		if(Result_Flag == 0)
		{
			printf("DATA ERROR!");
			//�����쳣
		}
 }

}


//	// USART1תUSART2����
//	while(1)
//		{
//			if(RecFlag == 1)
//			{
//				RecFlag = 0;
//				
//				// �ж��Ƿ����ͨ��Э�� 0x20 0x22 0x05 0x04
//				if((bluetoothdata[0] == 0x20) && (bluetoothdata[1] == 0x22) 
//					&& (bluetoothdata[2] == 0x05) && (bluetoothdata[3] == 0x04))
//				{
//					// ��ӡ�����ַ�֤��USART2�˿ڽ��ܳɹ�
//					printf("******************\n");
//					printf("Welocome to Ocean University of China!\n");
//					printf("My name is Jianxin Wu, \n and I have finished my homework.\n");
//					printf("Thank you very much!\n");
//					
//					// ������ܳɹ�����USART1�˷��� 0x1A
//					delay_ms(1000);
//					USART_SendData(USART1,0x1A);  
//				}
//				else
//				{
//					// �������ʧ�ܣ���USART1�˷��� 0x1B
//					delay_ms(1000);
//					USART_SendData(USART1,0x1B);  
//				}
//				
//			}
//			
//		}	
				


//    // ���°��
//    while(1)
//		{
//			delay_ms(1500);
//			Result_Flag = DHT11_ReadData(&DHT11_Data);
//			if(Result_Flag ==1)
//			{
//				//�Ѿ���ȡ����ȷ����ʪ������
//				//USART_SendData(USART1,DHT11_Data.temp_int);
//				
//				printf("�Ҽҵ��¶ȣ�%d.%d��\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci);
//				delay_ms(100);
//				printf("�Ҽҵ�ʪ�ȣ�%d.%dRH\r\n",DHT11_Data.humi_int,DHT11_Data.humi_deci);
//				delay_ms(100);
//				printf("��صĵ�ѹ��%dmv\r\n",ADC_Con[0]*3300/4095);
//			}
//			if(Result_Flag == 0)
//			{
//				//�����쳣
//			}
//   }


/* OLED��ʾ */
/*while(1)
{
	delay_ms(500);
		 
		 OLED_Clear();
		
		 OLED_ShowChinese(36,0,0,16);     // ��
		 OLED_ShowChinese(54,0,1,16);     // ��
		 OLED_ShowChinese(72,0,2,16);     // ��
		 OLED_ShowChinese(90,0,3,16);     // �� 
			
		 OLED_ShowChinese(36,16,4,16);     // ȡ
		 OLED_ShowChinese(54,16,5,16);     // �� 
		 OLED_ShowChinese(72,16,6,16);     // �� 
		 OLED_ShowChinese(90,16,7,16);     // Զ 
			
     OLED_ShowString(48,32,"SHANDONG",16);
		 OLED_ShowString(54,48,"OUC",16);
		 // OLED_ShowChar(48,48,1,16);   // ��ʾASCII�ַ�
		 
		 OLED_Refresh();
		 delay_ms(5000);
		 OLED_Clear();
		 
		 OLED_ShowChinese(0,0,0,16);     // 16*16 ��
		 OLED_ShowChinese(16,0,0,24);    // 24*24 ��
		 
		 OLED_Refresh();
		 delay_ms(500);
		 OLED_Clear();
		 
		 OLED_ShowString(0,0,"ABC",12);  // 6*12 "ABC"
		 OLED_ShowString(0,12,"ABC",16); // 8*16 "ABC"
		 OLED_Refresh();
		 delay_ms(500);
		 
		 OLED_ShowPicture(0,0,128,8,BMP1);	 
		 OLED_ScrollDisplay(11,4);
		 
		
			//  OLED_DrawPoint(1,1);
  		//OLED_Refresh();
}*/


/*
	�����շ�
	while(1)
		{
			if(RecFlag == 1)
			{
				RecFlag = 0;
				
				if((Uart1_Inf.UartRec_Inf.Head == 0xA5) && (Uart1_Inf.UartRec_Inf.Tall == 0x5A) && (Uart1_Inf.UartRec_Inf.Length == 0x07))
				{
					if(Uart1_Inf.UartRec_Inf.Data_High == 0x01)
					{
						BEEP = 1;
						USART_SendData(USART1, Uart1_Inf.UartRec_Inf.Data_High);
					}
					if(Uart1_Inf.UartRec_Inf.Data_High == 0x00)
					{
						BEEP = 0;
						USART_SendData(USART1, Uart1_Inf.UartRec_Inf.Data_High);
					}
				}
				delay_ms(1000);
				USART_SendData(USART1,0x0A);
			}
			// memset(Uart1_Inf.UartRec_Data,0,sizeof(Uart1_Inf.UartRec_Data))
		
	}
	
*/


/* **************
    �����ǰ��
	 ************** */
//#include "sys.h"	
//#include "delay.h"	
//#include "led.h" 
//#include "beep.h"
//#include "time.h" 
//#include "usart.h"
//#include "oled.h"
//#include "bmp.h"
//#include "stdio.h"
//#include "../hardware/MPU6050/mpu6050.h"
//#include "../hardware/MPU6050/eMPL/inv_mpu.h"
//#include "../hardware/MPU6050/eMPL/inv_mpu_dmp_motion_driver.h" 
//#include "../hardware/dht11/dht11.h"
//#include "../hardware/AD/ad.h"
//unsigned char TempData[3];


////����1����1���ַ� 
////c:Ҫ���͵��ַ�
//void usart1_send_char(u8 c)
//{   	
////	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
//	USART_SendData(USART1,c);  
//} 
////�������ݸ�����������λ�����(V2.6�汾)
////fun:������. 0XA0~0XAF
////data:���ݻ�����,���28�ֽ�!!
////len:data����Ч���ݸ���
//void usart1_niming_report(u8 fun,u8*data,u8 len)
//{
//	u8 send_buf[32];
//	u8 i;
//	if(len>28)return;	//���28�ֽ����� 
//	send_buf[len+3]=0;	//У��������
//	send_buf[0]=0X88;	//֡ͷ
//	send_buf[1]=fun;	//������
//	send_buf[2]=len;	//���ݳ���
//	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
//	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
//	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
//}
////���ͼ��ٶȴ��������ݺ�����������
////aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
////gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
//{
//	u8 tbuf[12]; 
//	tbuf[0]=(aacx>>8)&0XFF;
//	tbuf[1]=aacx&0XFF;
//	tbuf[2]=(aacy>>8)&0XFF;
//	tbuf[3]=aacy&0XFF;
//	tbuf[4]=(aacz>>8)&0XFF;
//	tbuf[5]=aacz&0XFF; 
//	tbuf[6]=(gyrox>>8)&0XFF;
//	tbuf[7]=gyrox&0XFF;
//	tbuf[8]=(gyroy>>8)&0XFF;
//	tbuf[9]=gyroy&0XFF;
//	tbuf[10]=(gyroz>>8)&0XFF;
//	tbuf[11]=gyroz&0XFF;
//	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
//}	
////ͨ������1�ϱ���������̬���ݸ�����
////aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
////gyrox,gyroy,gyroz:x,y,z�������������������ֵ
////roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
////pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
////yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
//void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
//{
//	u8 tbuf[28]; 
//	u8 i;
//	for(i=0;i<28;i++)tbuf[i]=0;//��0
//	tbuf[0]=(aacx>>8)&0XFF;
//	tbuf[1]=aacx&0XFF;
//	tbuf[2]=(aacy>>8)&0XFF;
//	tbuf[3]=aacy&0XFF;
//	tbuf[4]=(aacz>>8)&0XFF;
//	tbuf[5]=aacz&0XFF; 
//	tbuf[6]=(gyrox>>8)&0XFF;
//	tbuf[7]=gyrox&0XFF;
//	tbuf[8]=(gyroy>>8)&0XFF;
//	tbuf[9]=gyroy&0XFF;
//	tbuf[10]=(gyroz>>8)&0XFF;
//	tbuf[11]=gyroz&0XFF;	
//	tbuf[18]=(roll>>8)&0XFF;
//	tbuf[19]=roll&0XFF;
//	tbuf[20]=(pitch>>8)&0XFF;
//	tbuf[21]=pitch&0XFF;
//	tbuf[22]=(yaw>>8)&0XFF;
//	tbuf[23]=yaw&0XFF;
//	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
//}  

//// ���İ�PA9  �ӵ� ģ���  RXD 
//// ���İ�PA10 �ӵ� ģ���  TXD
//// ���İ�G    �ӵ� ģ���  GND

// int main(void)
// {
//	 	u8 t=0,report=1;			//Ĭ�Ͽ����ϱ�
//    unsigned char Resault_Flag = 0 ;
//	 	float pitch,roll,yaw; 		//ŷ����
//		short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//		short gyrox,gyroy,gyroz;	//������ԭʼ����
//		short temp;					//�¶�	
//	  delay_init();	    	 //��ʱ������ʼ��	   
//  	BEEP_Init();         //�������ܽų�ʼ?
//	  TIM3_Init(19,7199);        // 2ms 19 7199 72M /7200  10K 1/10 = 0.1ms  20  2ms
//	  UART_Init(9600);
//	  ADC_MyInit();
//		OLED_Init();
//	  MPU_Init();					//��ʼ��MPU6050
//	 
//	// 	OLED_Refresh();
//		delay_ms(1500);
//	 
//	 	while(mpu_dmp_init())
//		{
//		
//			delay_ms(200);

//		}  
//	 
//		while(1)
//		{


//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 
//			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//			if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
//			if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));

//			}
//		}
////		t++; 


//		
//}
