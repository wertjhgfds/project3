#include "led.h"
//LED�ĳ�ʼ��
void LED_Init(void)
{
	//1.�����GPIO������صĳ�ʼ���ṹ�����
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.��GPIO�˿ڵ�ʱ�� GPIOF�˿� GPIOE�˿� 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//3.�Խṹ������ĳ�Ա���и�ֵ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;	//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
}
//�����ĳ�ʼ��
void KEY_Init(void)
{
	//1.�����GPIO������صĳ�ʼ���ṹ�����
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.��GPIO�˿ڵ�ʱ�� GPIOA�˿� GPIOE�˿� 
 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//3.�Խṹ������ĳ�Ա���и�ֵ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;								//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;							//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;								//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;							//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void BEEP_Init(void)
{
  //1.�����GPIO������صĳ�ʼ���ṹ�����
	GPIO_InitTypeDef  GPIO_InitStructure;
  //2.��GPIO�˿ڵ�ʱ�� GPIOF�˿�
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//3.�Խṹ������ĳ�Ա���и�ֵ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;								//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//��������
	
	//4.��GPIO�˿ڽ��г�ʼ��
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}