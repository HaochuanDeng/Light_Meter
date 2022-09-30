#ifndef __IIC_H
#define __IIC_H


#include "STC8xxxx.H"
#include "DELAY.h"


//IO口位定义
sbit SDA = P1^4;
sbit SCL = P1^5;


void IIC_Init     (void);
void IIC_Start    (void);
void IIC_Stop     (void);
u8   IIC_WaitResp (void);
void IIC_Resp     (u8 resp);
void IIC_WriteByte(u8 byte);
u8   IIC_ReadByte (void);


#endif

