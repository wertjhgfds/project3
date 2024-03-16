#include "usart2.h"


uint8_t Rx1Counter = 0;
uint8_t Rx1Data = 0;
uint8_t Rx1Buffer[256];

uint8_t	 Tx3Buffer[512];
volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Rx3Buffer[512]={0};

/**********************************************printf�������********************************************/
#include <stdio.h>
#pragma import(__use_no_semihosting_swi)


struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;

//�û���Ҫ�ض���UART
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
	
	return ch;
}


void _sys_exit(int return_code) 
{

}
/**********************************************printf�������********************************************/


/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//����1�ĳ�ʼ��
void USART1_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.��GPIO�˿�  PA9 PA10  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  //2.�򿪴���ʱ��  USART1 -- APB2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  //3.ѡ�����ŵĸ��ù���
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9  , GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 , GPIO_AF_USART1);
  
  //4.����GPIO���Ų�������ʼ��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_9;	//���ű��
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	//5.����USART1�Ĳ�������ʼ��
  USART_InitStructure.USART_BaudRate 		= baud;										//������
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);
  
  //6.�����жϲ�������ʼ��
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//�ж�ͨ�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);
  
	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  //8.��USART1
  USART_Cmd(USART1, ENABLE);
}

//����3�ĳ�ʼ��
void USART3_Config(u32 baud)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //1.��GPIO�˿�  PB10 PB11  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  //2.�򿪴���ʱ��  USART3 -- APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  //3.ѡ�����ŵĸ��ù���
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10  , GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11  , GPIO_AF_USART3);
  
  //4.����GPIO���Ų�������ʼ��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;	//���ű��
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	//5.����USART3�Ĳ�������ʼ��
  USART_InitStructure.USART_BaudRate 		= baud;										//������
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
  USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);
  
  //6.�����жϲ�������ʼ��
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//�ж�ͨ�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);
  
	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  //8.��USART3
  USART_Cmd(USART3, ENABLE);
}

//�����ַ����ĺ���
void USART3_SendString(char *str)
{ 
	//ѭ�������ַ�
  while( *str != '\0' )
	{
		USART_SendData(USART3,*str++);
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
	}
}

void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	//�ж��ж��Ƿ񴥷�
	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET )
	{
		data = USART_ReceiveData(USART1); //һ��ֻ�ܽ���1���ֽ�
		USART_SendData(USART1,data);      //ͨ��USART1����1�ֽ�
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
  
	//�����־λ
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

