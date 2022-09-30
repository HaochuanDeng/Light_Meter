#include "OLED.h"


//дһ���ֽ����ݵ�GDDRAM
//
//ע�⣺���ô˺���ǰ��Ҫ��ʼ��IIC(���� IIC_Init() ����һ�μ���)
//      д���GDDRAMָ����Զ���1
//
//������u8 date����д�������
//
//����ֵ����
static void OLED_WriteD(u8 date)
{
	IIC_Start();               //������ʼ�ź�
	IIC_WriteByte(OLED_ADDR_W);//д�ӻ���ַ0111 1000(д)
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_WriteByte(0x40);       //д����0100 0000(����)
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_WriteByte(date);       //д������
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_Stop();                //����ֹͣ�ź�
}


//дһ���ֽ�����
//
//ע�⣺���ô˺���ǰ��Ҫ��ʼ��IIC(���� IIC_Init() ����һ�μ���)
//      ��һ��������в������ٴε��ô˺���������ز�������
//
//������u8 cmd����д�������
//
//����ֵ����
static void OLED_WriteC(u8 cmd)
{
	IIC_Start();               //������ʼ�ź�
	IIC_WriteByte(OLED_ADDR_W);//д�ӻ���ַ0111 1000(д)
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_WriteByte(0x00);       //д����0000 0000(����)
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_WriteByte(cmd);        //д������
	IIC_WaitResp();            //�ȴ��ӻ���Ӧ
	IIC_Stop();                //����ֹͣ�ź�
}


//���öԱȶ�
//
//������u8 leve���Աȶ�(0~255)
//
//����ֵ����
void Contrast_Set(u8 leve)
{
	OLED_WriteC(0x81);//д������
	OLED_WriteC(leve);//д��Աȶ�
}


//OLEDǿ����ʾ����
//
//������u8 arg��0���ָ�GDDRAM������ʾ(RESET)���������GDDRAM����
//              1��ȫ�����ص�򿪣��������RAM����
//
//����ֵ����
void Entire_Disp_ON_OFF(u8 arg)
{
	if(arg == 0)     //����GDDRAM������ʾOLED
		OLED_WriteC(0xA4);
	else if(arg == 1)//ǿ�Ƶ���ȫ��OLED������GDDRAM����
		OLED_WriteC(0xA5);
}


//������ʾģʽ
//
//������u8 arg��0��������ʾ(RESET)(0��1��)
//              1����ת��ʾ(0��1��)
//
//����ֵ����
void Disp_NOR_INV(u8 arg)
{
	if(arg == 0)     //������ʾ
		OLED_WriteC(0xA6);
	else if(arg == 1)//��ת��ʾ
		OLED_WriteC(0xA7);
}


//OLED��ʾ����
//
//������u8 arg��0����ʾ�ر�(˯��ģʽ)(RESET)
//              1����ʾ����(����ģʽ)
//
//����ֵ����
void Disp_ON_OFF(u8 arg)
{
	if(arg == 0)     //OLED��ʾ�ر�
		OLED_WriteC(0xAE);
	else if(arg == 1)//OLED��ʾ����
		OLED_WriteC(0xAF);
}

