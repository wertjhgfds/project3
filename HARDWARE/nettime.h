#ifndef __NETTIME_H
#define __NETTIME_H

//包含头文件
#include "stm32f4xx.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "esp8266.h"
#include "cJSON.h"
#include "rtc.h"
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//公历日月年周
	u8 year;
	u8  month;
	u8  date; 
}calendar_str;	


//宏定义
#define NETTIME_PORTNUM 	80						//网络时间 80	
#define NETTIME_SERVERIP 	"api.seniverse.com"			//网络时间IP
//宏定义

#define NETTIME_SERVERIP2 	"quan.suning.com"			//网络时间IP

//变量声明

//函数声明
int Get_Network_Time(void);
int calculating_time(void);
int Get_Network_Weather(void);
void WeatherHandle(void);
#endif