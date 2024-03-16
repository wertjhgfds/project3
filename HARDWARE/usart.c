#include "usart.h"
#include "nettime.h"
// __IO char ble_buffer[BLE_BUFFERSIZE];  //作为蓝牙模块的接收缓冲区
// __IO int  ble_cnt=0;										//作为蓝牙模块的接收计数器
// __IO uint8_t ble_recvflag = 0;					//作为蓝牙模块的接收标志位

 __IO char host_buffer[HOST_BUFFERSIZE];  //作为的串口2接收缓冲区
 __IO int  host_cnt=0;										//作为的接收计数器
 __IO uint8_t host_recvflag = 0;					//作为的接收标志位

extern __IO uint32_t uwTimeDisplay ;  //表示RTC唤醒中断的标志

//#pragma import(__use_no_semihosting_swi)

//struct __FILE { int handle; /* Add whatever you need here */ };

//FILE __stdout;
//FILE __stdin;

////用户需要重定向到UART
//int fputc(int ch, FILE *f) 
//{
//	USART_SendData(USART1,ch);
//	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
//	
//	return ch;
//}


//void _sys_exit(int return_code) 
//{

//}

//void USART1_IRQHandler(void)
//{
////	uint8_t data = 0;
//	//判断中断是否触发
//	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET )
//	{
////		data = USART_ReceiveData(USART1); //一次只能接收1个字节
//    if(host_cnt < HOST_BUFFERSIZE)
//		{
////      data=USART_ReceiveData(USART2);
//			host_buffer[host_cnt++] = USART_ReceiveData(USART1); //一次只能接收1个字节
////      USART_SendData(USART1,data);      //通过USART1发送1字节
//			USART_SendData(USART1,host_buffer[host_cnt-1]);      //通过USART1发送1字节	
//			//说明数据接收完成
//			if(host_buffer[host_cnt-1] == '0')
//			{
//				host_recvflag = 1;
//			}
//		}			
////		USART_SendData(USART1,data);      //通过USART1发送1字节
//	}
//}

void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	//判断中断是否触发
	if( USART_GetITStatus(USART2, USART_IT_RXNE) == SET )
	{
     data=USART_ReceiveData(USART2);
		//判断接收缓冲区是否满了
		if(host_cnt < HOST_BUFFERSIZE)
		{
//      data=USART_ReceiveData(USART2);
			host_buffer[host_cnt++] = USART_ReceiveData(USART2); //一次只能接收1个字节
			USART_SendData(USART1,host_buffer[host_cnt-1]);      //通过USART1发送1字节	
			//说明数据接收完成
			if(host_buffer[host_cnt-1] == '0')
			{
				host_recvflag = 1;
			}
		}			
	}
}
////串口1的初始化
//void USART1_Config(u32 baud)
//{
//  USART_InitTypeDef USART_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  //1.打开GPIO端口  PA9 PA10  
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  
//  //2.打开串口时钟  USART1 -- APB2
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//  
//  //3.选择引脚的复用功能
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9  , GPIO_AF_USART1);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 , GPIO_AF_USART1);
//  
//  //4.配置GPIO引脚参数并初始化
//  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
//  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_9;	//引脚编号
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//	//5.配置USART1的参数并初始化
//  USART_InitStructure.USART_BaudRate 		= baud;										//波特率
//  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
//  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
//  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
//  USART_Init(USART1, &USART_InitStructure);
//  
//  //6.配置中断参数并初始化
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//中断通道编号
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//抢占优先级
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//子优先级
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
//  NVIC_Init(&NVIC_InitStructure);
//  
//	//7.选择中断源   接收到数据则触发中断
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//  //8.打开USART1
//  USART_Cmd(USART1, ENABLE);
//}

//串口2的初始化
void USART2_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.打开GPIO端口  PA2 PA3  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  //2.打开串口时钟  USART2 -- APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  //3.选择引脚的复用功能
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2  , GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3  , GPIO_AF_USART2);
  
  //4.配置GPIO引脚参数并初始化
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;	//引脚编号
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	//5.配置USART1的参数并初始化
  USART_InitStructure.USART_BaudRate 		= baud;										//波特率
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
  USART_Init(USART2, &USART_InitStructure);
  
  //6.配置中断参数并初始化
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//中断通道编号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
  NVIC_Init(&NVIC_InitStructure);
  
	//7.选择中断源   接收到数据则触发中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  //8.打开USART2
  USART_Cmd(USART2, ENABLE);
}