/*
//�� page ������/���ƶ� 1 ��
//
//ע�⣺�����ٵ��� Go_Scroll() �� Stop_Scroll() �����������������ִ�У�
//      ��������2������ �ɶ� ʹ��
//
//������u8 lr��0������
//             1������
//      u8 sp����ʼ page ��ַ(0~7)
//      u8 bf��ÿ���ƶ���ʱ��֡(�ƶ��ٶ�)
//             0��5  ֡
//             1��64 ֡
//             2��128֡
//             3��256֡
//             4��3  ֡
//             5��4  ֡
//             6��25 ֡
//             7��2  ֡
//       u8 ep������ page ��ַ(0~7��������ڻ������ʼ page ��ַ)
//
//����ֵ����
void R_L_Horiz_Scroll(u8 lr, u8 sp, u8 bf, u8 ep)
{
	if(lr == 0)     //����
		OLED_WriteC(0x27);
	else if(lr == 1)//����
		OLED_WriteC(0x26);
	else
		return;       //��������
	
	OLED_WriteC(0x00);
	OLED_WriteC(sp);
	OLED_WriteC(bf);
	OLED_WriteC(ep);
	OLED_WriteC(0x00);
	OLED_WriteC(0xFF);
}


//�� page ������/���ƶ� 1 �У�����/���ƶ� vofset ��
//
//ע�⣺��Ҫ�ٵ��� Go_Scroll() �� Stop_Scroll() �����������������ִ�У�
//      ��������2������ �ɶ� ʹ��
//
//������u8 lr��0������
//             1������
//      u8 sp����ʼ page ��ַ(0~7)
//      u8 bf��ÿ���ƶ���ʱ��֡
//             0��5  ֡
//             1��64 ֡
//             2��128֡
//             3��256֡
//             4��3  ֡
//             5��4  ֡
//             6��25 ֡
//             7��2  ֡
//      u8 ep:���� page ��ַ(0~7��������ڻ������ʼ page ��ַ)
//      u8 vofset����ֱ�ƶ�����(0~63)(0��ֻ�����ƶ�����ֱ���ƶ�)
//
//����ֵ����
void R_L_Horiz_Verti_Scroll(u8 lr, u8 sp, u8 bf, u8 ep, u8 vofset)
{
	if(lr == 0)     //����
		OLED_WriteC(0x2A);
	else if(lr == 1)//����
		OLED_WriteC(0x29);
	else
		return;       //��������
	
	OLED_WriteC(0x00);
	OLED_WriteC(sp);
	OLED_WriteC(bf);
	OLED_WriteC(ep);
	OLED_WriteC(vofset);
}


//ֹͣ�ƶ�
//
//ע�⣺���ú���Ҫ����д��GDDRAM����
//
//��������
//
//����ֵ����
void Stop_Scroll(void)
{
	OLED_WriteC(0x2E);
}


//�����ƶ�
//
//ע�⣺����ǰ��Ҫ�����ƶ�����������ֻ�����һ��������Ч
//      ���ú��ֹ��дGDDRAM����ֹ�ı��ƶ�������ֱ��ֹͣ�ƶ�
//
//��������
//
//����ֵ����
void Go_Scroll(void)
{
	OLED_WriteC(0x2F);
}


//������/���ƶ�����(����)
//
//ע�⣺һ����˵����/���ƶ����� С�ڵ��� MUX ֵ����
//
//������u8 rf��Set No. of rows in top fixed area. The No. of
//             rows in top fixed area is referenced to the
//             top of the GDDRAM (i.e. row 0).[RESET =0]
//             (0~63)
//      u8 ra��Set No. of rows in scroll area. This is the
//             number of rows to be used for vertical
//             scrolling. The scroll area starts in the first
//             row below the top fixed area. [RESET = 64]
//             (0~127)
//
//����ֵ����
void Verti_Scroll_Area_Set(u8 rf, u8 ra)
{
	OLED_WriteC(0xA3);
	OLED_WriteC(rf);
	OLED_WriteC(ra);
}


//����ҳѰַģʽ�ĵ�����ʼ��ַ
//
//ע�⣺ֻ��ҳѰַģʽ��Ч
//
//������u8 addr��Set the lower nibble of the column start address
//               register for Page Addressing Mode using X[3:0]
//               as data bits. The initial display line register is
//               reset to 0000b after RESET
//               (0~15)
//
//����ֵ����
void Lower_Col_Addr_Set(u8 addr)
{
	OLED_WriteC(addr);
}


//����ҳѰַģʽ�ĸ�����ʼ��ַ
//
//ע�⣺ֻ��ҳѰַģʽ��Ч
//
//������u8 addr��Set the higher nibble of the column start address
//               register for Page Addressing Mode using X[3:0]
//               as data bits. The initial display line register is
//               reset to 0000b after RESET
//               (0~15)
//
//����ֵ����
void Higher_Col_Addr_Set(u8 addr)
{
	OLED_WriteC(addr | 0x10);
}
*/

//����Ѱַģʽ
//
//������u8 arg��0��ˮƽѰַģʽ
//              1����ֱѰַģʽ
//              2��ҳѰַģʽ
//
//����ֵ����
void Addr_Mode_Set(u8 arg)
{
	OLED_WriteC(0x20);
	OLED_WriteC(arg);
}


//��������ʼ�������ַ
//
//ע�⣺ֻ��ˮƽ/��ֱѰַģʽ��Ч
//
//������u8 sa������ʼ��ַ(0~127)
//      u8 ea���н�����ַ(0~127)
//
//����ֵ����
void Col_Addr_Set(u8 sa, u8 ea)
{
	OLED_WriteC(0x21);
	OLED_WriteC(sa);
	OLED_WriteC(ea);
}


