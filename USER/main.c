
/***********************************************************************************
* @file    main.c 
* @author  
* @version V1
* @date    
* @brief   
***********************************************************************************/
#include "stm32f4xx.h"  //�������
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "led.h"
#include "I2C.h"
#include "adc.h"
#include "rtc.h"
#include "bmp.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
#include "usart2.h"
#include "ws2812.h"
#include "esp8266.h"
#include "mqtt.h"
#include "nettime.h"
SemaphoreHandle_t xSemaphore;      //��ֵ�ź����ľ��
TaskHandle_t task1_handle = NULL;  //����1�ľ��
TaskHandle_t task2_handle = NULL;  //����2�ľ��
TaskHandle_t task3_handle = NULL;  //����3�ľ��
extern __IO uint32_t uwTimeDisplay ;  //��ʾRTC�����жϵı�־
extern calendar_str calendar;	//ʱ�ӽṹ��
extern uint8_t dhtbuf[5];//�洢��ʪ������
  extern      RTC_DateTypeDef  RTC_DateStructure;
  extern     RTC_TimeTypeDef  RTC_TimeStructure;
  extern volatile uint8_t	Rx3Buffer[512];
//��������
void task1_func(void *arg)//��ʼ����
{
	printf("task1 create success!\r\n");   Get_Network_Time();
    RTC_Config(calendar.hour,calendar.min,calendar.sec,calendar.year,calendar.month,calendar.date );
//   delay_ms(1000);
  while( Mqtt_Connect_Aliyun());					//����MQTT���Ӱ�����
  OLED_Clear();
	 char aShowTime[50] = {0};      //���ڴ洢RTC��ϵͳʱ��
   char aShowDate[50] = {0};      //���ڴ洢RTC��ϵͳ����
  uint16_t adc_value1 = 0;  //���ڴ洢����ADCת�����
  uint16_t adc_value2 = 0;  //���ڴ洢����ADCת�����
  uint16_t adc_value3 = 0;  //���ڴ洢��ǿADCת�����
  int ppm;
  float vrl=0;
   Get_Network_Time();
    RTC_Config(calendar.hour,calendar.min,calendar.sec,calendar.year,calendar.month,calendar.date );
//   delay_ms(1000);
  while( Mqtt_Connect_Aliyun());					//����MQTT���Ӱ�����
  OLED_Clear();
  uwTimeDisplay = 1;
	//��������ѭ��
	while(1)
	{ 
//��ȡ������   �����㳬ʱ����ȡ��������Զ������
		xSemaphoreTake( xSemaphore, portMAX_DELAY);	
		  if( uwTimeDisplay == 1)
		{
			uwTimeDisplay = 0; //������ѱ�־
      
//			printf("show time");
			//��ȡRTCʱ��
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
               //����ADCת��
		ADC_SoftwareStartConv(ADC1);
      //����ADCת��
		ADC_SoftwareStartConv(ADC2);
		//����ADCת��
		ADC_SoftwareStartConv(ADC3);
		
		//�ȴ�ת����� EOC��־
		while( ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
    //��ȡADCת�����  12bit���� value 0 ~ 4095
    adc_value1 = ADC_GetConversionValue(ADC1);
     printf("���洫����V = %4d\n",adc_value1);
     if(adc_value1<2500)
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    }
		while( ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);
    adc_value2 = ADC_GetConversionValue(ADC2);
     vrl=(5.0/4096)*(adc_value2);
    ppm=pow(11.5428*35.904*5.1*vrl/(5-vrl),0.6549);
    printf("����Ũ�� = %4d PPM\n",ppm);
    if(ppm>400)
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_9);
    }
		//�ȴ�ת����� EOC��־
		while( ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC) == RESET);
		adc_value3 = ADC_GetConversionValue(ADC3);
    
    printf("���������ѹֵ = %4d\n",adc_value3); 
    
    DHT11_GetValue(dhtbuf);
     printf("�¶� = %d ��,ʪ��=%d %%RH \n",dhtbuf[2],dhtbuf[0]);
			//��RTCʱ�����ת��
			sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
      sprintf((char*)aShowDate,"20%2d/%02d/%02d",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);			
      OLED_ShowChinese(0,1,8);
      OLED_ShowChinese(0,16,9);
      OLED_ShowChinese(2,1,6);
      OLED_ShowChinese(2,16,7);
      OLED_ShowString(1,5,":");
      OLED_ShowString(2,5,":");
			OLED_ShowString(2,7,aShowTime);
      OLED_ShowString(1,6,aShowDate); 
   
    OLED_ShowChinese(4,16,0);
    OLED_ShowChinese(4,32,1);
    OLED_ShowString(3,7,":");
    OLED_ShowChinese2(4,80,0);
    OLED_ShowChar(3,12,'C');
    OLED_ShowChinese(6,16,2);
    OLED_ShowChinese(6,32,1);
    OLED_ShowString(4,7,":");
    OLED_ShowChar(4,11,'%');
    OLED_ShowNum(3, 8, dhtbuf[2], 2);
    OLED_ShowNum(4, 8, dhtbuf[0], 2);
     
    	//����MQTT�ϴ���������
	mqtt_report_devices_status();	
