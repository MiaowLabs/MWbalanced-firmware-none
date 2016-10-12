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
** ��������: main()
** ��������: ������            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void main()
{
	DisableInterrupts;//��ֹ���ж�

	DriversInit();	  //MCU�ײ�������ʼ��
	MPU6050Init();	  //MPU6050���ó�ʼ��
	CarStandInit();	  //Ӧ�ó��������ʼ��

#if ULTRA_EN //�������Ͷ���������� ��ʱ��0����˲���ͬʱʹ������ģ��
	UltraInit();	  //���������ó�ʼ��
#else
	ServoInit();	  //���������ʼ��
#endif

	Delaynms(50);	  //��ʱ50ms 
	ON_LED0;		  //ָʾ�����𣬱�ʾ��ʼ�����
	
	EnableInterrupts; //ʹ�����ж�	 

	while(1)
	{ 					
	
		STC_ISP();
		//UART1SendByte(0x22);
		Delaynms(1);	        //��ʱ5ms 
		if(g_ucUart2Flag>=1)
		{
			setValues(g_cUart2Buffer);	//����ң�غ���
			g_ucUart2Flag = 0;
		//	UART1SendByte(0x11);
		}
		BatteryChecker();		//��ص�����⣨��������ʱ������ƣ�
		if(EchoFlag)			//�ж��Ƿ������
		{ 
			g_ucUltraDis=UltraDis();//��ȡ�����������
		}
		 
#if DEBUG_UART  //�������� Ԥ�������� 
//��Ҫ�۲첨�ν��е��ԣ��轫DEBUG_UART���ó�1���ð汾����Ҫע���������ƺ���
	
   		OutData[0] = g_fCarAngle;
   		OutData[1] = g_fGyroAngleSpeed;
   		OutData[2] = g_fGravityAngle;
   		OutData[3] = g_fAngleControlOut;
   		OutPut_Data();		
		 	  
#endif	 		
					
	}
}
