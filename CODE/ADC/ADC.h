#ifndef __ADC_H
#define __ADC_H


#include "STC8xxxx.H"
#include "DELAY.h"


void ADC_Init   (void);
u16  ADC_Convert(u8 chan);


#endif

