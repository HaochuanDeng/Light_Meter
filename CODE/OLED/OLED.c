#include "OLED.h"


//写一个字节数据到GDDRAM
//
//注意：调用此函数前需要初始化IIC(调用 IIC_Init() 函数一次即可)
//      写入后，GDDRAM指针会自动加1
//
//参数：u8 date：待写入的数据
//
//返回值：无
static void OLED_WriteD(u8 date)
{
	IIC_Start();               //产生起始信号
	IIC_WriteByte(OLED_ADDR_W);//写从机地址0111 1000(写)
	IIC_WaitResp();            //等待从机响应
	IIC_WriteByte(0x40);       //写命令0100 0000(数据)
	IIC_WaitResp();            //等待从机响应
	IIC_WriteByte(date);       //写入数据
	IIC_WaitResp();            //等待从机响应
	IIC_Stop();                //产生停止信号
}


//写一个字节命令
//
//注意：调用此函数前需要初始化IIC(调用 IIC_Init() 函数一次即可)
//      若一个命令后含有参数，再次调用此函数发送相关参数即可
//
//参数：u8 cmd：待写入的命令
//
//返回值：无
static void OLED_WriteC(u8 cmd)
{
	IIC_Start();               //产生起始信号
	IIC_WriteByte(OLED_ADDR_W);//写从机地址0111 1000(写)
	IIC_WaitResp();            //等待从机响应
	IIC_WriteByte(0x00);       //写命令0000 0000(命令)
	IIC_WaitResp();            //等待从机响应
	IIC_WriteByte(cmd);        //写入命令
	IIC_WaitResp();            //等待从机响应
	IIC_Stop();                //产生停止信号
}


//设置对比度
//
//参数：u8 leve：对比度(0~255)
//
//返回值：无
void Contrast_Set(u8 leve)
{
	OLED_WriteC(0x81);//写入命令
	OLED_WriteC(leve);//写入对比度
}


//OLED强制显示设置
//
//参数：u8 arg：0：恢复GDDRAM内容显示(RESET)，输出依据GDDRAM内容
//              1：全部像素点打开，输出无视RAM内容
//
//返回值：无
void Entire_Disp_ON_OFF(u8 arg)
{
	if(arg == 0)     //依据GDDRAM内容显示OLED
		OLED_WriteC(0xA4);
	else if(arg == 1)//强制点亮全部OLED，无视GDDRAM内容
		OLED_WriteC(0xA5);
}


//设置显示模式
//
//参数：u8 arg：0：正常显示(RESET)(0灭1亮)
//              1：反转显示(0亮1灭)
//
//返回值：无
void Disp_NOR_INV(u8 arg)
{
	if(arg == 0)     //正常显示
		OLED_WriteC(0xA6);
	else if(arg == 1)//反转显示
		OLED_WriteC(0xA7);
}


//OLED显示开关
//
//参数：u8 arg：0：显示关闭(睡眠模式)(RESET)
//              1：显示开启(正常模式)
//
//返回值：无
void Disp_ON_OFF(u8 arg)
{
	if(arg == 0)     //OLED显示关闭
		OLED_WriteC(0xAE);
	else if(arg == 1)//OLED显示开启
		OLED_WriteC(0xAF);
}

