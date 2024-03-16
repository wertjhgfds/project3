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

//串口1的初始化
void USART1_Config(u32 baud);

//串口3的初始化
void USART3_Config(u32 baud);
void USART3_Handle(void);
//发送字符串的函数
void USART3_SendString(char *str);

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len);

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


#endif

