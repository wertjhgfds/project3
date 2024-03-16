#include "usart.h"
#include "nettime.h"
// __IO char ble_buffer[BLE_BUFFERSIZE];  //��Ϊ����ģ��Ľ��ջ�����
// __IO int  ble_cnt=0;										//��Ϊ����ģ��Ľ��ռ�����
// __IO uint8_t ble_recvflag = 0;					//��Ϊ����ģ��Ľ��ձ�־λ

 __IO char host_buffer[HOST_BUFFERSIZE];  //��Ϊ�Ĵ���2���ջ�����
 __IO int  host_cnt=0;										//��Ϊ�Ľ��ռ�����
 __IO uint8_t host_recvflag = 0;					//��Ϊ�Ľ��ձ�־λ

extern __IO uint32_t uwTimeDisplay ;  //��ʾRTC�����жϵı�־

//#pragma import(__use_no_semihosting_swi)

//struct __FILE { int handle; /* Add whatever you need here */ };

//FILE __stdout;
//FILE __stdin;

////�û���Ҫ�ض���UART
//int fputc(int ch, FILE *f) 
//{
//	USART_SendData(USART1,ch);
//	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
//	
//	return ch;
//}


//void _sys_exit(int return_code) 
//{

//}

//void USART1_IRQHandler(void)
//{
////	uint8_t data = 0;
//	//�ж��ж��Ƿ񴥷�
//	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET )
//	{
////		data = USART_ReceiveData(USART1); //һ��ֻ�ܽ���1���ֽ�
//    if(host_cnt < HOST_BUFFERSIZE)
//		{
////      data=USART_ReceiveData(USART2);
//			host_buffer[host_cnt++] = USART_ReceiveData(USART1); //һ��ֻ�ܽ���1���ֽ�
////      USART_SendData(USART1,data);      //ͨ��USART1����1�ֽ�
//			USART_SendData(USART1,host_buffer[host_cnt-1]);      //ͨ��USART1����1�ֽ�	
//			//˵�����ݽ������
//			if(host_buffer[host_cnt-1] == '0')
//			{
//				host_recvflag = 1;
//			}
//		}			
////		USART_SendData(USART1,data);      //ͨ��USART1����1�ֽ�
//	}
//}

void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	//�ж��ж��Ƿ񴥷�
	if( USART_GetITStatus(USART2, USART_IT_RXNE) == SET )
	{
     data=USART_ReceiveData(USART2);
		//�жϽ��ջ������Ƿ�����
		if(host_cnt < HOST_BUFFERSIZE)
		{
//      data=USART_ReceiveData(USART2);
			host_buffer[host_cnt++] = USART_ReceiveData(USART2); //һ��ֻ�ܽ���1���ֽ�
			USART_SendData(USART1,host_buffer[host_cnt-1]);      //ͨ��USART1����1�ֽ�	
			//˵�����ݽ������
			if(host_buffer[host_cnt-1] == '0')
			{
				host_recvflag = 1;
			}
		}			
	}
}
////����1�ĳ�ʼ��
//void USART1_Config(u32 baud)
//{
//  USART_InitTypeDef USART_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  //1.��GPIO�˿�  PA9 PA10  
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  
//  //2.�򿪴���ʱ��  USART1 -- APB2
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//  
//  //3.ѡ�����ŵĸ��ù���
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9  , GPIO_AF_USART1);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 , GPIO_AF_USART1);
//  
//  //4.����GPIO���Ų�������ʼ��
//  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
//  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_9;	//���ű��
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//	//5.����USART1�Ĳ�������ʼ��
//  USART_InitStructure.USART_BaudRate 		= baud;										//������
//  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
//  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
//  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
//  USART_Init(USART1, &USART_InitStructure);
//  
//  //6.�����жϲ�������ʼ��
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//�ж�ͨ�����
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//��ռ���ȼ�
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//�����ȼ�
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);
//  
//	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//  //8.��USART1
//  USART_Cmd(USART1, ENABLE);
//}

//����2�ĳ�ʼ��
void USART2_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.��GPIO�˿�  PA2 PA3  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  //2.�򿪴���ʱ��  USART2 -- APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  //3.ѡ�����ŵĸ��ù���
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2  , GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3  , GPIO_AF_USART2);
  
  //4.����GPIO���Ų�������ʼ��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;	//���ű��
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	//5.����USART1�Ĳ�������ʼ��
  USART_InitStructure.USART_BaudRate 		= baud;										//������
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);
  
  //6.�����жϲ�������ʼ��
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//�ж�ͨ�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);
  
	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  //8.��USART2
  USART_Cmd(USART2, ENABLE);
}

