#include "VEML7700.h"


//дVEML7700�Ĵ���
//
//ע�⣺���ô˺���ǰ��Ҫ���� IIC_Init() ������ʼ��IIC����(��ʼ��һ�μ���)
//
//������u8  addr����д��ļĴ�����ַ(���� .h �ļ��ж���)
//      u16 date����д�������(16λ)
//
//����ֵ����
static void VEML7700_Write(u8 addr, u16 date)
{
	IIC_Start();                   //������ʼ�ź�
	IIC_WriteByte(VEML7700_ADDR_W);//���ʹӻ���ַ(д)
	IIC_WaitResp();                //�ȴ��ӻ���Ӧ
	IIC_WriteByte(addr);           //���ͼĴ�����ַ
	IIC_WaitResp();                //�ȴ��ӻ���Ӧ
	IIC_WriteByte((u8)date);       //����LSB
	IIC_WaitResp();                //�ȴ��ӻ���Ӧ
	IIC_WriteByte((u8)(date>>8));  //����MSB
	IIC_WaitResp();                //�ȴ��ӻ���Ӧ
	IIC_Stop();                    //����ֹͣ�ź�
}


//��VEML7700�Ĵ���
//
//ע�⣺���ô˺���ǰ��Ҫ���� IIC_Init() ������ʼ��IIC����(��ʼ��һ�μ���)
//
//������u8 addr������ȡ�Ĵ����ĵ�ַ(���� .h �ļ��ж���)
//
//����ֵ��u16��0xXXXX����ȡ��������(16λ)
//             0x0000����ȡ����ʧ��(Ҳ�п��ܶ�����ȷ��ֵ��ȫΪ0)
static u16 VEML7700_Read(u8 addr)
{
	u8 LSB,MSB;
	
	IIC_Start();                        //������ʼ�ź�
	IIC_WriteByte(VEML7700_ADDR_W);     //���ʹӻ���ַ(д)
	if(IIC_WaitResp() == 0)             //�ȴ��ӻ���Ӧ
		return 0;                         //�ӻ�δ��Ӧ������
	IIC_WriteByte(addr);                //���ʹ���ȡ�ļĴ�����ַ
	if(IIC_WaitResp() == 0)             //�ȴ��ӻ���Ӧ
		return 0;                         //�ӻ�δ��Ӧ������
	IIC_Start();                        //���²�����ʼ�ź�
	IIC_WriteByte(VEML7700_ADDR_R);     //���ʹӻ���ַ(��)
	if(IIC_WaitResp() == 0)             //�ȴ��ӻ���Ӧ
		return 0;                         //�ӻ�δ��Ӧ������
	LSB = IIC_ReadByte();               //����LSB
	IIC_Resp(1);                        //��ӻ�Ӧ��
	MSB = IIC_ReadByte();               //����MSB
	IIC_Resp(0);                        //����ӻ�Ӧ��
	IIC_Stop();                         //����ֹͣ�ź�
	
	return (((u16)MSB)<<8) | ((u16)LSB);//���ض���������
}


//���� ALS ����
//
//������VEML7700_ALS_GAIN_TypeDef gain��
//       ALS_gain_1   = ALS gain x 1
//       ALS_gain_2   = ALS gain x 2
//       ALS_gain_1_8 = ALS gain x (1/8)
//       ALS_gain_1_4 = ALS gain x (1/4)
//
//����ֵ����
void ALS_GAIN(VEML7700_ALS_GAIN_TypeDef gain)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//��ȡ�Ĵ���
	temp &= 0xE7FF;                   //������λ
	temp |= gain << 11;               //�������λ
	
	VEML7700_Write(ALS_CONF_0,temp);  //д��Ĵ���
}


