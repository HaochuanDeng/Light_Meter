#include "STATE_MACHINE.h"


//����   ˵��                  ��Χ
//
//0��    ��Ȧֵ                (0~6)
//1~4��  ��Ȧ����ֵ             float
//5��    ����ʱ��              (0~15)
//6~9��  ����ʱ�����ֵ         float
//10��   ISO/ASA               (0~20)
//11��   ��ǰ��ʾλ             (0~4)
//12��   �߼����������ÿ���λ (0~1)
//13��   ���ģʽѡ��λ         (0~1)
//14��   AE�㷨���β�����       (1~AE_SAMPLE_SIZE_MAX)
//15~18������У׼ϵ��         float
volatile u8 ROM_DATA[19]; //�����Ƭ��EEPROM(������0������ʼ��19���ֽ�)
volatile u8 state     = 0;//״̬��״̬λ(0~1)
volatile u8 disp_mode = 0;//OLED��ʾģʽ(0~1)
union       F_S_union     //���õ��Ĺ�Ȧ�������ֵ
{
	u8    x[4];
	float t;
}F,S;

//0~1:��ѡ�񡱰����㰴��˫����־λ
//2~3:��  �ӡ������㰴��˫����־λ
//4~5:��  ���������㰴��˫����־λ
extern u8    key_flag[6];   //������־λ
extern u8    AE_SAMPLE_SIZE;//AE�㷨���β�����
extern float F_Stop  [7];   //��Ȧ��ֵ��Ӧ��
extern float S_Speed [16];  //����ʱ���Ӧ��(��λ��s)
extern float ISO     [21];  //ISO/ASA��Ӧ��
extern union K_union        //����У׼ϵ��K(��ֵԽ���ع�Խ��)(Ĭ�ϣ�20.0)
{
	u8    x[4];
	float t;
}K;


//��ʼ��״̬��
//
//ע�⣺��EEPROM�м������ݣ���ʼ����ر���
//
//��������
//
//����ֵ����
void SM_Init(void)
{
	//����EEPROM����
	EEPROM_Read(ROM_DATA,0,0,19);
	
	//��ر�����ʼ��
	F.x[0]         = ROM_DATA[1]; //��Ȧ����ֵ
	F.x[1]         = ROM_DATA[2];
	F.x[2]         = ROM_DATA[3];
	F.x[3]         = ROM_DATA[4];
	S.x[0]         = ROM_DATA[6]; //����ʱ�����ֵ
	S.x[1]         = ROM_DATA[7];
	S.x[2]         = ROM_DATA[8];
	S.x[3]         = ROM_DATA[9];
	AE_SAMPLE_SIZE = ROM_DATA[14];//AE�㷨���β�����
	K.x[0]         = ROM_DATA[15];//����У׼ϵ��
	K.x[1]         = ROM_DATA[16];
	K.x[2]         = ROM_DATA[17];
	K.x[3]         = ROM_DATA[18];
}