//发送字符串的函数
void USART1_SendString(char *str)
{ 
	//循环发送字符
  while( *str != '\0' )
	{
		USART_SendData(USART1,*str++);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	}
}
//发送字符串的函数
void USART2_SendString(char *str)
{ 
	//循环发送字符
  while( *str != '\0' )
	{
		USART_SendData(USART2,*str++);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	}
}
void USART2_Handle(void)
{
//while(1)
//{
  while( 1==host_recvflag )
{  
   if(strstr((char *)host_buffer,"WEATHER"))
    {
      Get_Network_Weather();
      
      //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      printf("ok");
      continue;
    }
    
    if(strstr((char *)host_buffer,"LED ON"))
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_9);
//        WS2812_Open();
      //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"FAN ON"))
    {
         FanConfig();
       //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"CURTAIN ON"))
    {
  Motor_Config();
  
  delay_ms(5);
  for(int i=0;i<300;i++)
	{
			Motorcw1();
      delay_ms(2);
	}
    //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
        if(strstr((char *)host_buffer,"LED OFF"))
    {
//       WS2812_Close();
       GPIO_SetBits(GPIOF, GPIO_Pin_9);
       //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"FAN OFF"))
    {
      FanInit();
       //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,BLE_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"CURTAIN OFF"))
    {
  Motor_Config();
  
  delay_ms(5);
  for(int i=0;i<300;i++)
	{
			Motorcw2();
      delay_ms(2);
	}
    //清除接收标志位
			host_recvflag = 0;
			
			//清空接收缓冲区
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//清除接收计数器
			host_cnt = 0;
      continue;
    }
  }
// }
}
//void USART2_Handle(void)//蓝牙调时间
//{
//    char *p = NULL;
//      int h,m,s;
//    char aShowTime[50] = {0};      //用于存储RTC的系统时间
//    char aShowDate[50] = {0};      //用于存储RTC的系统日期
//       RTC_DateTypeDef  RTC_DateStructure;
//       RTC_TimeTypeDef  RTC_TimeStructure;
//    while(1)
//    {   //判断RTC唤醒标志是否成立
//      if( uwTimeDisplay == 1)
//		{
//			uwTimeDisplay = 0; //清除唤醒标志
//			
//			//获取RTC时间
//			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//			//对RTC时间进行转换
//			sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
//      sprintf((char*)aShowDate,"20%2d/%02d/%02d",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);			
//      OLED_ShowChinese(0,1,8);
//      OLED_ShowChinese(0,16,9);
//      OLED_ShowChinese(2,1,6);
//      OLED_ShowChinese(2,16,7);
//      OLED_ShowString(1,5,":");
//      OLED_ShowString(2,5,":");
//			OLED_ShowString(2,7,aShowTime);
//      OLED_ShowString(1,6,aShowDate);
//      
//		}
//    
//    if(ble_recvflag==1)
//  {
//    if(strstr((char*)ble_buffer,"SET TIME"))
//    {
//      OLED_ShowString(2,1," SET TIME ");
//    
//  //对接收到的数据的结束标志进行处理 '\n' --> '\0'  
//			ble_buffer[ble_cnt-1] = '\0'; // "set_time=9:26:30" 
//			
//			//对数据内容进行分析  设置日期 or 设置时间
//			
//				//对数据进行切割  "9:26:30"
//				p = strtok((char *)ble_buffer,"="); // "set_time"
//				p = strtok(NULL,"=");	//"9:26:30"
//				
//				p = strtok(p,":"); 		// "9"
//				h = atoi(p);       		// 9
//				
//				p = strtok(NULL,":");	// "26"
//				m = atoi(p);       		// 26
//				
//				p = strtok(NULL,":");	// "30"
//				s = atoi(p);       		// 30
//				
//     
//				//把转换之后的时间写入到RTC时间结构体中 注意：十进制 -- BCD格式
//				RTC_TimeStructure.RTC_Hours   = (h/10)*16+(h%10);				//时钟
//				RTC_TimeStructure.RTC_Minutes = (m/10)*16+(m%10);				//分钟
//				RTC_TimeStructure.RTC_Seconds = (s/10)*16+(s%10); 			//秒钟
//				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
//				
//				ble_recvflag = 0; //清除接收标志位
//				memset((char *)ble_buffer,0,BLE_BUFFERSIZE); //清除接收缓冲区
//				ble_cnt = 0;  //清除接收计数器      
//      }
//       if(strstr((char*)ble_buffer," SET Date "))
//    {
//      OLED_ShowString(1,1,"SET  Date");
//    
//  //对接收到的数据的结束标志进行处理 '\n' --> '\0'  
//			ble_buffer[ble_cnt-1] = '\0'; // "set_time=9:26:30" 
//			
//			//对数据内容进行分析  设置日期 or 设置时间
//			
//				//对数据进行切割  "9:26:30"
//				p = strtok((char *)ble_buffer,"="); // "set_time"
//				p = strtok(NULL,"=");	//"9:26:30"
//				
//				p = strtok(p,"/"); 		// "9"
//				h = atoi(p);       		// 9
//				
//				p = strtok(NULL,"/");	// "26"
//				m = atoi(p);       		// 26
//				
//				p = strtok(NULL,"/");	// "30"
//				s = atoi(p);       		// 30
//				
//     
//				//把转换之后的时间写入到RTC时间结构体中 注意：十进制 -- BCD格式
//			
//        RTC_DateStructure.RTC_Year 		= (h/10)*16+(h%10);									//年份
//        RTC_DateStructure.RTC_Month 	= (m/10)*16+(m%10);								//月份
//        RTC_DateStructure.RTC_Date 		= (s/10)*16+(s%10); 										//日期
//        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
//				
//				ble_recvflag = 0; //清除接收标志位
//				memset((char *)ble_buffer,0,BLE_BUFFERSIZE); //清除接收缓冲区
//				ble_cnt = 0;  //清除接收计数器      
//      }
//    }
//  }       
//}
