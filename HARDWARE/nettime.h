#ifndef __NETTIME_H
#define __NETTIME_H

//����ͷ�ļ�
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
	//������������
	u8 year;
	u8  month;
	u8  date; 
}calendar_str;	


//�궨��
#define NETTIME_PORTNUM 	80						//����ʱ�� 80	
#define NETTIME_SERVERIP 	"api.seniverse.com"			//����ʱ��IP
//�궨��

#define NETTIME_SERVERIP2 	"quan.suning.com"			//����ʱ��IP

//��������

//��������
int Get_Network_Time(void);
int calculating_time(void);
int Get_Network_Weather(void);
void WeatherHandle(void);
#endif