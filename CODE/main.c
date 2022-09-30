#include "LED.h"
#include "RCC.h"
#include "EXTI.h"
#include "EEPROM.h"
#include "ADC.h"
#include "DELAY.h"
#include "TIMER.h"
#include "VEML7700.h"
#include "OLED.h"
#include "AE.h"
#include "STATE_MACHINE.h"


u16   temp_u;           //16λ�޷�����ʱ����
float temp_f;           //32λ������ʱ����


//���������������
void main(void)
{
	//Ӳ����ʼ��
	RCC_Init();           //��ʼ���ⲿ��������(32MHz)
	ADC_Init();           //��ʼ��Ƭ��ADC
	EEPROM_Init();        //��ʼ��Ƭ��EEPROM
	IIC_Init();           //��ʼ��IIC����
	OLED_Init();          //��ʼ��OLEDģ��
	
	//��ʾ�̼��汾��
	OLED_Disp(1,1,10,0,1);//v1.01
	DelayXms(400);        //�ʵ���ʱ
	
	//��ʾ��ǰ��ص���(0~9999,�ٷֱ���)
	temp_f = ((6.6*(ADC_Convert(1)/1024.0))-2.6)/1.5;             //���㵱ǰ�����ٷֱ�
	temp_u = (u16)(temp_f*10000);                                 //�Ŵ�10000�������ڷ����λ��
	OLED_Disp(1,temp_u/1000,temp_u/100%10,temp_u/10%10,temp_u%10);//OLED��ʾ
	
	//׼������״̬��
	LED_Config();         //��ʼ��LED
	VEML7700_Init();      //��ʼ�������⴫����VEML7700
	AE_Init();            //��ʼ��AE
	SM_Init();            //��ʼ��״̬��
	T2_Init();            //��ʼ��T2��ʱ��
	EXTI_Init();          //��ʼ��EXTI
	
	//����״̬��
	while(1)
	{
		SM_Run();           //��ѯ״̬��
		DelayXms(10);       //�ʵ���ʱ��ʹ�ж��ܹ���ISR��Ӧ
	}
}