//���� ASL ����ʱ��
//
//������VEML7700_ALS_IT_TypeDef it��
//       ms25  = 25ms
//       ms50  = 50ms
//       ms100 = 100ms
//       ms200 = 200ms
//       ms400 = 400ms
//       ms800 = 800ms
//
//����ֵ����
void ALS_IT(VEML7700_ALS_IT_TypeDef it)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//��ȡ�Ĵ���
	temp &= 0xFC3F;                   //������λ
	temp |= it << 6;                  //�������λ
	
	VEML7700_Write(ALS_CONF_0,temp);  //д��Ĵ���
}

/*
//���� ALS_PERS �˻�ϵ��
//
//������VEML7700_ALS_PERS_TypeDef pers��
//       p1 = 1
//       p2 = 2
//       p4 = 4
//       p8 = 8
//
//����ֵ����
void ALS_PERS(VEML7700_ALS_PERS_TypeDef pers)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//��ȡ�Ĵ���
	temp &= 0xFFCF;                   //������λ
	temp |= pers << 4;                //�������λ
	
	VEML7700_Write(ALS_CONF_0,temp);  //д��Ĵ���
}
*/

//���� ALS �ж�ʹ��/ʧ��
//
//������VEML7700_ALS_INT_EN_TypeDef en��
//       EN  = ALS INT enable
//       DIS = ALS INT disable
//
//����ֵ����
void ALS_INT_EN(VEML7700_ALS_INT_EN_TypeDef en)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//��ȡ�Ĵ���
	temp &= 0xFFFD;                   //������λ
	temp |= en << 1;                  //�������λ
	
	VEML7700_Write(ALS_CONF_0,temp);  //д��Ĵ���
}


//���� ALS ����ʹ��/ʧ��
//
//������VEML7700_ALS_SD_TypeDef power��
//       ON  = ALS power on
//       OFF = ALS shut down
//
//����ֵ����
void ALS_SD(VEML7700_ALS_SD_TypeDef power)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//��ȡ�Ĵ���
	temp &= 0xFFFE;                   //������λ
	temp |= power;                    //�������λ
	
	VEML7700_Write(ALS_CONF_0,temp);  //д��Ĵ���
}

/*
//���� INT ��������ֵ
//
//������u16 H_Thre�������õĸ���ֵ
//
//����ֵ����
void ALS_H_THRE(u16 H_Thre)
{
	VEML7700_Write(ALS_WH,H_Thre);//д��Ĵ���
}


//���� INT ��������ֵ
//
//������u16 L_Thre�������õĵ���ֵ
//
//����ֵ����
void ALS_L_THRE(u16 L_Thre)
{
	VEML7700_Write(ALS_WL,L_Thre);//д��Ĵ���
}


//���� PSM ʡ��ģʽ
//
//������VEML7700_PSM_TypeDef mode��
//       m1 = mode 1
//       m2 = mode 2
//       m3 = mode 3
//       m4 = mode 4
//
//����ֵ����
void PSM(VEML7700_PSM_TypeDef mode)
{
	u16 temp;
	
	temp  = VEML7700_Read(POWER_SAVING);//��ȡ�Ĵ���
	temp &= 0xFFF9;                     //������λ
	temp |= mode << 1;                  //�������λ
	
	VEML7700_Write(POWER_SAVING,temp);  //д��Ĵ���
}
*/

//���� PSM_EN ʡ��ģʽʹ��/ʧ��
//
//������VEML7700_PSM_EN_TypeDef en��
//       enable  = enable
//       disable = disable
//
//����ֵ����
void PSM_EN(VEML7700_PSM_EN_TypeDef en)
{
	u16 temp;
	
	temp  = VEML7700_Read(POWER_SAVING);//��ȡ�Ĵ���
	temp &= 0xFFFE;                     //������λ
	temp |= en;                         //�������λ
	
	VEML7700_Write(POWER_SAVING,temp);  //д��Ĵ���
}


