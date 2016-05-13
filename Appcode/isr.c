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
	
		SampleInputVoltage();		//采集数据函数
		AngleControl();				//直立控制函数
			
		GetMotorPulse();			//采集脉冲函数
		
		/*该定时中断周期8ms，前面部分函数运行约3ms，因此两次中断之间间隔约5ms，最大稳定测距距离约70cm*/
		TrigCnt++;
		if(TrigCnt>=10)	//每中断10次触发一次超声波测距动作，触发间隔80ms
		{
			TrigCnt = 0;
			UltraTrig();
		}

	    UltraControl();	//红外控制

		g_ucSpeedControlCount++;
		if(g_ucSpeedControlCount>=10) 
			{	
				EliminateDirectionDeviation();	  //短距离直线纠正控制函数
				SpeedControl();					  //速度控制函数
				if(g_ucIRFlag==1)				  //一键启动循迹-由APP控制
				{
					DirectionControl();			  //红外循迹方向控制函数
				}	
				g_ucSpeedControlCount=0;
			}
	
		MotorOutput();							  //电机输出函数
 		
  		g_ucLEDCount++;
   		if(g_ucLEDCount >=125) 	                  //指示灯1秒交替闪烁
   		{
			g_ucLEDCount=0;
	 		LED0=~LED0;
   		}
			 
}



void Timer0_Update(void) interrupt  1
{
    
}
				 
/*----------------------------
UART2 中断服务程序
-----------------------------*/
void Uart2() interrupt 8 using 1
{
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;         //清除S2RI位
        g_ucRxd2 = S2BUF;              
    }
	g_ucUart2Flag = 1;
	
	/*
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除S2TI位
        busy = 0;               //清忙标志
    }
	*/
}
