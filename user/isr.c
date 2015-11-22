
#include "includes.h"




/***************************************************************
** ��������: Timer1_Update
** ��������: Timer1�жϺ���
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
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
	#if 0  //������ Ԥ��������
   OutData[0] = g_fCarSpeed;
   	OutData[1] = g_iLeftMotorPulseSigma;
   	OutData[2] = g_fSpeedControlOut ;
   	OutData[3] = g_fCarPosition;
   
   	OutPut_Data();

#endif					   	 
#if 1  //������ 
  	g_ucLEDCount++;
   	if(g_ucLEDCount >=125) //LED1��1�뽻����˸
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
** ��������: GetLeftMotorPulse
** ��������: �������岶׽����
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/

void GetLeftMotorPulse(void) interrupt 2
{
#if 0
   if(LeftMotorDirection == 1) 			//1 
   {
      g_iLeftMotorPulse++;  //A����͵�ƽ��B�����һ
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
** ��������: GetRightMotorPulse
** ��������: �ҵ�����岶׽����
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
/*
void GetRightMotorPulse(void) interrupt 0
{
#if 0
   if(RightMotorDirection == 0) 	   //0
   {
      g_liRightMotorPulse++; 	//A����͵�ƽ��B�����һ
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