//�����ַ����ĺ���
void USART1_SendString(char *str)
{ 
	//ѭ�������ַ�
  while( *str != '\0' )
	{
		USART_SendData(USART1,*str++);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
	}
}
//�����ַ����ĺ���
void USART2_SendString(char *str)
{ 
	//ѭ�������ַ�
  while( *str != '\0' )
	{
		USART_SendData(USART2,*str++);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
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
      
      //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
			host_cnt = 0;
      printf("ok");
      continue;
    }
    
    if(strstr((char *)host_buffer,"LED ON"))
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_9);
//        WS2812_Open();
      //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"FAN ON"))
    {
         FanConfig();
       //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
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
    //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
			host_cnt = 0;
      continue;
    }
        if(strstr((char *)host_buffer,"LED OFF"))
    {
//       WS2812_Close();
       GPIO_SetBits(GPIOF, GPIO_Pin_9);
       //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
			host_cnt = 0;
      continue;
    }
    if(strstr((char *)host_buffer,"FAN OFF"))
    {
      FanInit();
       //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,BLE_BUFFERSIZE);
			
			//������ռ�����
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
    //������ձ�־λ
			host_recvflag = 0;
			
			//��ս��ջ�����
			memset((char *)host_buffer,0,HOST_BUFFERSIZE);
			
			//������ռ�����
			host_cnt = 0;
      continue;
    }
  }
// }
}
//void USART2_Handle(void)//������ʱ��
//{
//    char *p = NULL;
//      int h,m,s;
//    char aShowTime[50] = {0};      //���ڴ洢RTC��ϵͳʱ��
//    char aShowDate[50] = {0};      //���ڴ洢RTC��ϵͳ����
//       RTC_DateTypeDef  RTC_DateStructure;
//       RTC_TimeTypeDef  RTC_TimeStructure;
//    while(1)
//    {   //�ж�RTC���ѱ�־�Ƿ����
//      if( uwTimeDisplay == 1)
//		{
//			uwTimeDisplay = 0; //������ѱ�־
//			
//			//��ȡRTCʱ��
//			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//			//��RTCʱ�����ת��
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
//  //�Խ��յ������ݵĽ�����־���д��� '\n' --> '\0'  
//			ble_buffer[ble_cnt-1] = '\0'; // "set_time=9:26:30" 
//			
//			//���������ݽ��з���  �������� or ����ʱ��
//			
//				//�����ݽ����и�  "9:26:30"
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
//				//��ת��֮���ʱ��д�뵽RTCʱ��ṹ���� ע�⣺ʮ���� -- BCD��ʽ
//				RTC_TimeStructure.RTC_Hours   = (h/10)*16+(h%10);				//ʱ��
//				RTC_TimeStructure.RTC_Minutes = (m/10)*16+(m%10);				//����
//				RTC_TimeStructure.RTC_Seconds = (s/10)*16+(s%10); 			//����
//				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
//				
//				ble_recvflag = 0; //������ձ�־λ
//				memset((char *)ble_buffer,0,BLE_BUFFERSIZE); //������ջ�����
//				ble_cnt = 0;  //������ռ�����      
//      }
//       if(strstr((char*)ble_buffer," SET Date "))
//    {
//      OLED_ShowString(1,1,"SET  Date");
//    
//  //�Խ��յ������ݵĽ�����־���д��� '\n' --> '\0'  
//			ble_buffer[ble_cnt-1] = '\0'; // "set_time=9:26:30" 
//			
//			//���������ݽ��з���  �������� or ����ʱ��
//			
//				//�����ݽ����и�  "9:26:30"
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
//				//��ת��֮���ʱ��д�뵽RTCʱ��ṹ���� ע�⣺ʮ���� -- BCD��ʽ
//			
//        RTC_DateStructure.RTC_Year 		= (h/10)*16+(h%10);									//���
//        RTC_DateStructure.RTC_Month 	= (m/10)*16+(m%10);								//�·�
//        RTC_DateStructure.RTC_Date 		= (s/10)*16+(s%10); 										//����
//        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
//				
//				ble_recvflag = 0; //������ձ�־λ
//				memset((char *)ble_buffer,0,BLE_BUFFERSIZE); //������ջ�����
//				ble_cnt = 0;  //������ռ�����      
//      }
//    }
//  }       
//}
