/********************************************************************
���ߣ�Songyimiao
��������: 20151129
�汾��V2.0
����ʵ���Ұ�Ȩ����
/********************************************************************/
#include "includes.h"

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: Timer1_Update
** ��������: 125hz�жϷ�����            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void Timer1_Update(void) interrupt  3		//125hz
{ 
	
		SampleInputVoltage();
		AngleControl();	
		GetMotorPulse();
		
		g_ucSpeedControlCount++;
		if(g_ucSpeedControlCount>=10) 
			{	
				SpeedControl();	
				g_ucSpeedControlCount=0;
			}
	
		MotorOutput();	
					   	 
  		g_ucLEDCount++;
   		if(g_ucLEDCount >=125) //LED1��1�뽻����˸
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
				 