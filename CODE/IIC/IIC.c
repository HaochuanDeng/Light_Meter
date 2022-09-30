#include "IIC.h"


//IIC���߳�ʼ��
//
//ע�⣺�˺������ʼ��IO�ڡ����ͷ�IIC����
//
//��������
//
//����ֵ����
void IIC_Init(void)
{
	P1M1  |= 0x30;//P1.4��P1.5����Ϊ��©���
	P1M0  |= 0x30;
	P1SR  &= 0xCF;//P1.4��P1.5 IO�ڵ�ƽת���ٶ�����Ϊ����
	
	SCL = 1;      //SCL����
	Delay2us();
	SDA = 1;      //�ͷ�SDA
	Delay2us();
}


//������ʼ�ź�
//
//��������
//
//����ֵ����
void IIC_Start(void)
{
	SDA = 1;  //SDA����
	Delay2us();
	SCL = 1;  //SCL����
	Delay2us();
	SDA = 0;  //SDA����
	Delay2us();
}


//����ֹͣ�ź�
//
//��������
//
//����ֵ����
void IIC_Stop(void)
{
	SDA = 0;  //SDA����
	Delay2us();
	SCL = 1;  //SCL����
	Delay2us();
	SDA = 1;  //�ͷ�SDA
	Delay2us();
}


//�ȴ����豸Ӧ��
//
//��������
//
//����ֵ��u8��0�����豸δӦ��(��ʱ)
//            1�����豸��Ӧ��
u8 IIC_WaitResp(void)
{
	u8 i = 0;
	
  SDA = 1;                      //�ͷ�SDA
	Delay2us();
	SCL = 1;                      //SCL����
	Delay2us();
	while((SDA == 1) && (i < 255))//�ȴ����豸��SDA����
		i++;
	SCL = 0;                      //SCL����
	Delay2us();
	
	//����ֵ����
	if(i == 255)
		return 0;//���豸δӦ��(��ʱ)
	else
		return 1;//���豸��Ӧ��
}


//����豸Ӧ���Ӧ��
//
//������u8 resp��0��������豸Ӧ��
//               1������豸Ӧ��
//
//����ֵ����
void IIC_Resp(u8 resp)
{
	if(resp == 0)     //������豸Ӧ��
	{
		SCL = 0;        //SCL����
		SDA = 1;        //SDA����
		Delay2us();
		SCL = 1;        //SCL����
		Delay2us();
		SCL = 0;        //SCL����
	}
	else if(resp == 1)//����豸Ӧ��
	{
		SCL = 0;        //SCL����
		SDA = 0;        //SDA����
		Delay2us();
		SCL = 1;        //SCL����
		Delay2us();
		SCL = 0;        //SCL����
		SDA = 1;        //�ͷ�SDA
		Delay2us();
	}
}


//����豸дһ���ֽ�(��λ�ȷ�)
//
//������u8 byte����д����ֽ�
//
//����ֵ����
void IIC_WriteByte(u8 byte)
{
	u8 i;
	
	for(i=0; i<8; i++)
	{
		byte = byte << 1;//�������ֽ����λ����CY
		SCL  = 0;        //SCL����
		Delay2us();
		SDA  = CY;       //SDA�ͳ�CY
		Delay2us();
		SCL  = 1;        //SCL����
		Delay2us();
	}
	SCL = 0;           //SCL����
	Delay2us();
	SDA = 1;           //�ͷ�SDA
	Delay2us();
}


//����豸��һ���ֽ�(�Ƚ��յ��ķ��ڸ�λ)
//
//��������
//
//����ֵ��u8���������ֽ�
u8 IIC_ReadByte(void)
{
	u8 i,k = 0;
	
	SCL = 0;  //SCL����
	Delay2us();
	SDA = 1;  //�ͷ�SDA
	for(i=0; i<8; i++)
	{
		SCL = 1;           //SCL����
		Delay2us();
		k = (k << 1) | SDA;//�����ȡ�ı���
		SCL = 0;           //SCL����
		Delay2us();
	}
	
	return k;
}

