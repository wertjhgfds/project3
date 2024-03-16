#include "motor.h"

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//FAN
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  void FanInit(void)
  {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//FAN
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//1.定义和GPIO外设相关的初始化结构体变量
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.打开GPIO端口的时钟 GPIOF端口 GPIOE端口 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//3.对结构体变量的成员进行赋值
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9);
  }
//TIM的初始化
void FanConfig(void)
{
  
//1.打开TIM14的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//2.打开GPIO端口时钟  PF9
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//3.配置GPIO引脚并初始化
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//输出速率
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;		//上拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	//4.需要选择引脚要复用的功能  PF9 -- TIM14_CH1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
	
//	TIM_DeInit(TIM1);//初始化TIM1寄存器
	//5.配置定时器的基本参数 + 初始化   注意：脉冲信号的周期尽可能短一点  比如10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;  // TIM14 84MHZ / 8400 = 10000HZ  100us计数1次
	TIM_TimeBaseStructure.TIM_Period 		= 100-1;	 // 10ms * 1000 / 100 = 100次
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM14只能递增计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;/*重复计数寄存器*/
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	//6.配置定时器的通道 + 初始化定时器通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							//PWM模式1  CNT < CCR1 通道有效 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较
	TIM_OCInitStructure.TIM_Pulse = 50;														//CCR寄存器的初值  默认占空比50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//高电平有效
//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//互补输出，与以上相反
TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
TIM_CtrlPWMOutputs(TIM1,ENABLE);//高级定时器TIM1就要写这个使能
//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR自动装载默认也是打开的
	//8.使能自动重载预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	//9.打开定时器
	TIM_Cmd(TIM1, ENABLE);

}

void Motor_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.打开GPIO端口的时钟 GPIOF端口 GPIOE端口 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	

	//3.对结构体变量的成员进行赋值
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_15;	//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//输出速率
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  
}

 
//步进电机反转函数
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
//步进电机正转函数
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
//电机停止函数
void MotorStop(void)  
{  
  GPIO_ResetBits(GPIOC,GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_15);
}




