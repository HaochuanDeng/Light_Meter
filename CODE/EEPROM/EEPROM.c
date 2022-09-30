#include "EEPROM.h"


//初始化EEPROM
//
//参数：无
//
//返回值：无
void EEPROM_Init(void)
{
	IAP_TPS    = 0x20;//EEPROM工作频率 = 32MHz / 1MHz = 32 = 0x20
	IAP_CONTR |= 0x80;//使能EEPROM操作
}


//写入EEPROM第0~7扇区的任意偏移地址
//
//注意：写1个字节：40us
//    擦除1个扇区： 6ms
//
//参数：u8* date：待写入的数据指针
//      u8  sect：待写入的扇区(0~7)
//      u16 offset：扇区偏移(0~511)
//      u16 num： 待写入的字节数，最大512(同时应避免越界到下一个扇区)
//
//返回值：u8：0 写入成功
//            1 参数错误
//            2 扇区擦除失败
//            3 写入失败
u8 EEPROM_Write(u8* date, u8 sect, u16 offset, u16 num)
{
	u16 i;
	
	if((sect > 7) || (offset > 511) || (num > 512))
		return 1;                            //参数错误
	
	//扇区擦除
	IAP_ADDRH = (u8)((sect*512)>>8);       //设置扇区擦除地址
	IAP_ADDRL = (u8)(sect*512);
	IAP_CMD   = 0x03;                      //设置扇区擦除命令
	IAP_TRIG  = 0x5A;                      //触发命令
	IAP_TRIG  = 0xA5;
	if((IAP_CONTR & 0x10) == 0x10)
	{
		IAP_CONTR &= 0xEF;                   //清除EEPROM操作失败标志位
		return 2;                            //扇区擦除失败
	}
	
	//扇区写入
	IAP_CMD = 0x02;                        //设置扇区写入命令
	for(i=sect*512+offset; i<sect*512+num+offset; i++)
	{
		IAP_ADDRH = (u8)(i>>8);              //设置待写入的目标地址
	  IAP_ADDRL = (u8)i;
		IAP_DATA  = date[i-sect*512-offset]; //设置待写入数据
		IAP_TRIG  = 0x5A;                    //触发命令
	  IAP_TRIG  = 0xA5;
		if((IAP_CONTR & 0x10) == 0x10)
	  {
		  IAP_CONTR &= 0xEF;                 //清除EEPROM操作失败标志位
		  return 3;                          //写入失败
	  }
	}
	
	return 0;                              //写入成功
}


//读EEPROM第0~7扇区的任意偏移地址
//
//注意：读1个字节：4个系统时钟周期
//
//参数：u8* date：读出的数据将放到此数据指针所指向的数组中
//      u8  sect：待读取的扇区(0~7)
//      u16 offset：扇区偏移量(0~511)
//      u16 num： 待读取的字节数，最大512(同时应避免越界到下一个扇区)
//
//返回值：u8：0 读取成功
//            1 参数错误
//            2 读取失败
u8 EEPROM_Read(u8* date, u8 sect, u16 offset, u16 num)
{
	u16 i;
	
	if((sect > 7) || (offset > 511) || (num > 512))
		return 1;                          //参数错误
	
	IAP_CMD   = 0x01;                    //设置扇区读取命令
	for(i=sect*512+offset; i<sect*512+num+offset; i++)
	{
		IAP_ADDRH = (u8)(i>>8);            //设置待写入的目标地址
	  IAP_ADDRL = (u8)i;
		IAP_TRIG  = 0x5A;                  //触发命令
	  IAP_TRIG  = 0xA5;
		if((IAP_CONTR & 0x10) == 0x10)
	  {
		  IAP_CONTR &= 0xEF;               //清除EEPROM操作失败标志位
		  return 2;                        //读取失败
	  }
		date[i-sect*512-offset] = IAP_DATA;//读出读到的数据
	}
	
	return 0;                            //读取成功
}