//����ҳ��ʼ�������ַ
//
//ע�⣺ֻ��ˮƽ/��ֱѰַģʽ��Ч
//
//������u8 sa��ҳ��ʼ��ַ(0~7)
//      u8 ea��ҳ������ַ(0~7)
//
//����ֵ����
void Page_Addr_Set(u8 sa, u8 ea)
{
	OLED_WriteC(0x22);
	OLED_WriteC(sa);
	OLED_WriteC(ea);
}

/*
//���� GDDRAM ҳ��ʼ��ַ
//
//ע�⣺ֻ��ҳѰַģʽ��Ч
//
//������u8 addr��GDDRAMҳ��ʼ��ַ(0~7)
//
//����ֵ����
void Page_Start_Addr_Set(u8 addr)
{
	OLED_WriteC(addr | 0xB0);
}
*/

//���� GDDRAM ��ʾ��ʼ����
//
//������u8 line��GDDRAM��ʾ��ʼ����(0~63)
//
//����ֵ����
void Disp_Start_Line_Set(u8 line)
{
	OLED_WriteC(line | 0x40);
}


//Seg��ӳ��
//
//������u8 arg��0��������ӳ��
//              1��������ӳ��
//
//����ֵ����
void Seg_Re_Map(u8 arg)
{
	if(arg == 0)     //������ӳ��
		OLED_WriteC(0xA0);
	else if(arg == 1)//������ӳ��
		OLED_WriteC(0xA1);
}


//���� MUX ����
//
//������u8 mux��MUX����(15~63)����Ӧ N �ķ�Χ(16~64)
//
//����ֵ����
void Mux_Set(u8 mux)
{
	OLED_WriteC(0xA8);
	OLED_WriteC(mux);
}


//COM���ɨ�跽������(COM��ӳ��)
//
//������u8 arg��0������ɨ�跽��
//              1����ӳ��ɨ�跽��
//
//����ֵ����
void COM_Scan_Dir_Set(u8 arg)
{
	if(arg == 0)     //����ɨ�跽��
		OLED_WriteC(0xC0);
	else if(arg == 1)//��ӳ��ɨ�跽��
		OLED_WriteC(0xC8);
}


//������ʾ����ֱƫ����
//
//������u8 ofset����ʾ����ֱƫ����(0~63)
//
//����ֵ����
void Disp_Offset_Set(u8 ofset)
{
	OLED_WriteC(0xD3);
	OLED_WriteC(ofset);
}


//���� COM ����Ӳ������
//
//������u8 conf��0��Sequential COM pin configuration
//                  (RESET), Disable COM Left/Right remap
//               1��(RESET), Alternative COM pin configuration
//                  (RESET), Disable COM Left/Right remap
//               2��Enable COM Left/Right remap
//                  Sequential COM pin configuration
//               3��(RESET), Alternative COM pin configuration
//                   Enable COM Left/Right remap
//
//����ֵ����
void COM_Pin_HW_Config(u8 conf)
{
	OLED_WriteC(0xDA);
	OLED_WriteC((conf << 4) | 0x02);
}


//���� DCLK ��Ƶϵ��D���Լ� Fosc Ƶ��
//
//������u8 dclk��DCLK��Ƶϵ��(0~15)����Ӧ D �ķ�Χ(1~16)
//      u8 fosc���ڲ�����Ƶ�ʣ���ֵԽ��Ƶ��Խ��(0~15)(8��333~407kHz)
//
//����ֵ����
void DCLK_Fosc_Set(u8 dclk, u8 fosc)
{
	OLED_WriteC(0xD5);
	OLED_WriteC((fosc << 4) | dclk);
}


//����Ԥ�������ڳ���
//
//������u8 p1���׶� 1 ����(1~15)
//      u8 p2���׶� 2 ����(1~15)
//
//����ֵ����
void Pre_Charge_Period_Set(u8 p1, u8 p2)
{
	OLED_WriteC(0xD9);
  OLED_WriteC((p2 << 4) | p1);
}


//���� Vcomh deselect ����
//
//������u8 leve��Vcomh deselect ����
//               0��~0.65 * Vcc
//               2��~0.77 * Vcc(RESET)
//               3��~0.83 * Vcc
//
//����ֵ����
void Vcomh_Deselect_Level_Set(u8 leve)
{
	OLED_WriteC(0xDB);
	OLED_WriteC(leve << 4);
}

/*
//�޲���ָ��
//
//��������
//
//����ֵ����
void NOP(void)
{
	OLED_WriteC(0xE3);
}
*/