//  printf("%d",RTC_TimeStructure.RTC_Seconds);
  if(!(RTC_TimeStructure.RTC_Seconds%10))
    { mqtt_send_heart();}
		} 
    //�ͷŻ����� 
		xSemaphoreGive( xSemaphore );
    vTaskDelay(200);
	}
}
	

//��������
void task2_func(void *arg)//��ȡ����ʱ�䲢����У׼��ʾ
{
	printf("task2 create success!\r\n");

  //��������ѭ��
	while(1)
	{
//    vTaskSuspend(task1_handle);
//��ȡ������   �����㳬ʱ����ȡ��������Զ������
	xSemaphoreTake( xSemaphore, portMAX_DELAY);	
    USART2_Handle();	
   
    //�ͷŻ����� 
		xSemaphoreGive( xSemaphore );
     vTaskDelay(50);
    
	}
}
void task3_func(void *arg)//��ȡ����ʱ�䲢����У׼��ʾ
{
  printf("task3 create success!\r\n");
   //��������ѭ��
	while(1)
	{  
    //��ȡ������   �����㳬ʱ����ȡ��������Զ������
		xSemaphoreTake( xSemaphore, portMAX_DELAY);	
    printf("x\n");
    mqtt_msg_handle();
   
    //�ͷŻ����� 
		xSemaphoreGive( xSemaphore );
     vTaskDelay(100);
//    vTaskResume(task1_handle);
  }
}


//�������
int main()
{
//  delay_init(168);
//  //0.Ӳ����ʼ��
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�ж����ȼ����� 4bit����ռ���ȼ���0~15��,�����ȼ��ǹ̶�Ϊ0
	//1.Ӳ����ʼ��
	USART1_Config(9600); 
  USART2_Config(9600);
	USART3_Config(115200); 
  IIC_Config();
  LED_Init();
  BEEP_Init();
  KEY_Init();
  OLED_Init(); 
  Fire_Config();
  LightSensor_Config();
  Smog_Config();
	DHT11_Config();
  FanInit();
  WS2812_Init();
  OLED_WriteCommand(0x2E);        //�رչ���
//  delay_ms(5);
  OLED_DrawBMP(0,0,128,8,BMP1);
  //����WiFi��STA����ģʽ����
	while( Esp8266_Init());
   
  BaseType_t xReturned; //�洢��������ֵ
  //1.����һ�������������д�����  ע�⣺�������ĳ�ֵ��1
	xSemaphore = xSemaphoreCreateMutex(); 
  //1.��������
	xReturned = xTaskCreate(    task1_func,												//�������ָ��
															"task1",													//��������
															128,															//��ջ��С 128 * 32 / 8 = 512�ֽ�
															NULL,														  //����Ҫ�����񴫲�
															3,																//�������ȼ�  ����Խ�������ȼ�Խ�ߣ����Ҳ�Ҫ�������ȼ����ֵ
															&task1_handle											//����ľ���������ṩ������API�ӿ�ʹ��
												 );
  //2.��������
	xReturned = xTaskCreate(    task2_func,												//�������ָ��
															"task2",													//��������
															128,															//��ջ��С 128 * 32 / 8 = 512�ֽ�
															NULL,														  //����Ҫ�����񴫲�
															2,																//�������ȼ�  ����Խ�������ȼ�Խ�ߣ����Ҳ�Ҫ�������ȼ����ֵ
															&task2_handle											//����ľ���������ṩ������API�ӿ�ʹ��
                         );
                           //2.��������
	xReturned = xTaskCreate(    task3_func,												//�������ָ��
															"task3",													//��������
															128,															//��ջ��С 128 * 32 / 8 = 512�ֽ�
															NULL,														  //����Ҫ�����񴫲�
															1,																//�������ȼ�  ����Խ�������ȼ�Խ�ߣ����Ҳ�Ҫ�������ȼ����ֵ
															&task3_handle											//����ľ���������ṩ������API�ӿ�ʹ��
                         );

  //2.��������������仰����Ĵ��붼����Ч��
	vTaskStartScheduler();
}