/*
//对 page 进行左/右移动 1 列
//
//注意：必须再调用 Go_Scroll() 或 Stop_Scroll() ，激活或放弃此命令的执行，
//      与上述两2个函数 成对 使用
//
//参数：u8 lr：0：左移
//             1：右移
//      u8 sp：起始 page 地址(0~7)
//      u8 bf：每次移动耗时几帧(移动速度)
//             0：5  帧
//             1：64 帧
//             2：128帧
//             3：256帧
//             4：3  帧
//             5：4  帧
//             6：25 帧
//             7：2  帧
//       u8 ep：结束 page 地址(0~7，必须大于或等于起始 page 地址)
//
//返回值：无
void R_L_Horiz_Scroll(u8 lr, u8 sp, u8 bf, u8 ep)
{
	if(lr == 0)     //左移
		OLED_WriteC(0x27);
	else if(lr == 1)//右移
		OLED_WriteC(0x26);
	else
		return;       //参数错误
	
	OLED_WriteC(0x00);
	OLED_WriteC(sp);
	OLED_WriteC(bf);
	OLED_WriteC(ep);
	OLED_WriteC(0x00);
	OLED_WriteC(0xFF);
}


//对 page 进行左/右移动 1 列，并上/下移动 vofset 行
//
//注意：需要再调用 Go_Scroll() 或 Stop_Scroll() ，激活或放弃此命令的执行，
//      与上述两2个函数 成对 使用
//
//参数：u8 lr：0：左移
//             1：右移
//      u8 sp：起始 page 地址(0~7)
//      u8 bf：每次移动耗时几帧
//             0：5  帧
//             1：64 帧
//             2：128帧
//             3：256帧
//             4：3  帧
//             5：4  帧
//             6：25 帧
//             7：2  帧
//      u8 ep:结束 page 地址(0~7，必须大于或等于起始 page 地址)
//      u8 vofset：垂直移动行数(0~63)(0：只左右移动，垂直不移动)
//
//返回值：无
void R_L_Horiz_Verti_Scroll(u8 lr, u8 sp, u8 bf, u8 ep, u8 vofset)
{
	if(lr == 0)     //左移
		OLED_WriteC(0x2A);
	else if(lr == 1)//右移
		OLED_WriteC(0x29);
	else
		return;       //参数错误
	
	OLED_WriteC(0x00);
	OLED_WriteC(sp);
	OLED_WriteC(bf);
	OLED_WriteC(ep);
	OLED_WriteC(vofset);
}


//停止移动
//
//注意：调用后需要重新写入GDDRAM数据
//
//参数：无
//
//返回值：无
void Stop_Scroll(void)
{
	OLED_WriteC(0x2E);
}


//启动移动
//
//注意：调用前需要配置移动参数，并且只有最后一个命令有效
//      调用后禁止读写GDDRAM，禁止改变移动参数，直到停止移动
//
//参数：无
//
//返回值：无
void Go_Scroll(void)
{
	OLED_WriteC(0x2F);
}


//设置上/下移动区域(行数)
//
//注意：一般来说，上/下移动行数 小于等于 MUX 值即可
//
//参数：u8 rf：Set No. of rows in top fixed area. The No. of
//             rows in top fixed area is referenced to the
//             top of the GDDRAM (i.e. row 0).[RESET =0]
//             (0~63)
//      u8 ra：Set No. of rows in scroll area. This is the
//             number of rows to be used for vertical
//             scrolling. The scroll area starts in the first
//             row below the top fixed area. [RESET = 64]
//             (0~127)
//
//返回值：无
void Verti_Scroll_Area_Set(u8 rf, u8 ra)
{
	OLED_WriteC(0xA3);
	OLED_WriteC(rf);
	OLED_WriteC(ra);
}


//设置页寻址模式的低列起始地址
//
//注意：只对页寻址模式有效
//
//参数：u8 addr：Set the lower nibble of the column start address
//               register for Page Addressing Mode using X[3:0]
//               as data bits. The initial display line register is
//               reset to 0000b after RESET
//               (0~15)
//
//返回值：无
void Lower_Col_Addr_Set(u8 addr)
{
	OLED_WriteC(addr);
}


//设置页寻址模式的高列起始地址
//
//注意：只对页寻址模式有效
//
//参数：u8 addr：Set the higher nibble of the column start address
//               register for Page Addressing Mode using X[3:0]
//               as data bits. The initial display line register is
//               reset to 0000b after RESET
//               (0~15)
//
//返回值：无
void Higher_Col_Addr_Set(u8 addr)
{
	OLED_WriteC(addr | 0x10);
}
*/