//��/�رյ�ɱ�
//
//ע�⣺�򿪵�ɱ�ǰҪ�ر�OLED��ʾ���򿪵�ɱú��ٴ�OLED��ʾ����
//
//������u8 arg��0���رյ�ɱ�
//              1���򿪵�ɱ�
//
//����ֵ����
void Charge_Pump_Set(u8 arg)
{
  OLED_WriteC(0x8D);
	
	if(arg == 0)     //�رյ�ɱ�
		OLED_WriteC(0x10);
	else if(arg == 1)//�򿪵�ɱ�
		OLED_WriteC(0x14);
}


//���OLED��ʾ��
//
//��������
//
//����ֵ����
static void OLED_Clear(void)
{
	u8 x,y;
	
	Addr_Mode_Set(0);   //ˮƽѰַģʽ
	Col_Addr_Set(0,127);//��������ʼ/������ַ
	Page_Addr_Set(0,7); //����ҳ��ʼ/������ַ
	
	//д��GDDRAM��ȫ����0
	for(x=0;x<8;x++)
	  for(y=0;y<128;y++)
			OLED_WriteD(0x00);
}


//��ʼ��OLED(OLED�ϵ�)
//
//ע�⣺���ô˺���ǰ��Ҫ��ʼ��IIC(���� IIC_Init() ����һ�μ���)
//
//��������
//
//����ֵ����
void OLED_Init(void)
{
	Disp_ON_OFF(0);             //�ر�OLED��ʾ
	
	DCLK_Fosc_Set(0,15);        //����Ƶ����15��OSCƵ��
  Mux_Set(63);                //MUX=63������64��(������)
	Disp_Offset_Set(0);         //��ʾ����ֱƫ��0��
	Disp_Start_Line_Set(0);     //GDDRAM�ӵ�0�п�ʼ��ʾ
	Charge_Pump_Set(1);         //�򿪵�ɱ�
	Seg_Re_Map(1);              //����Seg��ӳ��
	COM_Scan_Dir_Set(1);        //����COMɨ�跽����ӳ��
	COM_Pin_HW_Config(0);       //����COM���ã��������ҷ���
	Contrast_Set(255);          //���öԱȶ�255(���)
	Pre_Charge_Period_Set(2,15);//����OLED����ʾʱ��
	Vcomh_Deselect_Level_Set(3);//����Vcomh����
	Entire_Disp_ON_OFF(0);      //��ʾGDDRAM������
	Disp_NOR_INV(0);            //������ʾ(1��0��)
	
	OLED_Clear();               //����
	DelayXms(50);               //�ȴ���ɱò����� Vcc=7.5V ��ѹ�ȶ�
	Disp_ON_OFF(1);             //��OLED��ʾ
	DelayXms(100);              //100ms�Ƽ���ʱ
}

/*
//OLED����
//
//��������
//
//����ֵ����
void OLED_Power_Down(void)
{
	Disp_ON_OFF(0);    //�ر�OLED��ʾ
	Charge_Pump_Set(0);//�رյ�ɱ�
	
	DelayXms(100);     //�ȴ���ɱò����� Vcc=7.5V ����
}


//OLED����˯��ģʽ
//
//��������
//
//����ֵ����
void OLED_Sleep(void)
{
	Disp_ON_OFF(0);    //�ر�OLED��ʾ
	Charge_Pump_Set(0);//�رյ�ɱ�
}


//OLED����
//
//��������
//
//����ֵ����
void OLED_Wake(void)
{
	Charge_Pump_Set(1);//�򿪵�ɱ�
	Disp_ON_OFF(1);    //��OLED��ʾ
	
	DelayXms(100);     //�ȴ���ɱò����� Vcc=7.5V ��ѹ�ȶ�
}
*/

