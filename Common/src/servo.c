#include "includes.h"




void ServoInit(void)
{
	//��ʱ��0 16λ�Զ���װ��1Tģʽ��78us��ʱ�ж�
	AUXR |= 0x80;	
	TMOD &= 0xF0;	
	TL0 = 0xE5;		
	TH0 = 0xF9;		
	TF0 = 0;		
	TR0 = 1;				  

	//��·8λPWM�������ʱ��0 ����ж���Ϊʱ��Դ
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
	0x06: 0�㣬0x0d: 45�㣬0x13: 90�㣬0x19: 135�㣬0x20: 180�� ��
* channel:
	0: ͨ��1��1: ͨ��2
	
*/ 

 void ServoCtr(unsigned char speed,unsigned char channel)
 {
 	if((speed<0x06)||(speed>0x20))return;
	
 	if(channel==0)CCAP0H=CCAP0L = ~speed;// 
	if(channel==1)CCAP1H=CCAP1L = ~speed;// 
 }


	
