#include "DELAY.h"


//�����ʱ1ms
//
//ע�⣺@32MHz(ϵͳʱ��)
//
//��������
//
//����ֵ����
static void Delay1ms(void)
{
	u8 i,j;

	_nop_();
	i = 42;
	j = 140;
	do
	{
		while(--j);
	}while(--i);
}


//�����ʱ X ms
//
//ע�⣺@32MHz(ϵͳʱ��)
//
//������u16 x����ʱ X ms(0~65535)
//
//����ֵ����
void DelayXms(u16 x)
{
	for(x=x;x>0;x--)
	  Delay1ms();
}


//�����ʱ2us
//
//ע�⣺@32MHz(ϵͳʱ��)�����ڿ������ģ���IIC����SCLʱ��Ƶ�ʣ�Tscl = 2 * 2us = 4us��Fscl = 1s / Tscl = 250KHz
//
//��������
//
//����ֵ����
void Delay2us(void)
{
	u8 i;

	_nop_();
	i = 19;
	while(--i);
}

