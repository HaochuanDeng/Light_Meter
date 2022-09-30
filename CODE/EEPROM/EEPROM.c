#include "EEPROM.h"


//��ʼ��EEPROM
//
//��������
//
//����ֵ����
void EEPROM_Init(void)
{
	IAP_TPS    = 0x20;//EEPROM����Ƶ�� = 32MHz / 1MHz = 32 = 0x20
	IAP_CONTR |= 0x80;//ʹ��EEPROM����
}


//д��EEPROM��0~7����������ƫ�Ƶ�ַ
//
//ע�⣺д1���ֽڣ�40us
//    ����1�������� 6ms
//
//������u8* date����д�������ָ��
//      u8  sect����д�������(0~7)
//      u16 offset������ƫ��(0~511)
//      u16 num�� ��д����ֽ��������512(ͬʱӦ����Խ�絽��һ������)
//
//����ֵ��u8��0 д��ɹ�
//            1 ��������
//            2 ��������ʧ��
//            3 д��ʧ��
u8 EEPROM_Write(u8* date, u8 sect, u16 offset, u16 num)
{
	u16 i;
	
	if((sect > 7) || (offset > 511) || (num > 512))
		return 1;                            //��������
	
	//��������
	IAP_ADDRH = (u8)((sect*512)>>8);       //��������������ַ
	IAP_ADDRL = (u8)(sect*512);
	IAP_CMD   = 0x03;                      //����������������
	IAP_TRIG  = 0x5A;                      //��������
	IAP_TRIG  = 0xA5;
	if((IAP_CONTR & 0x10) == 0x10)
	{
		IAP_CONTR &= 0xEF;                   //���EEPROM����ʧ�ܱ�־λ
		return 2;                            //��������ʧ��
	}
	
	//����д��
	IAP_CMD = 0x02;                        //��������д������
	for(i=sect*512+offset; i<sect*512+num+offset; i++)
	{
		IAP_ADDRH = (u8)(i>>8);              //���ô�д���Ŀ���ַ
	  IAP_ADDRL = (u8)i;
		IAP_DATA  = date[i-sect*512-offset]; //���ô�д������
		IAP_TRIG  = 0x5A;                    //��������
	  IAP_TRIG  = 0xA5;
		if((IAP_CONTR & 0x10) == 0x10)
	  {
		  IAP_CONTR &= 0xEF;                 //���EEPROM����ʧ�ܱ�־λ
		  return 3;                          //д��ʧ��
	  }
	}
	
	return 0;                              //д��ɹ�
}


//��EEPROM��0~7����������ƫ�Ƶ�ַ
//
//ע�⣺��1���ֽڣ�4��ϵͳʱ������
//
//������u8* date�����������ݽ��ŵ�������ָ����ָ���������
//      u8  sect������ȡ������(0~7)
//      u16 offset������ƫ����(0~511)
//      u16 num�� ����ȡ���ֽ��������512(ͬʱӦ����Խ�絽��һ������)
//
//����ֵ��u8��0 ��ȡ�ɹ�
//            1 ��������
//            2 ��ȡʧ��
u8 EEPROM_Read(u8* date, u8 sect, u16 offset, u16 num)
{
	u16 i;
	
	if((sect > 7) || (offset > 511) || (num > 512))
		return 1;                          //��������
	
	IAP_CMD   = 0x01;                    //����������ȡ����
	for(i=sect*512+offset; i<sect*512+num+offset; i++)
	{
		IAP_ADDRH = (u8)(i>>8);            //���ô�д���Ŀ���ַ
	  IAP_ADDRL = (u8)i;
		IAP_TRIG  = 0x5A;                  //��������
	  IAP_TRIG  = 0xA5;
		if((IAP_CONTR & 0x10) == 0x10)
	  {
		  IAP_CONTR &= 0xEF;               //���EEPROM����ʧ�ܱ�־λ
		  return 2;                        //��ȡʧ��
	  }
		date[i-sect*512-offset] = IAP_DATA;//��������������
	}
	
	return 0;                            //��ȡ�ɹ�
}

