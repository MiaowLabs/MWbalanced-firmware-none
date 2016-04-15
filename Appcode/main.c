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

	CLK_DIV_1();	  //����MCU����Ƶ��Ϊ�ⲿ����Ƶ��
	DriversInit();	  //MCU�ײ�������ʼ��
	MPU6050Init();	  //MPU6050���ó�ʼ��
	CarStandInit();	  //Ӧ�ó��������ʼ��
	
	Delaynms(200);	  //��ʱ200ms 
	ON_LED0;		  //ָʾ�����𣬱�ʾ��ʼ�����
	
	EnableInterrupts; //ʹ�����ж�	 

	while(1)
	{ 
		if(g_ucUart2Flag>=1)
		{
			BluetoothControl();	//�������ƺ���
			g_ucUart2Flag = 0;
		}
		BatteryChecker();

#if DEBUG_UART  //�������� Ԥ�������� 
//��Ҫ�۲첨�ν��е��ԣ��轫DEBUG_UART���ó�1���ð汾����Ҫע���������ƺ���
	
   		OutData[0] = g_fCarAngle;
   		OutData[1] = g_fGyroAngleSpeed;
   		OutData[2] = g_fGravityAngle ;
   		OutData[3] = g_ucRxd2;
   		OutPut_Data();		
		 	  
#endif	 		
					
	}
}
