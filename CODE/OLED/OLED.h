#ifndef __OLED_H
#define __OLED_H


#include "IIC.h"
#include "DELAY.h"
#include "EEPROM.h"


//SSD1306 IIC地址定义(对于SSD1306，IIC模式只能写，不能读)
#define OLED_ADDR_W 0x78


//SSD1306命令函数
void   Contrast_Set            (u8 leve);
void   Entire_Disp_ON_OFF      (u8 arg);
void   Disp_NOR_INV            (u8 arg);
void   Disp_ON_OFF             (u8 arg);
//void   R_L_Horiz_Scroll        (u8 lr, u8 sp, u8 bf, u8 ep);
//void   R_L_Horiz_Verti_Scroll  (u8 lr, u8 sp, u8 bf, u8 ep, u8 vofset);
//void   Stop_Scroll             (void);
//void   Go_Scroll               (void);
//void   Verti_Scroll_Area_Set   (u8 rf, u8 ra);
//void   Lower_Col_Addr_Set      (u8 addr);
//void   Higher_Col_Addr_Set     (u8 addr);
void   Addr_Mode_Set           (u8 arg);
void   Col_Addr_Set            (u8 sa, u8 ea);
void   Page_Addr_Set           (u8 sa, u8 ea);
//void   Page_Start_Addr_Set     (u8 addr);
void   Disp_Start_Line_Set     (u8 line);
void   Seg_Re_Map              (u8 arg);
void   Mux_Set                 (u8 mux);
void   COM_Scan_Dir_Set        (u8 arg);
void   Disp_Offset_Set         (u8 ofset);
void   COM_Pin_HW_Config       (u8 conf);
void   DCLK_Fosc_Set           (u8 dclk, u8 fosc);
void   Pre_Charge_Period_Set   (u8 p1, u8 p2);
void   Vcomh_Deselect_Level_Set(u8 leve);
//void   NOP                     (void);
void   Charge_Pump_Set         (u8 arg);

//OLED控制函数
void OLED_Init               (void);
//void OLED_Power_Down         (void);
//void OLED_Sleep              (void);
//void OLED_Wake               (void);

//OLED显示函数
void OLED_Disp               (u8 slash, u8 s0, u8 s1, u8 s2, u8 s3);
void FLOAT_Conv              (float num, u8* seg);
void INT_Conv                (float num, u8* seg);


#endif

