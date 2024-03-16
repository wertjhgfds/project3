#include "oled.h"
#include "I2C.h"
#include "delay.h"
#include "oledfont.h"
/**
 * @brief  OLEDд����
 * @param  Command Ҫд�������
 * @retval ��
 */
void OLED_WriteCommand(uint8_t Command)
{
    IIC_Start();
    IIC_SendByte(0x78);
    IIC_WaitSlaveAck();
    IIC_SendByte(0x00); // д����
    IIC_WaitSlaveAck();
    IIC_SendByte(Command);
    IIC_WaitSlaveAck();
    IIC_Stop();
}

/**
 * @brief  OLEDд����
 * @param  Data Ҫд�������
 * @retval ��
 */
void OLED_WriteData(uint8_t Data)
{
    IIC_Start();
    IIC_SendByte(0x78);
    IIC_WaitSlaveAck();
    IIC_SendByte(0x40); // д����
    IIC_WaitSlaveAck();
    IIC_SendByte(Data);
    IIC_WaitSlaveAck();
    IIC_Stop();
}

/**
 * @brief  OLED���ù��λ��
 * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
 * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
 * @retval ��
 */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                 // ����Yλ��
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4)); // ����Xλ�ø�4λ
    OLED_WriteCommand(0x00 | (X & 0x0F));        // ����Xλ�õ�4λ
}

/**
 * @brief  OLED����
 * @param  ��
 * @retval ��
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
 * @brief  OLED��ʾһ���ַ�
 * @param  Line ��λ�ã���Χ��1~4
 * @param  Column ��λ�ã���Χ��1~16
 * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
 * @retval ��
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8); // ���ù��λ�����ϰ벿��
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]); // ��ʾ�ϰ벿������
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8); // ���ù��λ�����°벿��
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]); // ��ʾ�°벿������
    }
}

/**
 * @brief  OLED��ʾ�ַ���
 * @param  Line ��ʼ��λ�ã���Χ��1~4
 * @param  Column ��ʼ��λ�ã���Χ��1~16
 * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
 * @retval ��
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
 * @brief  OLED�η�����
 * @retval ����ֵ����X��Y�η�
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  OLED��ʾ���֣�ʮ���ƣ�������
 * @param  Line ��ʼ��λ�ã���Χ��1~4
 * @param  Column ��ʼ��λ�ã���Χ��1~16
 * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
 * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
 * @retval ��
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
 * @param  Line ��ʼ��λ�ã���Χ��1~4
 * @param  Column ��ʼ��λ�ã���Χ��1~16
 * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
 * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
 * @retval ��
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
 * @param  Line ��ʼ��λ�ã���Χ��1~4
 * @param  Column ��ʼ��λ�ã���Χ��1~16
 * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
 * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
 * @retval ��
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief  OLED��ʾ���֣������ƣ�������
 * @param  Line ��ʼ��λ�ã���Χ��1~4
 * @param  Column ��ʼ��λ�ã���Χ��1~16
 * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
 * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
 * @retval ��
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
  * @brief  OLED��ʾ����
  * @param  Line ��λ��
  * @param  Column ��λ��
  * @param  Chinese ��ʾ���ֱ��
  * @retval ��
  */
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t Chinese)
{
	uint8_t i, j;
	
	for(i=0; i<2; i++) {
		OLED_SetCursor(Line+i, Column);	//ѭ��2����ʾ��/�°벿������	
		for (j=0; j<16; j++) {//�ֿ�Ϊ16
			OLED_WriteData(Chinese_16x16[Chinese][j+i*16]);			
		}
	}
}
void OLED_ShowChinese2(uint8_t Line, uint8_t Column, uint8_t Chinese)
{
	uint8_t i, j;
	
	for(i=0; i<1; i++) {
		OLED_SetCursor(Line+i, Column);	//ѭ��2����ʾ��/�°벿������	
		for (j=0; j<8; j++) {//�ֿ�Ϊ16
			OLED_WriteData(Chinese_8x8[Chinese][j+i*8]);			
		}
	}
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
unsigned int j=0;
 unsigned char x,y;   
   
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
  
	for(y=y0;y<y1;y++)
	{
		OLED_SetCursor(y0+y,x0);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WriteData(BMP[j++]);	    	
	    }
	}
} 
/**
 * @brief  OLED��ʼ��
 * @param  ��
 * @retval ��
 */
void OLED_Init(void)
{
   delay_ms(5);
    // IIC_init(); // OLED IIC�ӿڳ�ʼ�� ���������г�ʼ����

    OLED_WriteCommand(0xAE); // �ر���ʾ

    OLED_WriteCommand(0xD5); // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // ���ö�·������
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // ������ʾƫ��
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // ������ʾ��ʼ��

    OLED_WriteCommand(0xA1); // �������ҷ���0xA1���� 0xA0���ҷ���

    OLED_WriteCommand(0xC8); // �������·���0xC8���� 0xC0���·���

    OLED_WriteCommand(0xDA); // ����COM����Ӳ������
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // ���öԱȶȿ���
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // ����Ԥ�������
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // ����VCOMHȡ��ѡ�񼶱�
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // ����������ʾ��/�ر�

    OLED_WriteCommand(0xA6); // ��������/��ת��ʾ

    OLED_WriteCommand(0x8D); // ���ó���
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // ������ʾ

    OLED_Clear(); // OLED����
}