//״̬��
//
//ע�⣺���������в�������
//
//��������
//
//����ֵ����
void SM_Run(void)
{
	u8 d[4];
	
	EA = 0;                          //�����ٽ���
	
	/****************�߼�����****************/
	//״̬ת�� & ��������
	if(state == 0)                   //״̬0��������ά��
	{
		if(key_flag[0]      == 1)      //���ù�Ȧ�����š�ISO
		{
			key_flag[0]   = 0;
			if((ROM_DATA[11] < 3) || (ROM_DATA[12] == 1))
			{
			  disp_mode   = 1;
			  state       = 1;
			}
		}
		else if(key_flag[1] == 1)      //���
		{
			key_flag[1] = 0;
			if(ROM_DATA[13]   == 0)      //��Ȧ����ģʽ���
				S.t   = ESAM_Calc_S_Speed(ROM_DATA[0],ROM_DATA[10]);
			else                         //��������ģʽ���
				F.t   = ESAM_Calc_F_Stop (ROM_DATA[5],ROM_DATA[10]);
		}
		else if(key_flag[2] == 1)      //��ʾ��һ��
		{
			key_flag[2] = 0;
			if(ROM_DATA[11] > 0)
				ROM_DATA[11]--;
		}
		else if(key_flag[3] == 1)      //��Ȧ/��������ģʽ�л�
		{
			key_flag[3]    = 0;
			if(ROM_DATA[13]   == 1)
				ROM_DATA[13] = 0;          //�л�Ϊ��Ȧ����ģʽ
			else
				ROM_DATA[13] = 1;          //�л�Ϊ��������ģʽ
		}
		else if(key_flag[4] == 1)      //��ʾ��һ��
		{
			key_flag[4] = 0;
			if(ROM_DATA[11] < 2)
				ROM_DATA[11]++;
			else if((ROM_DATA[11] < 4) && (ROM_DATA[12] == 1))
				ROM_DATA[11]++;
		}
		else if(key_flag[5] == 1)      //��/�رղ���߼���������
		{
			key_flag[5]    = 0;
			if(ROM_DATA[12]   == 1)
				ROM_DATA[12] = 0;          //�رղ���߼���������
			else
				ROM_DATA[12] = 1;          //�򿪲���߼���������
		}
		else
			goto L1;
	}
	else if(state == 1)              //״̬1�����ù�Ȧ�����š�ISO��AE�㷨���β�����������У׼ϵ��
	{
		if(key_flag[0]      == 1)      //�˳�����
		{
			key_flag[0] = 0;
			disp_mode   = 0;
			state       = 0;
		}
		else if(key_flag[2] == 1)      //��
		{
			key_flag[2] = 0;
			if(ROM_DATA[11]      == 0)   //�޸Ĺ�Ȧ
			{
				if(ROM_DATA[0]  < 6)
					ROM_DATA[0]++;
			}
			else if(ROM_DATA[11] == 1)   //�޸Ŀ���
			{
				if(ROM_DATA[5]  > 0)
					ROM_DATA[5]--;
			}
			else if(ROM_DATA[11] == 2)   //�޸�ISO
			{
				if(ROM_DATA[10] < 20)
					ROM_DATA[10]++;
			}
			else if(ROM_DATA[11] == 3)   //�޸�AE�㷨���β�����
			{
				if(AE_SAMPLE_SIZE < AE_SAMPLE_SIZE_MAX)
					AE_SAMPLE_SIZE++;
			}
			else if(ROM_DATA[11] == 4)   //�޸Ĳ���У׼ϵ��
			{
				if(K.t < K_MAX)
					K.t += 0.5;
			}
		}
		else if(key_flag[4] == 1)      //��
		{
			key_flag[4] = 0;
			if(ROM_DATA[11]      == 0)   //�޸Ĺ�Ȧ
			{
				if(ROM_DATA[0] > 0)
					ROM_DATA[0]--;
			}
			else if(ROM_DATA[11] == 1)   //�޸Ŀ���
			{
				if(ROM_DATA[5] < 15)
					ROM_DATA[5]++;
			}
			else if(ROM_DATA[11] == 2)   //�޸�ISO
			{
				if(ROM_DATA[10] > 0)
					ROM_DATA[10]--;
			}
			else if(ROM_DATA[11] == 3)   //�޸�AE�㷨���β�����
			{
				if(AE_SAMPLE_SIZE > AE_SAMPLE_SIZE_MIN)
					AE_SAMPLE_SIZE--;
			}
			else if(ROM_DATA[11] == 4)   //�޸Ĳ���У׼ϵ��
			{
				if(K.t > K_MIN)
					K.t -= 0.5;
			}
		}
		else
			goto L1;
	}
	
	
	/****************��ʾ����****************/
	Disp_NOR_INV(disp_mode);                                               //OLED��ʾģʽ����
	
	//��ʾ��Ӧ����
	if(     (ROM_DATA[11] == 0) && (ROM_DATA[13] == 1) && (disp_mode == 0))//��ʾ��Ȧ����������ģʽ��  ������ʾ
	{
		if(F.t < 0.01)                                                       //��Ȧ��ֵС��С����ʾ��Χ
		{
			OLED_Disp(0,14,14,14,11);
		}
		else if(F.t > 99.9)                                                  //��Ȧ��ֵ����С����ʾ��Χ
		{
			INT_Conv(F.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //��Ȧ��ֵλ��С����ʾ��Χ
		{
			FLOAT_Conv(F.t,d);
		  OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 0)                                            //��ʾ��Ȧ����Ȧ����ģʽ �� ��ת��ʾ
	{
		if(ROM_DATA[0] < 3)                                                  //��Ȧ��С��
		{
			FLOAT_Conv(F_Stop[ROM_DATA[0]],d);
		}
		else                                                                 //��Ȧ������
		{
			INT_Conv(F_Stop[ROM_DATA[0]],d);
		}
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	}
	else if((ROM_DATA[11] == 1) && (ROM_DATA[13] == 0) && (disp_mode == 0))//��ʾ���ţ���Ȧ����ģʽ��  ������ʾ
	{
		if(S.t < 0.0001)                                                     //���Ź���(��ֵ���������ֵ��С)
		{
			OLED_Disp(0,14,14,14,11);
		}
		else if(S.t > 99.9)                                                  //���ų���С����ʾ��Χ
		{
			INT_Conv(S.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else if(S.t <= 1.0)                                                  //����С�ڻ����1s
		{
			INT_Conv(1/S.t,d);
			OLED_Disp(1,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //����λ��С����ʾ��Χ
		{
			FLOAT_Conv(S.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 1)                                            //��ʾ���ţ���������ģʽ �� ��ת��ʾ
	{
		if(ROM_DATA[5] < 5)                                                  //����Ϊ����
		{
			INT_Conv(S_Speed[ROM_DATA[5]],d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //����ΪС��
		{
			INT_Conv(1/S_Speed[ROM_DATA[5]],d);
			OLED_Disp(1,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 2)                                            //��ʾISO
	{
		if(ROM_DATA[10] < 2)                                                 //ISOΪС��
		{
			FLOAT_Conv(ISO[ROM_DATA[10]],d);
		}
		else                                                                 //ISOΪ����
		{
			INT_Conv(ISO[ROM_DATA[10]],d);
		}
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	 }
	else if( ROM_DATA[11] == 3)                                            //��ʾAE�㷨���β�����
	{
		OLED_Disp(0,14,14,14,AE_SAMPLE_SIZE);
	}
	else if( ROM_DATA[11] == 4)                                            //��ʾ����У׼ϵ��
	{
		FLOAT_Conv(K.t,d);
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	}
	

	/****************����д�ز���****************/
	//д��EEPROM
	ROM_DATA[1]  = F.x[0];        //��Ȧ����ֵ
	ROM_DATA[2]  = F.x[1];
	ROM_DATA[3]  = F.x[2];
	ROM_DATA[4]  = F.x[3];
	ROM_DATA[6]  = S.x[0];        //����ʱ�����ֵ
	ROM_DATA[7]  = S.x[1];
	ROM_DATA[8]  = S.x[2];
	ROM_DATA[9]  = S.x[3];
	ROM_DATA[14] = AE_SAMPLE_SIZE;//AE�㷨���β�����
	ROM_DATA[15] = K.x[0];        //����У׼ϵ��
	ROM_DATA[16] = K.x[1];
	ROM_DATA[17] = K.x[2];
	ROM_DATA[18] = K.x[3];
	EEPROM_Write(ROM_DATA,0,0,19);
	
L1:	
	EA = 1;                       //�˳��ٽ���
}

