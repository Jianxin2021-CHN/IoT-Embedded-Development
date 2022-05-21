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
// 核心板PA9 接到模块的RXD
// 核心板PA10接到模块的TXD
// 核心板G   接到模块的GND

 int main(void)
 {
	unsigned char Result_Flag = 0;
	 
	delay_init();	    	// 延时函数初始化	  
  LED_Init();		  	 	// 初始化与LED连接的硬件接口
  BEEP_Init();        // 蜂鸣器管脚初始化
	UART_Init(9600);
	 
	UART2_Init(9600);   // 接受蓝牙信号   
	 
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
			
			OLED_ShowChinese(18,0,0,16);      // 温
			OLED_ShowChinese(36,0,1,16);      // 度
			OLED_ShowChinese(18,16,2,16);     // 湿
			OLED_ShowChinese(36,16,3,16);     // 度
			
			OLED_ShowNum(54,0,DHT11_Data.temp_int,2,16);
			OLED_ShowString(72,0,".",16);
			OLED_ShowNum(80,0,DHT11_Data.temp_deci,1,16);
			OLED_ShowChinese(88,0,4,16);  // 摄氏度 ℃
			
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
			OLED_ShowString(88,16,"RH",16);  // 摄氏度 ℃
			
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
					printf("Temperature: %d.%d℃\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci);
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
			//数据异常
		}
 }

}


//	// USART1转USART2蓝牙
//	while(1)
//		{
//			if(RecFlag == 1)
//			{
//				RecFlag = 0;
//				
//				// 判断是否符合通信协议 0x20 0x22 0x05 0x04
//				if((bluetoothdata[0] == 0x20) && (bluetoothdata[1] == 0x22) 
//					&& (bluetoothdata[2] == 0x05) && (bluetoothdata[3] == 0x04))
//				{
//					// 打印如下字符证明USART2端口接受成功
//					printf("******************\n");
//					printf("Welocome to Ocean University of China!\n");
//					printf("My name is Jianxin Wu, \n and I have finished my homework.\n");
//					printf("Thank you very much!\n");
//					
//					// 如果接受成功，向USART1端返回 0x1A
//					delay_ms(1000);
//					USART_SendData(USART1,0x1A);  
//				}
//				else
//				{
//					// 如果接受失败，向USART1端返回 0x1B
//					delay_ms(1000);
//					USART_SendData(USART1,0x1B);  
//				}
//				
//			}
//			
//		}	
				


//    // 测温板块
//    while(1)
//		{
//			delay_ms(1500);
//			Result_Flag = DHT11_ReadData(&DHT11_Data);
//			if(Result_Flag ==1)
//			{
//				//已经读取到正确的温湿度数据
//				//USART_SendData(USART1,DHT11_Data.temp_int);
//				
//				printf("我家的温度：%d.%d℃\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci);
//				delay_ms(100);
//				printf("我家的湿度：%d.%dRH\r\n",DHT11_Data.humi_int,DHT11_Data.humi_deci);
//				delay_ms(100);
//				printf("电池的电压：%dmv\r\n",ADC_Con[0]*3300/4095);
//			}
//			if(Result_Flag == 0)
//			{
//				//数据异常
//			}
//   }


/* OLED显示 */
/*while(1)
{
	delay_ms(500);
		 
		 OLED_Clear();
		
		 OLED_ShowChinese(36,0,0,16);     // 海
		 OLED_ShowChinese(54,0,1,16);     // 纳
		 OLED_ShowChinese(72,0,2,16);     // 百
		 OLED_ShowChinese(90,0,3,16);     // 川 
			
		 OLED_ShowChinese(36,16,4,16);     // 取
		 OLED_ShowChinese(54,16,5,16);     // 则 
		 OLED_ShowChinese(72,16,6,16);     // 行 
		 OLED_ShowChinese(90,16,7,16);     // 远 
			
     OLED_ShowString(48,32,"SHANDONG",16);
		 OLED_ShowString(54,48,"OUC",16);
		 // OLED_ShowChar(48,48,1,16);   // 显示ASCII字符
		 
		 OLED_Refresh();
		 delay_ms(5000);
		 OLED_Clear();
		 
		 OLED_ShowChinese(0,0,0,16);     // 16*16 中
		 OLED_ShowChinese(16,0,0,24);    // 24*24 中
		 
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
	串口收发
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
    陀螺仪板块
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


////串口1发送1个字符 
////c:要发送的字符
//void usart1_send_char(u8 c)
//{   	
////	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//	USART_SendData(USART1,c);  
//} 
////传送数据给匿名四轴上位机软件(V2.6版本)
////fun:功能字. 0XA0~0XAF
////data:数据缓存区,最多28字节!!
////len:data区有效数据个数
//void usart1_niming_report(u8 fun,u8*data,u8 len)
//{
//	u8 send_buf[32];
//	u8 i;
//	if(len>28)return;	//最多28字节数据 
//	send_buf[len+3]=0;	//校验数置零
//	send_buf[0]=0X88;	//帧头
//	send_buf[1]=fun;	//功能字
//	send_buf[2]=len;	//数据长度
//	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
//	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
//	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
//}
////发送加速度传感器数据和陀螺仪数据
////aacx,aacy,aacz:x,y,z三个方向上面的加速度值
////gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
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
//	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
//}	
////通过串口1上报结算后的姿态数据给电脑
////aacx,aacy,aacz:x,y,z三个方向上面的加速度值
////gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
////roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
////pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
////yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
//{
//	u8 tbuf[28]; 
//	u8 i;
//	for(i=0;i<28;i++)tbuf[i]=0;//清0
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
//	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
//}  

//// 核心板PA9  接到 模块的  RXD 
//// 核心板PA10 接到 模块的  TXD
//// 核心板G    接到 模块的  GND

// int main(void)
// {
//	 	u8 t=0,report=1;			//默认开启上报
//    unsigned char Resault_Flag = 0 ;
//	 	float pitch,roll,yaw; 		//欧拉角
//		short aacx,aacy,aacz;		//加速度传感器原始数据
//		short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//		short temp;					//温度	
//	  delay_init();	    	 //延时函数初始化	   
//  	BEEP_Init();         //蜂鸣器管脚初始?
//	  TIM3_Init(19,7199);        // 2ms 19 7199 72M /7200  10K 1/10 = 0.1ms  20  2ms
//	  UART_Init(9600);
//	  ADC_MyInit();
//		OLED_Init();
//	  MPU_Init();					//初始化MPU6050
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
//			temp=MPU_Get_Temperature();	//得到温度值
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
//			if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));

//			}
//		}
////		t++; 


//		
//}