//设置寻址模式
//
//参数：u8 arg：0：水平寻址模式
//              1：垂直寻址模式
//              2：页寻址模式
//
//返回值：无
void Addr_Mode_Set(u8 arg)
{
	OLED_WriteC(0x20);
	OLED_WriteC(arg);
}


//设置列起始与结束地址
//
//注意：只对水平/垂直寻址模式有效
//
//参数：u8 sa：列起始地址(0~127)
//      u8 ea：列结束地址(0~127)
//
//返回值：无
void Col_Addr_Set(u8 sa, u8 ea)
{
	OLED_WriteC(0x21);
	OLED_WriteC(sa);
	OLED_WriteC(ea);
}


//设置页起始与结束地址
//
//注意：只对水平/垂直寻址模式有效
//
//参数：u8 sa：页起始地址(0~7)
//      u8 ea：页结束地址(0~7)
//
//返回值：无
void Page_Addr_Set(u8 sa, u8 ea)
{
	OLED_WriteC(0x22);
	OLED_WriteC(sa);
	OLED_WriteC(ea);
}

/*
//设置 GDDRAM 页起始地址
//
//注意：只对页寻址模式有效
//
//参数：u8 addr：GDDRAM页起始地址(0~7)
//
//返回值：无
void Page_Start_Addr_Set(u8 addr)
{
	OLED_WriteC(addr | 0xB0);
}
*/

//设置 GDDRAM 显示起始行数
//
//参数：u8 line：GDDRAM显示起始行数(0~63)
//
//返回值：无
void Disp_Start_Line_Set(u8 line)
{
	OLED_WriteC(line | 0x40);
}


//Seg重映射
//
//参数：u8 arg：0：禁用重映射
//              1：启用重映射
//
//返回值：无
void Seg_Re_Map(u8 arg)
{
	if(arg == 0)     //禁用重映射
		OLED_WriteC(0xA0);
	else if(arg == 1)//启用重映射
		OLED_WriteC(0xA1);
}


//设置 MUX 比率
//
//参数：u8 mux：MUX比率(15~63)，对应 N 的范围(16~64)
//
//返回值：无
void Mux_Set(u8 mux)
{
	OLED_WriteC(0xA8);
	OLED_WriteC(mux);
}


//COM输出扫描方向设置(COM重映射)
//
//参数：u8 arg：0：正常扫描方向
//              1：重映射扫描方向
//
//返回值：无
void COM_Scan_Dir_Set(u8 arg)
{
	if(arg == 0)     //正常扫描方向
		OLED_WriteC(0xC0);
	else if(arg == 1)//重映射扫描方向
		OLED_WriteC(0xC8);
}


//设置显示区垂直偏移量
//
//参数：u8 ofset：显示区垂直偏移量(0~63)
//
//返回值：无
void Disp_Offset_Set(u8 ofset)
{
	OLED_WriteC(0xD3);
	OLED_WriteC(ofset);
}


//设置 COM 引脚硬件配置
//
//参数：u8 conf：0：Sequential COM pin configuration
//                  (RESET), Disable COM Left/Right remap
//               1：(RESET), Alternative COM pin configuration
//                  (RESET), Disable COM Left/Right remap
//               2：Enable COM Left/Right remap
//                  Sequential COM pin configuration
//               3：(RESET), Alternative COM pin configuration
//                   Enable COM Left/Right remap
//
//返回值：无
void COM_Pin_HW_Config(u8 conf)
{
	OLED_WriteC(0xDA);
	OLED_WriteC((conf << 4) | 0x02);
}


//设置 DCLK 分频系数D，以及 Fosc 频率
//
//参数：u8 dclk：DCLK分频系数(0~15)，对应 D 的范围(1~16)
//      u8 fosc：内部振荡器频率，数值越大，频率越大，(0~15)(8：333~407kHz)
//
//返回值：无
void DCLK_Fosc_Set(u8 dclk, u8 fosc)
{
	OLED_WriteC(0xD5);
	OLED_WriteC((fosc << 4) | dclk);
}