//��ȡ ALS �������
//
//��������
//
//����ֵ��u16��0xXXXX����ȡ��������(16λ)
//             0x0000����ȡ����ʧ��(Ҳ�п��ܶ�����ȷ��ֵ��ȫΪ0)
u16 ALS_DATA_Read(void)
{
	return (VEML7700_Read(ALS));
}

/*
//��ȡ WHITE ͨ������
//
//��������
//
//����ֵ��u16��0xXXXX����ȡ��������(16λ)
//             0x0000����ȡ����ʧ��(Ҳ�п��ܶ�����ȷ��ֵ��ȫΪ0)
u16 WHITE_DATA_Read(void)
{
	return (VEML7700_Read(WHITE));
}


//��ȡ INT ����ֵ״̬
//
//��������
//
//����ֵ��u8��λ״̬(0/1)
u8 INT_L_Read(void)
{
	return ((u8)((VEML7700_Read(ALS_INT) & 0x8000) >> 15));
}


//��ȡ INT ����ֵ״̬
//
//��������
//
//����ֵ��u8��λ״̬(0/1)
u8 INT_H_Read(void)
{
	return ((u8)((VEML7700_Read(ALS_INT) & 0x4000) >> 14));
}
*/

//��ʼ��VEML7700
//
//��������
//
//����ֵ����
void VEML7700_Init(void)
{
	ALS_INT_EN(DIS);//�ر� INT ����
	PSM_EN(disable);//�ر�ʡ��ģʽ
}


