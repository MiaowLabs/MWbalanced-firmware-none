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
** 备    注: 启动方式：直接扶起开机即可。
********************喵呜实验室版权所有**************************
***************************************************************/
void main()
{
	DisableInterrupts;//禁止总中断

	CLK_DIV_1();	  //设置MCU工作频率为内部RC时钟
	DriversInit();	  //芯片底层驱动初始化
	MPU6050Init();	  //MPU6050初始化
	CarStandInit();	  //直立小车功能函数初始化
	
	Delaynms(100);	  //延时
	ON_LED0;		  //灯亮
	Delaynms(100);	  //延时
									
	EnableInterrupts;//允许总中断	 

	while(1)
	{ 
		 
	BluetoothControl();	//蓝牙控制函数
						 
#if DEBUG_UART  //调试启用 预编译命令

   	OutData[0] = g_fCarAngle;
   	OutData[1] = g_fGravityAngle;
   	OutData[2] = g_fGyroAngleSpeed ;
   	OutData[3] = 45;  
   	OutPut_Data();		
		 	  
#endif	 		
					
	}
}
