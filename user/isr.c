
#include "includes.h"




/***************************************************************
** 函数名称: Timer1_Update
** 功能描述: Timer1中断函数
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
void Timer1_Update(void) interrupt  3		//125hz
{ 
	SampleInputVoltage();
	AngleControl();	

	GetMotorPulse();
	g_ucSpeedControlCount++;
	if(g_ucSpeedControlCount>=10) //80ms
		{	
			SpeedControl();	
			g_ucSpeedControlCount=0;
		}
	MotorOutput();		
	#if 0  //调试用 预编译命令
   OutData[0] = g_fCarSpeed;
   	OutData[1] = g_iLeftMotorPulseSigma;
   	OutData[2] = g_fSpeedControlOut ;
   	OutData[3] = g_fCarPosition;
   
   	OutPut_Data();

#endif					   	 
#if 1  //调试用 
  	g_ucLEDCount++;
   	if(g_ucLEDCount >=125) //LED1灯1秒交替闪烁
   	{
   	 	g_ucLEDCount=0;
	 	LED0=~LED0;
   	}
#endif	   				 
}

void Timer0_Update(void) interrupt  1
{
    
	g_ucSpeedCountCarry++;
//	LED2=~LED2;
}



/***************************************************************
** 函数名称: GetLeftMotorPulse
** 功能描述: 左电机脉冲捕捉函数
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/

void GetLeftMotorPulse(void) interrupt 2
{
#if 0
   if(LeftMotorDirection == 1) 			//1 
   {
      g_iLeftMotorPulse++;  //A脉冲低电平则B脉冲加一
      //LED0=~LED0;
   }	 
   else 
   {
      g_iLeftMotorPulse--; 
      //LED0=~LED0;
   }
#else
	  g_iLeftMotorPulse++;
	  LED0=~LED0;
#endif	
} 


/***************************************************************
** 函数名称: GetRightMotorPulse
** 功能描述: 右电机脉冲捕捉函数
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
/*
void GetRightMotorPulse(void) interrupt 0
{
#if 0
   if(RightMotorDirection == 0) 	   //0
   {
      g_liRightMotorPulse++; 	//A脉冲低电平则B脉冲加一
   		//LED2=~LED2;
   }
   else 
   {
      g_liRightMotorPulse--;
		//LED2=~LED2;
   }
#else
		g_liRightMotorPulse++;
		//LED2=~LED2;
#endif
}
*/						 