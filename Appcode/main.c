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

	CLK_DIV_1();	  //设置MCU工作频率为外部晶振频率
	DriversInit();	  //MCU底层驱动初始化
	MPU6050Init();	  //MPU6050配置初始化
	CarStandInit();	  //应用程序变量初始化
	
	Delaynms(200);	  //延时200ms 
	ON_LED0;		  //绿灯亮起，表示初始化完毕
	
	EnableInterrupts; //允许总中断	 

	while(1)
	{ 
		BluetoothControl();	//蓝牙控制函数
	
#if DEBUG_UART  //调试启用 预编译命令  若要观察波形进行调试，需将DEBUG_UART设置成1并注释蓝牙控制函数
	
   		OutData[0] = g_fCarAngle;
   		OutData[1] = g_fGyroAngleSpeed;
   		OutData[2] = g_fGravityAngle ;
   		OutData[3] = 45;  

   		OutPut_Data();		
		 	  
#endif	 		
					
	}
}
