
/***********************************************************************************
* @file    main.c 
* @author  
* @version V1
* @date    
* @brief   
***********************************************************************************/
#include "stm32f4xx.h"  //必须添加
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
SemaphoreHandle_t xSemaphore;      //二值信号量的句柄
TaskHandle_t task1_handle = NULL;  //任务1的句柄
TaskHandle_t task2_handle = NULL;  //任务2的句柄
TaskHandle_t task3_handle = NULL;  //任务3的句柄
extern __IO uint32_t uwTimeDisplay ;  //表示RTC唤醒中断的标志
extern calendar_str calendar;	//时钟结构体
extern uint8_t dhtbuf[5];//存储温湿度数据
  extern      RTC_DateTypeDef  RTC_DateStructure;
  extern     RTC_TimeTypeDef  RTC_TimeStructure;
  extern volatile uint8_t	Rx3Buffer[512];
//任务的入口
void task1_func(void *arg)//初始界面
{
	printf("task1 create success!\r\n");   Get_Network_Time();
    RTC_Config(calendar.hour,calendar.min,calendar.sec,calendar.year,calendar.month,calendar.date );
//   delay_ms(1000);
  while( Mqtt_Connect_Aliyun());					//配置MQTT链接阿里云
  OLED_Clear();
	 char aShowTime[50] = {0};      //用于存储RTC的系统时间
   char aShowDate[50] = {0};      //用于存储RTC的系统日期
  uint16_t adc_value1 = 0;  //用于存储火焰ADC转换结果
  uint16_t adc_value2 = 0;  //用于存储烟雾ADC转换结果
  uint16_t adc_value3 = 0;  //用于存储光强ADC转换结果
  int ppm;
  float vrl=0;
   Get_Network_Time();
    RTC_Config(calendar.hour,calendar.min,calendar.sec,calendar.year,calendar.month,calendar.date );
//   delay_ms(1000);
  while( Mqtt_Connect_Aliyun());					//配置MQTT链接阿里云
  OLED_Clear();
  uwTimeDisplay = 1;
	//必须有死循环
	while(1)
	{ 
//获取互斥锁   不打算超时（获取不到则永远阻塞）
		xSemaphoreTake( xSemaphore, portMAX_DELAY);	
		  if( uwTimeDisplay == 1)
		{
			uwTimeDisplay = 0; //清除唤醒标志
      
//			printf("show time");
			//获取RTC时间
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
               //开启ADC转换
		ADC_SoftwareStartConv(ADC1);
      //开启ADC转换
		ADC_SoftwareStartConv(ADC2);
		//开启ADC转换
		ADC_SoftwareStartConv(ADC3);
		
		//等待转换完成 EOC标志
		while( ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
    //读取ADC转换结果  12bit精度 value 0 ~ 4095
    adc_value1 = ADC_GetConversionValue(ADC1);
     printf("火焰传感器V = %4d\n",adc_value1);
     if(adc_value1<2500)
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    }
		while( ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);
    adc_value2 = ADC_GetConversionValue(ADC2);
     vrl=(5.0/4096)*(adc_value2);
    ppm=pow(11.5428*35.904*5.1*vrl/(5-vrl),0.6549);
    printf("烟雾浓度 = %4d PPM\n",ppm);
    if(ppm>400)
    {
       GPIO_ResetBits(GPIOF, GPIO_Pin_9);
    }
		//等待转换完成 EOC标志
		while( ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC) == RESET);
		adc_value3 = ADC_GetConversionValue(ADC3);
    
    printf("光敏电阻电压值 = %4d\n",adc_value3); 
    
    DHT11_GetValue(dhtbuf);
     printf("温度 = %d ℃,湿度=%d %%RH \n",dhtbuf[2],dhtbuf[0]);
			//对RTC时间进行转换
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
     
    	//利用MQTT上传给阿里云
	mqtt_report_devices_status();	
//  printf("%d",RTC_TimeStructure.RTC_Seconds);
  if(!(RTC_TimeStructure.RTC_Seconds%10))
    { mqtt_send_heart();}
		} 
    //释放互斥锁 
		xSemaphoreGive( xSemaphore );
    vTaskDelay(200);
	}
}
	

//任务的入口
void task2_func(void *arg)//获取网络时间并进行校准显示
{
	printf("task2 create success!\r\n");

  //必须有死循环
	while(1)
	{
//    vTaskSuspend(task1_handle);
//获取互斥锁   不打算超时（获取不到则永远阻塞）
	xSemaphoreTake( xSemaphore, portMAX_DELAY);	
    USART2_Handle();	
   
    //释放互斥锁 
		xSemaphoreGive( xSemaphore );
     vTaskDelay(50);
    
	}
}
void task3_func(void *arg)//获取网络时间并进行校准显示
{
  printf("task3 create success!\r\n");
   //必须有死循环
	while(1)
	{  
    //获取互斥锁   不打算超时（获取不到则永远阻塞）
		xSemaphoreTake( xSemaphore, portMAX_DELAY);	
    printf("x\n");
    mqtt_msg_handle();
   
    //释放互斥锁 
		xSemaphoreGive( xSemaphore );
     vTaskDelay(100);
//    vTaskResume(task1_handle);
  }
}


//程序入口
int main()
{
//  delay_init(168);
//  //0.硬件初始化
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //中断优先级分组 4bit的抢占优先级（0~15）,子优先级是固定为0
	//1.硬件初始化
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
  OLED_WriteCommand(0x2E);        //关闭滚动
//  delay_ms(5);
  OLED_DrawBMP(0,0,128,8,BMP1);
  //配置WiFi以STA联网模式工作
	while( Esp8266_Init());
   
  BaseType_t xReturned; //存储函数返回值
  //1.创建一个互斥锁并进行错误处理  注意：互斥锁的初值是1
	xSemaphore = xSemaphoreCreateMutex(); 
  //1.创建任务
	xReturned = xTaskCreate(    task1_func,												//函数入口指针
															"task1",													//任务名称
															128,															//堆栈大小 128 * 32 / 8 = 512字节
															NULL,														  //不需要给任务传参
															3,																//任务优先级  数字越大则优先级越高，并且不要超过优先级最大值
															&task1_handle											//任务的句柄，可以提供给其他API接口使用
												 );
  //2.创建任务
	xReturned = xTaskCreate(    task2_func,												//函数入口指针
															"task2",													//任务名称
															128,															//堆栈大小 128 * 32 / 8 = 512字节
															NULL,														  //不需要给任务传参
															2,																//任务优先级  数字越大则优先级越高，并且不要超过优先级最大值
															&task2_handle											//任务的句柄，可以提供给其他API接口使用
                         );
                           //2.创建任务
	xReturned = xTaskCreate(    task3_func,												//函数入口指针
															"task3",													//任务名称
															128,															//堆栈大小 128 * 32 / 8 = 512字节
															NULL,														  //不需要给任务传参
															1,																//任务优先级  数字越大则优先级越高，并且不要超过优先级最大值
															&task3_handle											//任务的句柄，可以提供给其他API接口使用
                         );

  //2.启动调度器，这句话后面的代码都是无效的
	vTaskStartScheduler();
}