//���㵱ǰ�ն�
//
//ע�⣺���������չٷ�Ӧ��ָ�ϣ�����AGC��AITC���Լ������Խ���
//
//��������
//
//����ֵ��float����ǰ�ն�(��λ��lx �� Cd/m^2)
float Lux_Calc(void)
{
	char IT      = 0;      //-2��-1��0��1��2��3
	char G       = 1;      //1��2��3��4
	u16 ALS_Data = 0;      //ALSԭʼ����
	float temp   = 0.0;
	
	ALS_SD(ON);            //оƬ�ϵ�
	DelayXms(3);           //�ϵ���ʱ3ms
	
	ALS_IT(ms100);         //����ʱ���ʼ��Ϊ100ms
	ALS_GAIN(ALS_gain_1_8);//ALS�����ʼ��Ϊ1/8

L1:	
	switch(IT)             //��ʱ���ȴ�������Ч
	{
		case 0:
			DelayXms(200);
		  break;
		
		case 1:
			DelayXms(400);
		  break;
		
		case 2:
			DelayXms(800);
		  break;
		
		case 3:
			DelayXms(1600);
		  break;
	}
             
	ALS_Data = ALS_DATA_Read();  //��ALS_Data
	if(ALS_Data <= 100)          //ALS_Data <= 100
	{
		ALS_SD(OFF);               //оƬ����
		
		if(G != 4)
		  G  += 1;                 //�л�ALS���棬��С����
		switch(G)                  //�л�ALS���棬ִ��
		{
			case 2:
				ALS_GAIN(ALS_gain_1_4);//ALS��������Ϊ1/4
			  break;
				
			case 3:
				ALS_GAIN(ALS_gain_1);  //ALS��������Ϊ1
			  break;
				
			case 4:
				ALS_GAIN(ALS_gain_2);  //ALS��������Ϊ2
			  break;
		}
		
		if(G == 4)                 //�����Ѵ���С
		{
			IT += 1;                 //���ӻ���ʱ��
			switch(IT)               //���ӻ���ʱ�䣬ִ��
			{
				case 1:
					ALS_IT(ms200);       //����ʱ������Ϊ200ms
					break;
					
				case 2:
					ALS_IT(ms400);       //����ʱ������Ϊ400ms
					break;
					
				case 3:
					ALS_IT(ms800);       //����ʱ������Ϊ800ms
					break;
					
				case 4:                //�����ѳ�����Χ
					break;
			}
			
			if(IT == 4)              //����ʱ���Ѵ����
			{
				ALS_SD(OFF);                        //оƬ����
				return (ALS_Data * ((float)0.0036));//���ص�ǰ�ն�ֵ
			}
			else                     //����ʱ��δ�����
			{
				ALS_SD(ON);            //оƬ�ϵ�
	      DelayXms(3);           //�ϵ���ʱ3ms
			
			  goto L1;               //�ٴβ���
			}
		}
		else                       //����δ����С
		{
			ALS_SD(ON);              //оƬ�ϵ�
	    DelayXms(3);             //�ϵ���ʱ3ms
			
			goto L1;                 //�ٴβ���
		}
	}
	else                         //ALS_Data >  100
	{
L2:
		if(ALS_Data > 10000)       //ALS_Data > 10000
		{
			ALS_SD(OFF);             //оƬ����
		
			IT -= 1;                 //���̻���ʱ��
			switch(IT)               //���̻���ʱ�䣬ִ��
			{
				case 2:
					ALS_IT(ms400);       //����ʱ������Ϊ400ms
					break;
					
				case 1:
					ALS_IT(ms200);       //����ʱ������Ϊ200ms
					break;
					
				case 0:
					ALS_IT(ms100);       //����ʱ������Ϊ100ms
					break;
					
				case -1:
					ALS_IT(ms50);        //����ʱ������Ϊ50ms
					break;
					
				case -2:
					ALS_IT(ms25);              //����ʱ������Ϊ25ms
					
					ALS_SD(ON);                //оƬ�ϵ�
	        DelayXms(3);               //�ϵ���ʱ3ms
				
				  DelayXms(50);              //�ȴ�������Ч
				  ALS_Data = ALS_DATA_Read();//���¶�ȡALS_Data
					break;
			}
			
			if(IT == -2)                   //����ʱ���Ѵ����
			{
				switch(G)                    //���ݵ�ǰ���������ʱ������ն�
				{
					case 1:
						temp = ALS_Data * ((float)1.8432);
					  break;
					
					case 2:
						temp = ALS_Data * ((float)0.9216);
					  break;
					
					case 3:
						temp = ALS_Data * ((float)0.2304);
					  break;
					
					case 4:
						temp = ALS_Data * ((float)0.1125);
					  break;
				}
				temp = temp*(temp*(temp*(C1*temp-C2)+C3)+C4);
				ALS_SD(OFF);       //оƬ����
				return temp;       //���ص�ǰ�ն�
			}
			else                 //����ʱ��δ�����
			{
				ALS_SD(ON);        //оƬ�ϵ�
	      DelayXms(3);       //�ϵ���ʱ3ms
				
				switch(IT)         //��ʱ���ȴ�������Ч
				{
					case 2:
						DelayXms(800);
					  break;
					
					case 1:
						DelayXms(400);
					  break;
					
					case 0:
						DelayXms(200);
					  break;
					
					case -1:
						DelayXms(100);
					  break;
				}
				
				ALS_Data = ALS_DATA_Read();//���¶�ȡALS_Data
				goto L2;                   //�ٴ��ж�ALS_Data��Χ
			}
		}
		else                           //100 < ALS_Data <= 10000
		{
			switch(IT)                   //���ݵ�ǰ���������ʱ������ն�
			{
				case 3:                    //800ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.0288);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0072);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0036);
							break;
					}
					break;
				
				case 2:                    //400ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0144);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0072);
							break;
					}
					break;
				
				case 1:                    //200ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.2304);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0288);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0144);
							break;
					}
					break;
				
				case 0:                    //100ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.4608);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.2304);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0288);
							break;
					}
					break;
				
				case -1:                   //50ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.9216);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.4608);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0576);
							break;
					}
					break;
			}
			temp = temp*(temp*(temp*(C1*temp-C2)+C3)+C4);
			ALS_SD(OFF);                 //оƬ����
			return temp;                 //���ص�ǰ�ն�
		}
	}
}