//设置预充电各周期长度
//
//参数：u8 p1：阶段 1 长度(1~15)
//      u8 p2：阶段 2 长度(1~15)
//
//返回值：无
void Pre_Charge_Period_Set(u8 p1, u8 p2)
{
	OLED_WriteC(0xD9);
  OLED_WriteC((p2 << 4) | p1);
}


//设置 Vcomh deselect 级别
//
//参数：u8 leve：Vcomh deselect 级别
//               0：~0.65 * Vcc
//               2：~0.77 * Vcc(RESET)
//               3：~0.83 * Vcc
//
//返回值：无
void Vcomh_Deselect_Level_Set(u8 leve)
{
	OLED_WriteC(0xDB);
	OLED_WriteC(leve << 4);
}

/*
//无操作指令
//
//参数：无
//
//返回值：无
void NOP(void)
{
	OLED_WriteC(0xE3);
}
*/

//打开/关闭电荷泵
//
//注意：打开电荷泵前要关闭OLED显示，打开电荷泵后再打开OLED显示即可
//
//参数：u8 arg：0：关闭电荷泵
//              1：打开电荷泵
//
//返回值：无
void Charge_Pump_Set(u8 arg)
{
  OLED_WriteC(0x8D);
	
	if(arg == 0)     //关闭电荷泵
		OLED_WriteC(0x10);
	else if(arg == 1)//打开电荷泵
		OLED_WriteC(0x14);
}


//清除OLED显示区
//
//参数：无
//
//返回值：无
static void OLED_Clear(void)
{
	u8 x,y;
	
	Addr_Mode_Set(0);   //水平寻址模式
	Col_Addr_Set(0,127);//设置列起始/结束地址
	Page_Addr_Set(0,7); //设置页起始/结束地址
	
	//写入GDDRAM，全部清0
	for(x=0;x<8;x++)
	  for(y=0;y<128;y++)
			OLED_WriteD(0x00);
}


//初始化OLED(OLED上电)
//
//注意：调用此函数前需要初始化IIC(调用 IIC_Init() 函数一次即可)
//
//参数：无
//
//返回值：无
void OLED_Init(void)
{
	Disp_ON_OFF(0);             //关闭OLED显示
	
	DCLK_Fosc_Set(0,15);        //不分频，第15级OSC频率
  Mux_Set(63);                //MUX=63，驱动64行(所有行)
	Disp_Offset_Set(0);         //显示区域垂直偏移0行
	Disp_Start_Line_Set(0);     //GDDRAM从第0行开始显示
	Charge_Pump_Set(1);         //打开电荷泵
	Seg_Re_Map(1);              //启用Seg重映射
	COM_Scan_Dir_Set(1);        //启用COM扫描方向重映射
	COM_Pin_HW_Config(0);       //序列COM配置，禁用左右反置
	Contrast_Set(255);          //设置对比度255(最大)
	Pre_Charge_Period_Set(2,15);//设置OLED行显示时序
	Vcomh_Deselect_Level_Set(3);//设置Vcomh级别
	Entire_Disp_ON_OFF(0);      //显示GDDRAM中内容
	Disp_NOR_INV(0);            //正常显示(1亮0灭)
	
	OLED_Clear();               //清屏
	DelayXms(50);               //等待电荷泵产生的 Vcc=7.5V 电压稳定
	Disp_ON_OFF(1);             //打开OLED显示
	DelayXms(100);              //100ms推荐延时
}

/*
//OLED掉电
//
//参数：无
//
//返回值：无
void OLED_Power_Down(void)
{
	Disp_ON_OFF(0);    //关闭OLED显示
	Charge_Pump_Set(0);//关闭电荷泵
	
	DelayXms(100);     //等待电荷泵产生的 Vcc=7.5V 掉电
}


//OLED进入睡眠模式
//
//参数：无
//
//返回值：无
void OLED_Sleep(void)
{
	Disp_ON_OFF(0);    //关闭OLED显示
	Charge_Pump_Set(0);//关闭电荷泵
}


//OLED唤醒
//
//参数：无
//
//返回值：无
void OLED_Wake(void)
{
	Charge_Pump_Set(1);//打开电荷泵
	Disp_ON_OFF(1);    //打开OLED显示
	
	DelayXms(100);     //等待电荷泵产生的 Vcc=7.5V 电压稳定
}
*/

