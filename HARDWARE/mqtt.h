#ifndef __MQTT_H
#define __MQTT_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "usart2.h"
#include "esp8266.h"
#include "cjson.h"


//�궨��
//�˴��ǰ����Ʒ������Ĺ���ʵ����½����------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������
#define MQTT_BROKERADDRESS		"iot-06z00hok44wo8hy.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID					"k0wb3zqcWE7.DHT11|securemode=2,signmethod=hmacsha256,timestamp=1710225574013|"
#define MQTT_USARNAME					"DHT11&k0wb3zqcWE7"
#define MQTT_PASSWD						"fb078858113c1238018c1a599fa43273b3640e8eabba3d8cbe456f924744d5a6"
#define	MQTT_PUBLISH_TOPIC		"/sys/k0wb3zqcWE7/DHT11/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC	"/sys/k0wb3zqcWE7/DHT11/thing/service/property/set"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define CONNECT_MQTT_LED(x)	PCout(13)=(x)?0:1

//��������

//��������
int Mqtt_Connect_Aliyun(void);											//����MQTT���Ӱ�����
int32_t mqtt_connect(char *client_id,char *user_name,char *password);	//MQTT���ӷ�����
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);	//MQTT��Ϣ����
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);	//MQTT��Ϣ����
int32_t mqtt_send_heart(void);											//MQTT����������
void mqtt_report_devices_status(void);									//�豸״̬�ϱ�
void mqtt_disconnect(void);												//MQTT�������Ͽ�
void mqtt_send_bytes(uint8_t *buf,uint32_t len);						//MQTT��������
void mqtt_msg_handle(void);												//����MQTT�·�����
uint32_t  mqtt_cjson_parse(char *pbuf);									//����MQTT�·�����
void mqtt_report_devices_status2(void);
#endif
