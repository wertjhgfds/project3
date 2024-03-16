#ifndef _USART2_H
#define _USART2_H

#include "stm32f4xx.h"
//#include "usart2.h"

extern uint8_t Rx1Counter;
extern uint8_t Rx1Data;
extern uint8_t Rx1Buffer[256];


extern uint8_t	 Tx3Buffer[512];
extern volatile uint32_t	Rx3Counter;
extern volatile uint8_t	Rx3Data;
extern volatile uint8_t	Rx3End;
extern volatile uint8_t	Rx3Buffer[512];

//����1�ĳ�ʼ��
void USART1_Config(u32 baud);

//����3�ĳ�ʼ��
void USART3_Config(u32 baud);
void USART3_Handle(void);
//�����ַ����ĺ���
void USART3_SendString(char *str);

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len);

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


#endif