//OLEDͨ����ʾ����
//
//ע�⣺32*16�ֿ�([15][64])����EEPROM�ж�ȡ(960bytes,sect=6��7)��(0~9������)(10��С����)(11����ĸ L)(12����ĸ S)(13��б��)(14������ʾ)
//      ����ǰ�����ʼ��EEPROM
//
//������u8 slash��б��λ(0������ʾ��1����ʾ)
//      u8 s0����0��ʾλ(0~12)(14������ʾ)
//      u8 s1����1��ʾλ(0~12)(14������ʾ)
//      u8 s2����2��ʾλ(0~12)(14������ʾ)
//      u8 s3����3��ʾλ(0~12)(14������ʾ)
//
//����ֵ����
void OLED_Disp(u8 slash, u8 s0, u8 s1, u8 s2, u8 s3)
{
	u8 x,y,s,num,i = 0;
	u8 xdata temp[64]; //��Ŵ�EEPROM�ж�����1���ֿ�(32*16)����
	
	OLED_Clear();      //����
	Addr_Mode_Set(1);  //��ֱ��ʾģʽ
	Page_Addr_Set(1,7);//����ҳ��ַ��Χ(1~7)
	
	//�����е�ַ��Χ
	if(slash == 1)     //б��
	{
		Col_Addr_Set(28,31);
		s   = 4;
		EEPROM_Read(temp,7,320,64);
		
		goto L2;
	}

L1:
	s = 16;
	switch(i)
	{
		case 0:
		  Col_Addr_Set(33,48);//��0λ����
		  num = s0;	
		break;
		
		case 1:
			Col_Addr_Set(50,65);//��1λ����
		  num = s1;
		break;
		
		case 2:
			Col_Addr_Set(67,82);//��2λ����
		  num = s2;
		break;
		
		case 3:
			Col_Addr_Set(84,99);//��3λ����
		  num = s3;
		break;
		
		default:
			return;
	}
	if(num  < 8)//0~7
		EEPROM_Read(temp,6,num*64,64);
	else        //8~14
		EEPROM_Read(temp,7,(num-8)*64,64);
	
	goto L2;

L2:
	//��ʼ��ʾ
	for(x=0;x<s;x++)
	{
		for(y=1;y<8;y++)
		{
			switch(y)
			{
				case 1:
					OLED_WriteD(temp[x*4]);
					break;
				
				case 2:
					OLED_WriteD(temp[x*4+1]);
					break;
				
				case 3:
					OLED_WriteD(temp[x*4+2]);
					break;
				
				case 4:
					OLED_WriteD(temp[x*4+3]);
					break;
				
				case 5:
					OLED_WriteD(temp[x*4]);
					break;
				
				case 6:
					OLED_WriteD(temp[x*4+1]);
					break;
				
				case 7:
					OLED_WriteD(temp[x*4+2]);
					break;
			}
		}
	}
	if(s != 4)
	  i++;
	
	goto L1;
}


//��float�� С�� ����ת��Ϊ����OLED_Disp()���õ��ĺ�4λ����
//
//ע�⣺����OLED_Disp()��4λ������(0~9������)(10��С����)
//
//������float num����ת����float�ͱ���
//      u8* seg��  ���ɵĺ���OLED_Disp()��4λ������ָ��
//
//����ֵ����
void FLOAT_Conv(float num, u8* seg)
{
	u16 temp;
	
	if(num < 10)              //С�����ڵ�1λ
	{
		temp   = (u16)(num*100);//����100�������ڷ����λ��
		seg[0] = temp/100;      //�����λ��
		seg[1] = 10;
		seg[2] = temp/10%10;
		seg[3] = temp%10;
	}
	else                      //С�����ڵ�2λ
	{
		temp   = (u16)(num*10); //����10�������ڷ����λ��
		seg[0] = temp/100;      //�����λ��
		seg[1] = temp/10%10;
		seg[2] = 10;
		seg[3] = temp%10;
	}
}


//��float�� ���� ����ת��Ϊ����OLED_Disp()���õ��ĺ�4λ����
//
//ע�⣺����OLED_Disp()��4λ������(0~9������)(14������ʾ)
//
//������float num����ת����float�ͱ���
//      u8* seg��  ���ɵĺ���OLED_Disp()��4λ������ָ��
//
//����ֵ����
void INT_Conv(float num, u8* seg)
{
	u8  i;
	u16 temp = (u16)num;
	
	//�жϱ�����Чλ��
	if((temp < 10000) && (temp >999))    //4λ��
		i = 0;
	else if((temp < 1000) && (temp > 99))//3λ��
		i = 1;
	else if((temp < 100)  && (temp > 9)) //2λ��
		i = 2;
	else                                 //1λ��
		i = 3;
	
	//ת��Ϊ����OLED_Disp()����Ĳ���
	seg[0] = temp/1000;
	seg[1] = temp/100%10;
	seg[2] = temp/10%10;
	seg[3] = temp%10;
	
	//ȥ����λ��0����֤������ʾ
	for(i=i;i>0;i--)
		seg[i-1] = 14;
}