//OLED通用显示函数
//
//注意：32*16字库([15][64])，从EEPROM中读取(960bytes,sect=6、7)，(0~9：数字)(10：小数点)(11：字母 L)(12：字母 S)(13：斜杠)(14：不显示)
//      调用前必须初始化EEPROM
//
//参数：u8 slash：斜杠位(0：不显示；1：显示)
//      u8 s0：第0显示位(0~12)(14：不显示)
//      u8 s1：第1显示位(0~12)(14：不显示)
//      u8 s2：第2显示位(0~12)(14：不显示)
//      u8 s3：第3显示位(0~12)(14：不显示)
//
//返回值：无
void OLED_Disp(u8 slash, u8 s0, u8 s1, u8 s2, u8 s3)
{
	u8 x,y,s,num,i = 0;
	u8 xdata temp[64]; //存放从EEPROM中读到的1个字库(32*16)数据
	
	OLED_Clear();      //清屏
	Addr_Mode_Set(1);  //垂直显示模式
	Page_Addr_Set(1,7);//设置页地址范围(1~7)
	
	//设置列地址范围
	if(slash == 1)     //斜杠
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
		  Col_Addr_Set(33,48);//第0位数字
		  num = s0;	
		break;
		
		case 1:
			Col_Addr_Set(50,65);//第1位数字
		  num = s1;
		break;
		
		case 2:
			Col_Addr_Set(67,82);//第2位数字
		  num = s2;
		break;
		
		case 3:
			Col_Addr_Set(84,99);//第3位数字
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
	//开始显示
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


//将float型 小数 变量转换为函数OLED_Disp()所用到的后4位参数
//
//注意：函数OLED_Disp()后4位参数：(0~9：数字)(10：小数点)
//
//参数：float num：待转换的float型变量
//      u8* seg：  生成的函数OLED_Disp()后4位参数的指针
//
//返回值：无
void FLOAT_Conv(float num, u8* seg)
{
	u16 temp;
	
	if(num < 10)              //小数点在第1位
	{
		temp   = (u16)(num*100);//扩大100倍，便于分离各位数
		seg[0] = temp/100;      //分离各位数
		seg[1] = 10;
		seg[2] = temp/10%10;
		seg[3] = temp%10;
	}
	else                      //小数点在第2位
	{
		temp   = (u16)(num*10); //扩大10倍，便于分离各位数
		seg[0] = temp/100;      //分离各位数
		seg[1] = temp/10%10;
		seg[2] = 10;
		seg[3] = temp%10;
	}
}


//将float型 整数 变量转换为函数OLED_Disp()所用到的后4位参数
//
//注意：函数OLED_Disp()后4位参数：(0~9：数字)(14：不显示)
//
//参数：float num：待转换的float型变量
//      u8* seg：  生成的函数OLED_Disp()后4位参数的指针
//
//返回值：无
void INT_Conv(float num, u8* seg)
{
	u8  i;
	u16 temp = (u16)num;
	
	//判断变量有效位数
	if((temp < 10000) && (temp >999))    //4位数
		i = 0;
	else if((temp < 1000) && (temp > 99))//3位数
		i = 1;
	else if((temp < 100)  && (temp > 9)) //2位数
		i = 2;
	else                                 //1位数
		i = 3;
	
	//转换为函数OLED_Disp()所需的参数
	seg[0] = temp/1000;
	seg[1] = temp/100%10;
	seg[2] = temp/10%10;
	seg[3] = temp%10;
	
	//去除首位的0，保证正常显示
	for(i=i;i>0;i--)
		seg[i-1] = 14;
}

