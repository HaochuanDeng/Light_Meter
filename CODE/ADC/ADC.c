#include "ADC.h"


//��ʼ��ADC
//
//��������
//
//����ֵ����
void ADC_Init(void)
{	
	P1M1  |= 0x09;//P1.0->ADC0(��������ģʽ)��P1.3->ADC3(��������ģʽ)
	P1M0  &= 0xF6;
	P1IE  &= 0xF6;//P1.0��P1.3��ֹ�����ź�����
	
	ADCCFG = 0x21;//ת������Ҷ��롢ADCʱ��Ƶ��8MHz
	ADCTIM = 0xFF;//T_setup=2T��T_hold=4T��T_duty=32T��T_convert=10T��F_adc=380.952KHz
}


//ADC��ָ��ͨ����ģ���ѹ����ת��
//
//������u8 chan��0��3.3Vϵͳ�����ѹ | ADC0 | P1.0
//               1�� 0.5��﮵�ص�ѹ | ADC3 | P1.3
//
//����ֵ��u16��ת���������10λ��Ч
u16 ADC_Convert(u8 chan)
{
	//����ADCת��ͨ��
	if(chan == 0)       //3.3V��ѹ|ADC0|P1.0
	{
		ADC_CONTR &= 0xF0;//���ADC_CHS[3:0]������ת��ͨ��ΪADC0
	}
	else if(chan == 1)  //3V��ѹ|ADC3|P1.3
	{
		ADC_CONTR &= 0xF0;//���ADC_CHS[3:0]
		ADC_CONTR |= 0x03;//����ת��ͨ��ΪADC3
	}
	
	//��ʼת��
	ADC_CONTR |= 0x80;                //��ADC��Դ
	DelayXms(1);                      //�ȴ�ADC��Դ�ȶ�
	ADC_CONTR |= 0x40;                //����ADCת��
	while((ADC_CONTR & 0x20) == 0x00);//�ȴ�ת������
	ADC_CONTR &= 0xDF;                //���ת��������־λ
	ADC_CONTR &= 0x7F;                //�ر�ADC��Դ
	
	return (((u16)ADC_RES<<8) | ((u16)ADC_RESL));
}

