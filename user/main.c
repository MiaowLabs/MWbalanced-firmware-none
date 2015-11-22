
#include "includes.h"

/***************************************************************
** 函数名称:  main
** 功能描述: 主函数            
** 输　  入:   
** 输　  出:   
** 全局变量: 
** 作　  者: 喵呜实验室
** 淘    宝：Http://miaowlabs.taobao.com
** 日　  期: 2014年09月02日
***************************************************************/
void main()
{
	DisableInterrupts;//禁止总中断
	/*硬件底层初始化*/
	CLK_DIV_1();	 
	//所有I/O口全设为准双向，弱上拉模式
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
   
	//设置需要使用的PWM输出口为强推挽模式
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
	
	Delaynms(2000);	  //延时0.5s

	GetGyroOffset();
	LEDInit();        //指示灯初始化
	Delaynms(3000);	  //延时
										
	EnableInterrupts; //允许总中断

	while(1)
	{
		 
		BluetoothControl();
	
#if 0  //调试用 预编译命令
   		OutData[0] = g_fCarSpeed;
   		OutData[1] = g_fSpeedControlOutOld;
   		OutData[2] = g_fSpeedControlOut ;
   		OutData[3] = 100;
   
   		OutPut_Data();

#endif	

					
	}
}
