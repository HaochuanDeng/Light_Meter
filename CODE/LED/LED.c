#include "LED.h"


//����LED״̬
//
//������u8 LED_STATUS��1  ����LED
//                     0  Ϩ��LED
//
//����ֵ����
static void LED_SET(u8 LED_STATUS)
{
	if(LED_STATUS == 0)     //Ϩ��LED
		P11 = 1;
	else if(LED_STATUS == 1)//����LED
    P11 = 0; 
}


//��ʼ��LED�õ���GPIO��
//
//��������
//
//����ֵ����
void LED_Config(void)
{
	P1DR &= 0xFD;//��ǿ��������
	P1M1 &= 0xFD;//�������ģʽ��������20mA
	P1M0 |= 0x02;
	LED_SET(0);  //Ĭ��Ϩ��LED
}


//��˸LED
//
//ע�⣺1��LED��˸��������(ά��dely����)����(ά��dely����)
//
//������u8 num��  ��˸����(1~255)
//      u16 dely��LED״̬��ת���ʱ��(��λ��ms)(0~65535)
//
//����ֵ����
void LED_Blink(u8 num, u16 dely)
{
	u8 x;
	
	for(x=0;x<num;x++)
	{
		LED_SET(1);
		DelayXms(dely);
		LED_SET(0);
		DelayXms(dely);
	}
}

