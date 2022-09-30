#ifndef __EEPROM_H
#define __EEPROM_H


#include "STC8xxxx.H"


void EEPROM_Init (void);
u8   EEPROM_Write(u8* date, u8 sect, u16 offset, u16 num);
u8   EEPROM_Read (u8* date, u8 sect, u16 offset, u16 num);


#endif

