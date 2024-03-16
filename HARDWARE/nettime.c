#include "nettime.h"
#include "usart.h"
extern volatile uint8_t	Rx3Buffer[512];
calendar_str calendar;	//时钟结构体
//获取网络天气
int Get_Network_Weather(void)
{
	uint8_t ret;
		while( Esp8266_Init());
	//连接到目标TCP服务器
	ret = esp8266_connect_server("TCP",NETTIME_SERVERIP,NETTIME_PORTNUM);
	if(ret)
	{
		printf("esp8266_connection_target fail\r\n");
		return -7;
	}	
	printf("esp8266_connection_target success\r\n");
	delay_ms(300);
	
	//检测连接状态
	ret = esp8266_check_connection_status();
	if(ret)
	{
		printf("esp8266_check_connection_status fail\r\n");
		
		//重新连接热点
		while(Esp8266_Init());
	}	
	printf("esp8266_check_connection_status success\r\n");
	delay_ms(1000);delay_ms(1000);
	
	//进入透传模式
	ret = esp8266_entry_transparent_transmission();
	if(ret)
	{
		printf("esp8266_entry_transparent_transmission fail\r\n");
		return -6;
	}	
	printf("esp8266_entry_transparent_transmission success\r\n");
	delay_ms(1000);

	//向目标提出获取时间信息请求
	esp8266_send_str("GET https://api.seniverse.com/v3/weather/now.json?key=S3VkVaWrTIJ76EZbD&location=guangzhou&language=zh-Hans&unit=c&start=0&days=3\r\n\r\n");
	delay_ms(20);//延时20ms返回的是指令发送成功的状态

	//delay_ms(1000);
	//等待时间网址反馈时间信息结束
//	while(esp8266_find_str_in_rx_packet("Connection: close",5000));
	
	
	//退出透传模式，才能输入AT指令
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	delay_ms(1000);
	 printf("%s\r\n",Rx3Buffer);
  WeatherHandle();
	//断开服务器
	ret=esp8266_disconnect_server();
	if(ret)
	{
		printf("esp8266_disconnect_server fail\r\n");
		return -1;
	}	
	printf("esp8266_disconnect_server success\r\n");
	delay_ms(5000);
 
	return 0;
}
/*{"results":[{"location":{"id":"WS0E9D8WN298",
"name":"广州","country":"CN",
"path":"广州,广州,广东,中国","timezone":"Asia/Shanghai",
"timezone_offset":"+08:00"},"now":{"text":"中雨","code":"14",
"temperature":"13"},"last_update":"2024-03-10T15:44:15+08:00"}]}
*/
void WeatherHandle(void)//
{
  char buf[256]={0};
  cJSON *root = cJSON_Parse((const char *)Rx3Buffer);
	printf("%s\n",cJSON_Print(root)); 
	cJSON * obj1 = cJSON_GetObjectItem(root,"results");

	cJSON *obj2 = cJSON_GetArrayItem(obj1,0);

	cJSON *obj3 = cJSON_GetObjectItem(obj2,"now");
  

  printf("%s\r\n",cJSON_GetObjectItem(obj3,"text")->valuestring);
  
	
	printf("%s\r\n",cJSON_GetObjectItem(obj3,"temperature")->valuestring);
  
  sprintf(buf,"?%s]",cJSON_GetObjectItem(obj3,"temperature")->valuestring);

  printf("%s",buf);
  USART2_SendString(buf);


}
//获取网络时间
int Get_Network_Time(void)
{
	uint8_t ret;
	while(1)
	//连接到目标TCP服务器
	{ret = esp8266_connect_server("TCP",NETTIME_SERVERIP2,NETTIME_PORTNUM);
	if(ret)
	{
		printf("esp8266_connection_target fail\r\n");
		return -7;
	}	
	printf("esp8266_connection_target success\r\n");
	delay_ms(100);
	
	//检测连接状态
	ret = esp8266_check_connection_status();
	if(ret)
	{
		printf("esp8266_check_connection_status fail\r\n");
		
		//重新连接热点
		while(Esp8266_Init());
	}	
	printf("esp8266_check_connection_status success\r\n");
	delay_ms(100);delay_ms(100);
	
	//进入透传模式
	ret = esp8266_entry_transparent_transmission();
	if(ret)
	{
		printf("esp8266_entry_transparent_transmission fail\r\n");
		return -6;
	}	
	printf("esp8266_entry_transparent_transmission success\r\n");
	delay_ms(100);

	//向目标提出获取时间信息请求
	esp8266_send_str("GET http://quan.suning.com/getSysTime.do HTTP/1.1\r\nHost: quan.suning.com\r\n\r\n");
	delay_ms(20);//延时20ms返回的是指令发送成功的状态

	delay_ms(100);
	//等待时间网址反馈时间信息结束
//	while(esp8266_find_str_in_rx_packet("Connection: close",5000));

	//退出透传模式，才能输入AT指令
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	delay_ms(100);
	 printf("%s\r\n",Rx3Buffer);
  if(calculating_time()!=0) 
	//断开服务器
	{ret=esp8266_disconnect_server();
	if(ret)
	{
		printf("esp8266_disconnect_server fail\r\n");
		return -1;
	}	
	printf("esp8266_disconnect_server success\r\n");
	delay_ms(100);
  break;}
  else
    continue;
}
	return 0;
}

//计算网络时间
int calculating_time(void)
{
   
    char year[5]={0};
    char month[3]={0};
    char day[3]={0};
    char hour[3]={0};
    char min[3]={0};
    char sec[3]={0};
	 char *data_pt = strstr((const char *) Rx3Buffer,(const char *)"sysTime1")+13;   //寻找到时间结果的地址,首地址返回给字符类型的指针变量（char*） data_pt 
   for(int i=0;i<2;i++)
   {
     year[i]=*(data_pt);
     (data_pt)++;
   }
   for(int j=0;j<2;j++)
   {
      month[j]=*(data_pt);
     (data_pt)++;
   }
   for(int k=0;k<2;k++)
   { 
     day[k]=*(data_pt);
     (data_pt)++;
   }
   for(int l=0;l<2;l++)
   {
      hour[l]=*(data_pt);
     (data_pt)++;
   }
   for(int m=0;m<2;m++)
   { 
     min[m]=*(data_pt);
     (data_pt)++;
   }
   for(int n=0;n<2;n++)
   {
      sec[n]=*(data_pt);
     (data_pt)++;
   }  
    calendar.year=atoi((year));
    calendar.month=atoi((month));
    calendar.date=atoi((day));
    calendar.hour=atoi((hour));
    calendar.min=atoi((min));
    calendar.sec=atoi((sec));
   printf("%d-%d-%d-%d-%d-%d\n",calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);
   return calendar.year;
}