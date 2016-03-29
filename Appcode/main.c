/********************************************************************
作者：Songyimiao
建立日期: 20151129
版本：V2.0
喵呜实验室版权所有
/********************************************************************/
#include "includes.h"

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: main()
** 功能描述: 主函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void main()
{
	DisableInterrupts;//禁止总中断

	CLK_DIV_1();	  //设置MCU工作频率为内部RC时钟
	DriversInit();	
	MPU6050Init();	
	CarStandInit();
	
	Delaynms(200);	 //延时1s
	//GetGyroRevise();
	ON_LED0;
	//Delaynms(2000);	 //延时1s
	//Delaynms(200);	 //延时3s
									
	EnableInterrupts;//允许总中断	 

	while(1)
	{ 
		BluetoothControl();
	
#if 0//DEBUG_UART  //调试启用 预编译命令
	
   	OutData[0] = g_fCarAngle;
   	OutData[1] = g_fGyroAngleSpeed;
   	OutData[2] = g_fGravityAngle ;
   	OutData[3] = 45;  
/*	OutData[0] = g_fCarSpeed;
   	OutData[1] = g_fCarPosition;
   	OutData[2] = g_iLeftMotorPulseSigma ;
   	OutData[3] = g_iLeftMotorPulse;  */
   	OutPut_Data();		
		 	  
#endif	 		
					
	}
}
