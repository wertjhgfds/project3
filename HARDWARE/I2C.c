#include "I2C.h"

//IICʱ�� ��ʼ�ź� + �����ֽ� + �ӻ�Ӧ�� + ��ȡ�ֽ� + ����Ӧ�� + ֹͣ�ź�

//IIC�ĳ�ʼ��
void IIC_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;			 			//���ģʽ
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		 			  //=
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
	
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;							//���ű��
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;							//���ű��
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 //ȷ��SDA��SCL�����ڿ���״̬
	 IIC_SDA(1);
	 IIC_SCL(1);
}	

//SDA����Ϊ���ģʽ
static void IIC_SDASetOutputMode(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;			 			//���ģʽ
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			  //���츴��
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
	
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;							//���ű��
   GPIO_Init(GPIOB, &GPIO_InitStructure);

}

//SDA����Ϊ����ģʽ
static void IIC_SDASetInputMode(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			 			//���ģʽ
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
	
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;							//���ű��
   GPIO_Init(GPIOB, &GPIO_InitStructure);

}

//IIC�Ŀ�ʼ�ź�
void IIC_Start(void)
{
	//1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();
	
	//2.ȷ��SDA��SCLΪ����״̬  �ߵ�ƽ
	IIC_SDA(1);
	IIC_SCL(1);
	delay_us(5);
	
	//3.��SCL�ߵ�ƽ�ڼ䣬��SDA��ƽ����
	IIC_SDA(0);
	delay_us(5);
	
//	//4.��SCLʱ���ߵĵ�ƽ����
//	IIC_SCL(0);
//	delay_us(5);
}

//IIC�����ֽ�
void IIC_SendByte(uint8_t data)
{
	uint8_t i = 0;
	
	//1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();

	//2.��SCLʱ���ߵ�ƽ����
	IIC_SCL(0);
  IIC_SDA(0);
	delay_us(5);
	
	//3.ѭ������8bit����
	for(i=0;i<8;i++)
	{
		//4.�������������ݵ����λ MSB
		if( data & (1<<(7-i)))
//      	if( data & 0x80 )
			IIC_SDA(1);
		else
			IIC_SDA(0);
		
//		data <<= 1; //��������1bit
		delay_us(5);
		
		//5.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�ӻ����Զ�ȡ
		IIC_SCL(1);
		delay_us(5);
		
		//6.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ��������׼��
		IIC_SCL(0);
		delay_us(5);
	}
}


//IIC�ĵȴ�������Ӧ��
bool IIC_WaitSlaveAck(void)
{
  bool status;
	//1.����SDA������Ϊ����ģʽ
	IIC_SDASetInputMode();
	
//	//2.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ�ӻ�����׼��
//	IIC_SCL(0);
//	delay_us(5);
	
	//3.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�ӻ����Զ�ȡ
	IIC_SCL(1);
	delay_us(5);
	
	//4.��������ȡSDA�����ߵĵ�ƽ״̬
	if( IIC_READ )
		status= false;
	else
		status= true;
  
  IIC_SCL(0);
  delay_us(5);
  
  return status;

}

//IIC��ȡ�ֽ�
uint8_t IIC_ReadByte(void)
{
	uint8_t data = 0;
	uint8_t i = 0;
	
	//1.����SDA������Ϊ����ģʽ
	IIC_SDASetInputMode();
	
	
	//2.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ�ӻ�����׼��
	IIC_SCL(0);
	delay_us(5);
	
	//3.ѭ����ȡ8bit���ݲ��洢�ڱ���
	for(i=0;i<8;i++)
	{
		//4.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�������Զ�ȡ
		IIC_SCL(1);
		delay_us(5);
		
		//5.��ȡSDA�����ߵĵ�ƽ״̬  MSB
    if(IIC_READ==1)
      data|=1<<(7-i);
//		data <<= 1;
//		data |= IIC_READ;
		
		//6.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ�ӻ�����׼��
	  IIC_SCL(0);
	  delay_us(5);
	}
	
	return data;
}

//IIC��������Ӧ��
void IIC_MasterSendAck(uint8_t ack)
{
	//1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();
	
//	//2.��SCLʱ���ߵĵ�ƽ���ͣ���ʱ��������׼��
//	IIC_SCL(0);
//	delay_us(5);
	
	//3.��������Ӧ��
	if( ack )
		IIC_SDA(1); //��ʾ��Ӧ��
	else
		IIC_SDA(0); //��ʾӦ����
	
	delay_us(5);
	
	//4.��SCLʱ���ߵĵ�ƽ���ߣ���ʱ�ӻ����Զ�ȡ
	IIC_SCL(1);
	delay_us(5);
  
  	//4.��SCLʱ���ߵĵ�ƽ���ף��õھŸ��������
	IIC_SCL(0);
	delay_us(5);
  
}


//IIC����ֹͣ�ź�
void IIC_Stop(void)
{
	//1.SDA��������Ϊ���ģʽ
	IIC_SDASetOutputMode();
	
	//2.ȷ��SDA��SCL������͵�ƽ
	IIC_SDA(0);
	IIC_SCL(0);
	delay_us(5);
	
	//3.��SCLʱ���ߵĵ�ƽ����
	IIC_SCL(1);
	delay_us(5);
	
	//4.��SDAʱ���ߵĵ�ƽ����
	IIC_SDA(1);
	delay_us(5);
}