
#include "includes.h"

/***************************************************************
** ��������:  main
** ��������: ������            
** �䡡  ��:   
** �䡡  ��:   
** ȫ�ֱ���: 
** ����  ��: ����ʵ����
** ��    ����Http://miaowlabs.taobao.com
** �ա�  ��: 2014��09��02��
***************************************************************/
void main()
{
	DisableInterrupts;//��ֹ���ж�
	/*Ӳ���ײ��ʼ��*/
	CLK_DIV_1();	 
	//����I/O��ȫ��Ϊ׼˫��������ģʽ
	P0M0=0x00;
	P0M1=0x00;
	P1M0=0x00;
	P1M1=0x00;
	P2M0=0x00;      
	P2M1=0x00;
	P3M0=0x00;
	P3M1=0x00;	
	P4M0=0x00;
	P4M1=0x00;
	P5M0=0x00;
	P5M1=0x00;	   
   
	//������Ҫʹ�õ�PWM�����Ϊǿ����ģʽ
	P2M0=0x0e;
	P2M1=0x00;
	P3M0=0x80;
	P3M1=0x00;
		  
//	Timer0Init();	
	Timer1Init();
	PWMInit();
	Uart1Init();  //9600bps@33.000MHz
	Uart2Init();  //115200bps@33.000MHz
	Timer3Timer4Init();

	MPU6050Init();	
	CarUpstandInit();
	
	Delaynms(2000);	  //��ʱ0.5s

	GetGyroOffset();
	LEDInit();        //ָʾ�Ƴ�ʼ��
	Delaynms(3000);	  //��ʱ
										
	EnableInterrupts; //�������ж�

	while(1)
	{
		 
		BluetoothControl();
	
#if 0  //������ Ԥ��������
   		OutData[0] = g_fCarSpeed;
   		OutData[1] = g_fSpeedControlOutOld;
   		OutData[2] = g_fSpeedControlOut ;
   		OutData[3] = 100;
   
   		OutPut_Data();

#endif	

					
	}
}
