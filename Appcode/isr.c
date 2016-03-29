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
** 函数名称: Timer1_Update
** 功能描述: 125hz中断服务函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void Timer1_Update(void) interrupt  3		//125hz
{ 
	
		SampleInputVoltage();
		AngleControl();
			
		GetMotorPulse();
		
		g_ucSpeedControlCount++;
		if(g_ucSpeedControlCount>=10) 
			{	
				EliminateDirectionDeviation();
				SpeedControl();					
				g_ucSpeedControlCount=0;
			}
	
		MotorOutput();	
#if 0//DEBUG_UART  //调试启用 预编译命令
	
   	OutData[0] = g_fCarAngle;
   	OutData[1] = g_fGyroAngleSpeed;
   	OutData[2] = g_fGravityAngle ;
   	OutData[3] = 45;  

   	OutPut_Data();		
		 	  
#endif	 		
  		g_ucLEDCount++;
   		if(g_ucLEDCount >=125) //LED1灯1秒交替闪烁
   		{
			g_ucLEDCount=0;
	 		LED0=~LED0;
   		}
			 
}

void Timer0_Update(void) interrupt  1
{
    
//	g_ucSpeedCountCarry++;
//	LED2=~LED2;
}
				 