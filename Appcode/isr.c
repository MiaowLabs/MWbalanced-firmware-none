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
	
		SampleInputVoltage();		//�ɼ����ݺ���
		AngleControl();				//ֱ�����ƺ���
			
		GetMotorPulse();			//�ɼ����庯��
		
		/*�ö�ʱ�ж�����8ms��ǰ�沿�ֺ�������Լ3ms����������ж�֮����Լ5ms������ȶ�������Լ70cm*/
		TrigCnt++;
		if(TrigCnt>=10)	//ÿ�ж�10�δ���һ�γ�������ද�����������80ms
		{
			TrigCnt = 0;
			UltraTrig();
		}

	    UltraControl();	//�������

		g_ucSpeedControlCount++;
		if(g_ucSpeedControlCount>=10) 
			{	
				EliminateDirectionDeviation();	  //�̾���ֱ�߾������ƺ���
				SpeedControl();					  //�ٶȿ��ƺ���
				if(g_ucIRFlag==1)				  //һ������ѭ��-��APP����
				{
					DirectionControl();			  //����ѭ��������ƺ���
				}	
				g_ucSpeedControlCount=0;
			}
	
		MotorOutput();							  //����������
 		
  		g_ucLEDCount++;
   		if(g_ucLEDCount >=125) 	                  //ָʾ��1�뽻����˸
   		{
			g_ucLEDCount=0;
	 		LED0=~LED0;
   		}
			 
}



void Timer0_Update(void) interrupt  1
{
    
}
				 
/*----------------------------
UART2 �жϷ������
-----------------------------*/
void Uart2() interrupt 8 using 1
{
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;         //���S2RIλ
        g_ucRxd2 = S2BUF;              
    }
	g_ucUart2Flag = 1;
	
	/*
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy = 0;               //��æ��־
    }
	*/
}
