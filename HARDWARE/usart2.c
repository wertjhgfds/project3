#include "usart2.h"


uint8_t Rx1Counter = 0;
uint8_t Rx1Data = 0;
uint8_t Rx1Buffer[256];

uint8_t	 Tx3Buffer[512];
volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Rx3Buffer[512]={0};

/**********************************************printf函数相关********************************************/
#include <stdio.h>
#pragma import(__use_no_semihosting_swi)


struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;

//用户需要重定向到UART
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	
	return ch;
}


void _sys_exit(int return_code) 
{

}
/**********************************************printf函数相关********************************************/


/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//串口1的初始化
void USART1_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.打开GPIO端口  PA9 PA10  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  //2.打开串口时钟  USART1 -- APB2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  //3.选择引脚的复用功能
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9  , GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 , GPIO_AF_USART1);
  
  //4.配置GPIO引脚参数并初始化
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_9;	//引脚编号
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	//5.配置USART1的参数并初始化
  USART_InitStructure.USART_BaudRate 		= baud;										//波特率
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
  USART_Init(USART1, &USART_InitStructure);
  
  //6.配置中断参数并初始化
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//中断通道编号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
  NVIC_Init(&NVIC_InitStructure);
  
	//7.选择中断源   接收到数据则触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  //8.打开USART1
  USART_Cmd(USART1, ENABLE);
}

//串口3的初始化
void USART3_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.打开GPIO端口  PB10 PB11  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  //2.打开串口时钟  USART3 -- APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  //3.选择引脚的复用功能
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10  , GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11  , GPIO_AF_USART3);
  
  //4.配置GPIO引脚参数并初始化
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;	//引脚编号
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	//5.配置USART3的参数并初始化
  USART_InitStructure.USART_BaudRate 		= baud;										//波特率
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
  USART_Init(USART3, &USART_InitStructure);
  
  //6.配置中断参数并初始化
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//中断通道编号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
  NVIC_Init(&NVIC_InitStructure);
  
	//7.选择中断源   接收到数据则触发中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  //8.打开USART3
  USART_Cmd(USART3, ENABLE);
}

//发送字符串的函数
void USART3_SendString(char *str)
{ 
	//循环发送字符
  while( *str != '\0' )
	{
		USART_SendData(USART3,*str++);
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	}
}

void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	//判断中断是否触发
	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET )
	{
		data = USART_ReceiveData(USART1); //一次只能接收1个字节
		USART_SendData(USART1,data);      //通过USART1发送1字节
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		Rx3Data = USART_ReceiveData(USART3);

		Rx3Buffer[Rx3Counter++] = Rx3Data;
	
		if(Rx3Counter >= sizeof( Rx3Buffer))
		{
			Rx3Counter = 0;
			Rx3End=1;
		}
	}
  
	//清除标志位
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

