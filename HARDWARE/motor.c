#include "motor.h"

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//FAN
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  void FanInit(void)
  {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//FAN
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//1.�����GPIO������صĳ�ʼ���ṹ�����
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.��GPIO�˿ڵ�ʱ�� GPIOF�˿� GPIOE�˿� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//3.�Խṹ������ĳ�Ա���и�ֵ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9);
  }
//TIM�ĳ�ʼ��
void FanConfig(void)
{
  
//1.��TIM14��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//2.��GPIO�˿�ʱ��  PF9
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//3.����GPIO���Ų���ʼ��
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//���ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;		//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	//4.��Ҫѡ������Ҫ���õĹ���  PF9 -- TIM14_CH1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
	
//	TIM_DeInit(TIM1);//��ʼ��TIM1�Ĵ���
	//5.���ö�ʱ���Ļ������� + ��ʼ��   ע�⣺�����źŵ����ھ����ܶ�һ��  ����10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;  // TIM14 84MHZ / 8400 = 10000HZ  100us����1��
	TIM_TimeBaseStructure.TIM_Period 		= 100-1;	 // 10ms * 1000 / 100 = 100��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM14ֻ�ܵ�������
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;/*�ظ������Ĵ���*/
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	//6.���ö�ʱ����ͨ�� + ��ʼ����ʱ��ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							//PWMģʽ1  CNT < CCR1 ͨ����Ч 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 50;														//CCR�Ĵ����ĳ�ֵ  Ĭ��ռ�ձ�50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//�ߵ�ƽ��Ч
//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//����������������෴
TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
TIM_CtrlPWMOutputs(TIM1,ENABLE);//�߼���ʱ��TIM1��Ҫд���ʹ��
//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	//8.ʹ���Զ�����Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	//9.�򿪶�ʱ��
	TIM_Cmd(TIM1, ENABLE);

}

void Motor_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.��GPIO�˿ڵ�ʱ�� GPIOF�˿� GPIOE�˿� 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	

	//3.�Խṹ������ĳ�Ա���и�ֵ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_15;	//���ű��

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//�������
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  
}

 
//���������ת����
void Motorcw2(void)
{
	DC_A_ON;
	DC_B_ON;
	DC_C_OFF;
	DC_D_OFF;
	delay_ms(2);
	
	DC_A_OFF;
	DC_B_ON;
	DC_C_ON;
	DC_D_OFF;
	delay_ms(2);
	
	DC_A_OFF;
	DC_B_OFF;
	DC_C_ON;
	DC_D_ON;
	delay_ms(2);	
	
	DC_A_ON;
	DC_B_OFF;
	DC_C_OFF;
	DC_D_ON;
	delay_ms(2);	
}
//���������ת����
void Motorcw1(void)
{	
	DC_A_ON;
	DC_B_OFF;
	DC_C_OFF;
	DC_D_ON;
	delay_ms(2);

	DC_A_OFF;
	DC_B_OFF;
	DC_C_ON;
	DC_D_ON;
	delay_ms(2);	

	DC_A_OFF;
	DC_B_ON;
	DC_C_ON;
	DC_D_OFF;
	delay_ms(2);

	DC_A_ON;
	DC_B_ON;
	DC_C_OFF;
	DC_D_OFF;
	delay_ms(2);
}
//���ֹͣ����
void MotorStop(void)  
{  
  GPIO_ResetBits(GPIOC,GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_15);
}




