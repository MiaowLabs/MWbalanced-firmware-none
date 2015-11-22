#ifndef __LED_H__
#define __LED_H__

#include "IAP15W4K61S4.h"

sbit LED0 = P4^4;
sbit LED1 = P2^0;


void LEDInit(void);

#endif