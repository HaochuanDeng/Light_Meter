#include "AE.h"


//AE�㷨���β�����
volatile u8 AE_SAMPLE_SIZE;

//��Ȧ��ֵ��Ӧ��
float xdata F_Stop [7];

//����ʱ���Ӧ��(��λ��s)
float xdata S_Speed[16];

//ISO/ASA��Ӧ��
float xdata ISO    [21];

//����У׼ϵ��K(��ֵԽ���ع�Խ��)(Ĭ�ϣ�20.0)
union K_union
{
	u8    x[4];
	float t;
}K;


//��ʼ��AE
//
//ע�⣺����Ȧ�����š�ISO/ASA���ձ��EEPROM�ж���
//
//��������
//
//����ֵ����
void AE_Init(void)
{
	u8    i;
	union temp
  {
	  u8    x[4];
	  float t;
  }D;
 
  //��EEPROM
  for(i=0;i<44;i++)
	{
		EEPROM_Read(D.x,1,i*4,4);
		
		if(i < 7)                    //��Ȧ��Ӧ��
			F_Stop[i]    = D.t;
		else if((i > 6)  && (i < 23))//���Ŷ�Ӧ��
			S_Speed[i-7] = D.t;
		else if((i > 22) && (i < 44))//ISO��Ӧ��
			ISO[i-23]    = D.t;
	}
}


//����ISO����Ȧ���������ʱ��(��Ȧ���Ȳ��)
//
//ע�⣺ʹ��ESAM�㷨����ָ��ƽ��������ƽ���㷨
//
//������u8 f��  ��ǰ��Ȧ(��������)(0~6)
//      u8 iso����ǰISO(��������)(0~20)
//
//����ֵ��float������õ��Ŀ���ʱ��(��λ��s)
float ESAM_Calc_S_Speed(u8 f, u8 iso)
{
	u8 x;
	float data sum                             =   0;
	float data temp       [AE_SAMPLE_SIZE_MAX] = {0};
	float data temp_smooth[AE_SAMPLE_SIZE_MAX] = {0};
	
	//�Ի�������ж�β���
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = Lux_Calc();//����һ��
		LED_Blink(1,10);     //��˸LEDָʾ�û�
	}
	
	//�����عⷽ�̼������ʱ��
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = ((F_Stop[f]*F_Stop[f])*K.t) / (temp[x]*ISO[iso]);
	}
	
	//�Լ�����Ŀ���ʱ�����ָ��ƽ�� & ����ƽ��
	temp_smooth[0] = temp[0];//��̬
	sum           += temp_smooth[0];
	for(x=1;x<AE_SAMPLE_SIZE;x++)
	{
		temp_smooth[x] = (EXP_SMOOTH_PARA*temp[x-1]) + ((1.0-EXP_SMOOTH_PARA)*temp_smooth[x-1]);
		sum           += temp_smooth[x];
	}
	sum /= AE_SAMPLE_SIZE;
	
	//��˸LEDָʾ�û��������
	LED_Blink(2,30);
	
	return sum;//���ؿ���ʱ��
}


//����ISO������ʱ�䣬�����Ȧ(�������Ȳ��)
//
//ע�⣺ʹ��ESAM�㷨����ָ��ƽ��������ƽ���㷨
//
//������u8 s��  ��ǰ����ʱ��(��������)(0~15)
//      u8 iso����ǰISO(��������)(0~20)
//
//����ֵ��float������õ��Ĺ�Ȧ
float ESAM_Calc_F_Stop(u8 s, u8 iso)
{
	u8 x;
	float data sum                             =   0;
	float data temp       [AE_SAMPLE_SIZE_MAX] = {0};
	float data temp_smooth[AE_SAMPLE_SIZE_MAX] = {0};
	
	//�Ի�������ж�β���
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = Lux_Calc();//����һ��
		LED_Blink(1,10);     //��˸LEDָʾ�û�
	}
	
	//�����عⷽ�̼����Ȧ
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = sqrt((temp[x]*ISO[iso]*S_Speed[s]) / K.t);
	}
	
	//�Լ�����Ĺ�Ȧ����ָ��ƽ�� & ����ƽ��
	temp_smooth[0] = temp[0];//��̬
	sum           += temp_smooth[0];
	for(x=1;x<AE_SAMPLE_SIZE;x++)
	{
		temp_smooth[x] = (EXP_SMOOTH_PARA*temp[x-1]) + ((1.0-EXP_SMOOTH_PARA)*temp_smooth[x-1]);
		sum           += temp_smooth[x];
	}
	sum /= AE_SAMPLE_SIZE;
	
	//��˸LEDָʾ�û��������
	LED_Blink(2,30);
	
	return sum;//���ع�Ȧ
}

