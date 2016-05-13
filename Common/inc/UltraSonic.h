#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

extern void UltraInit(void);
extern void UltraTrig(void);
extern unsigned char  UltraDis(void);

extern bit EchoFlag;
extern unsigned char xdata TrigCnt;

#endif

