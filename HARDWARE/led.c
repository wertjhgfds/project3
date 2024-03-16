#include "led.h"
//LED的初始化
void LED_Init(void)
{
	//1.定义和GPIO外设相关的初始化结构体变量
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.打开GPIO端口的时钟 GPIOF端口 GPIOE端口 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//3.对结构体变量的成员进行赋值
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;	//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
}
//按键的初始化
void KEY_Init(void)
{
	//1.定义和GPIO外设相关的初始化结构体变量
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//2.打开GPIO端口的时钟 GPIOA端口 GPIOE端口 
 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//3.对结构体变量的成员进行赋值
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;								//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;							//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;								//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;							//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void BEEP_Init(void)
{
  //1.定义和GPIO外设相关的初始化结构体变量
	GPIO_InitTypeDef  GPIO_InitStructure;
  //2.打开GPIO端口的时钟 GPIOF端口
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//3.对结构体变量的成员进行赋值
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;								//引脚编号

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					//无上下拉
	
	//4.对GPIO端口进行初始化
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}