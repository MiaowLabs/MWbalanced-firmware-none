#include "includes.h"




void ServoInit(void)
{
	//定时器0 16位自动重装，1T模式，78us定时中断
	AUXR |= 0x80;	
	TMOD &= 0xF0;	
	TL0 = 0xE5;		
	TH0 = 0xF9;		
	TF0 = 0;		
	TR0 = 1;				  

	//两路8位PWM输出，定时器0 溢出中断作为时钟源
	CMOD	|=  0x04;	// choose T0 output as source clk
	CCAPM0	|=  0x42;	// PWM mode
	CCAPM1	|=  0x42;	//

	PCA_PWM0 |=0x00;	// 8bit PWM mode 
	PCA_PWM1 |=0x00;

	CCAP0H=CCAP0L = ~0x13;
	CCAP1H=CCAP1L = ~0x13;

	AUXR1 |= 0x20;		// P2.5/CCP0  P2.6/CCP1

	CCON  |= 0x40;		//start PAC module

}



/*

* speed: 
	0x06: 0°，0x0d: 45°，0x13: 90°，0x19: 135°，0x20: 180° 。
* channel:
	0: 通道1，1: 通道2
	
*/ 

 void ServoCtr(unsigned char speed,unsigned char channel)
 {
 	if((speed<0x06)||(speed>0x20))return;
	
 	if(channel==0)CCAP0H=CCAP0L = ~speed;// 
	if(channel==1)CCAP1H=CCAP1L = ~speed;// 
 }


	
