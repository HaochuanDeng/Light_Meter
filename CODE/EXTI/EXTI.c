#include "EXTI.h"


volatile u8 key_flag[6]={0};//(0~1:��ѡ�񡱰����㰴��˫����־λ��2~3:���ӡ������㰴��˫����־λ��4~5:�����������㰴��˫����־λ)
extern u8 T2_BUSY_FLAG;     //T2��ʱ��ռ�ñ�־λ(0��δռ�ã�1����ѡ�񡱰���ռ�ã�2�����ӡ�����ռ�ã�3������������ռ��) 
extern u8 disp_mode;        //OLED��ʾģʽ(0~1)


//��ʼ���ⲿ�ж�0��1��2Ϊ�½��ش��������ȼ���Ϊ���
//
//��������
//
//����ֵ����
void EXTI_Init(void)
{
	IT0       = 1;   //�½��ش���
	EX0       = 1;   //�����ⲿ�ж�0�����жϣ�P3.2(׼˫���)
	
	IT1       = 1;   //�½��ش���
	EX1       = 1;   //�����ⲿ�ж�1�����жϣ�P3.3(׼˫���)
	
	IP       &= 0xFA;//�ⲿ�ж�0��1���ȼ�����Ϊ��ͼ�
	IPH      &= 0xFA;
	
	INT_CLKO |= 0x10;//�����ⲿ�ж�2(P3.6׼˫���)�����жϣ����ȼ�Ĭ���ҹ̶�Ϊ��ͼ���Ĭ���ҹ̶�Ϊ�½��ش���
	
	P3M1     &= 0xB3;//P3.2��P3.3��P3.6��Ϊ׼˫���
	P3M0     &= 0xB3;
	
	EA        = 1;   //�����ж�
}


//�ⲿ�ж�0(�½����ж�)ISR(��ѡ�񡱰���)
//
//��������
//
//����ֵ����
void INT0_Routine(void) interrupt 0
{
	if(T2_BUSY_FLAG == 1)             //T2��ʱ��������
	{
		AUXR &= 0xEF;                   //ֹͣT2��ʱ��
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2��ʱ����װ
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED������ʾ
		  key_flag[1]  = 1;             //����˫����־λ
		T2_BUSY_FLAG   = 0;             //�ͷ�T2��ʱ��ռ�ñ�־
	}
	else if(T2_BUSY_FLAG == 0)        //T2��ʱ��δ����
	{
		AUXR |= 0x10;                   //����T2��ʱ��
		T2_BUSY_FLAG = 1;               //��ѡ�񡱰���ռ��T2��ʱ��
	}
}


//�ⲿ�ж�1(�½����ж�)ISR(���ӡ�����)
//
//��������
//
//����ֵ����
void INT1_Routine(void) interrupt 2
{
	if(T2_BUSY_FLAG == 2)             //T2��ʱ��������
	{
		AUXR &= 0xEF;                   //ֹͣT2��ʱ��
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2��ʱ����װ
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED������ʾ
		{
			LED_Blink(1,50);              //��˸LEDָʾ�û�
		  key_flag[3]  = 1;             //����˫����־λ
		}
		T2_BUSY_FLAG   = 0;             //�ͷ�T2��ʱ��ռ�ñ�־
	}
	else if(T2_BUSY_FLAG == 0)        //T2��ʱ��δ����
	{
		AUXR |= 0x10;                   //����T2��ʱ��
		T2_BUSY_FLAG = 2;               //���ӡ�����ռ��T2��ʱ��
	}
}


//�ⲿ�ж�2(�½����ж�)ISR(����������)
//
//��������
//
//����ֵ����
void INT2_Routine(void) interrupt 10
{
	if(T2_BUSY_FLAG == 3)             //T2��ʱ��������
	{
		AUXR &= 0xEF;                   //ֹͣT2��ʱ��
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2��ʱ����װ
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED������ʾ
		{
			LED_Blink(1,50);              //��˸LEDָʾ�û�
		  key_flag[5]  = 1;             //����˫����־λ
		}
		T2_BUSY_FLAG   = 0;             //�ͷ�T2��ʱ��ռ�ñ�־
	}
	else if(T2_BUSY_FLAG == 0)        //T2��ʱ��δ����
	{
		AUXR |= 0x10;                   //����T2��ʱ��
		T2_BUSY_FLAG = 3;               //����������ռ��T2��ʱ��
	}
}

