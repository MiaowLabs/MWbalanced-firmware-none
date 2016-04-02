/********************************************************************
���ߣ�Songyimiao
��������: 20151129
�汾��V2.0
����ʵ���Ұ�Ȩ����
/********************************************************************/
#include "includes.h"

unsigned int xdata g_uiStartCount;
unsigned char xdata g_ucLEDCount;
/******������Ʋ���******/
int   g_iCarSpeedSet;
float g_fSpeedControlOut;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
/******�Ƕȿ��Ʋ���******/
int   g_iAccelInputVoltage_X_Axis ;	//���ٶ�X������
int   g_iGyroInputVoltage_Y_Axis  ;	//������Y������
int   g_iGyroInputVoltage_Z_Axis  ;	//������Y������

float g_fCarAngle;         			//��ģ���
float g_fGyroAngleSpeed;			//���ٶ�      			
float g_fGyroscopeAngleIntegral;	//���ٶȻ���ֵ
float g_fGravityAngle;				//���ٶȳ�������õ������
int g_iGyroOffset;
/******�ٶȿ��Ʋ���******/
int   g_iLeftMotorPulse;
int   g_iRightMotorPulse;
int   g_iLeftMotorPulseSigma;
int   g_iRightMotorPulseSigma;
float g_fCarSpeed;
float g_fCarSpeedOld;
float g_fCarPosition;
unsigned char g_ucSpeedControlPeriod ;
unsigned char g_ucSpeedControlCount ;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
float code g_fcAngle_P = 90.0;  //  85
float code g_fcAngle_D = 3.0;   // 2.8	
float code g_fcSpeed_P = 12.0 ; //  15
float code g_fcSpeed_I = 1.4;   // 1.7
float code g_fcDirection_P = 5.0;		   
/******�������Ʋ���******/
float xdata g_fBluetoothSpeed;
float xdata g_fBluetoothDirection;

float xdata g_fDirectionDeviation;

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: DriversInit
** ��������: �ײ�������ʼ��            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void DriversInit(void)
{

	GPIOInit();
  	Timer1Init();
	PWMInit();
	Uart1Init();
	Uart2Init();
	Timer3Timer4Init();

}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: CarStandInit
** ��������: ֱ��������ʼ��            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void CarStandInit()
{
	g_iAccelInputVoltage_X_Axis = g_iGyroInputVoltage_Y_Axis = 0;
	g_iLeftMotorPulse = g_iRightMotorPulse = 0;
	g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;
	g_iCarSpeedSet = 0;

	g_iCarSpeedSet=0;
	g_fCarSpeed    = 0;
	g_fCarSpeedOld = 0;
	g_fCarPosition = 0;
	g_fCarAngle    = 0;
	g_fGyroAngleSpeed = 0;
	g_fGravityAngle   = 0;
	g_fGyroscopeAngleIntegral = 0;

	g_fAngleControlOut = g_fSpeedControlOut = 0;

	g_fLeftMotorOut    = g_fRightMotorOut   = 0;
	g_fBluetoothSpeed  = g_fBluetoothDirection = 0;

    g_ucLEDCount = 0;				
	g_uiStartCount= 0;
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: DataSynthesis
** ��������: ���ݺϳɺ���            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
int DataSynthesis(unsigned char REG_Address)	
{
	char idata uiHighByte; /*�߰�λ*/
	char idata ucLowByte; /*�Ͱ�λ*/

	uiHighByte = Single_ReadI2C(REG_Address)  ;
	ucLowByte  = Single_ReadI2C(REG_Address+1);

	return ((uiHighByte << 8) + ucLowByte);   /*���غϳ�����*/
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: SampleInputVoltage
** ��������: MPU6050��������            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void SampleInputVoltage(void)
{	

	g_iGyroInputVoltage_Y_Axis   = DataSynthesis(GYRO_YOUT_H) ; //������Y��
    g_iAccelInputVoltage_X_Axis  = DataSynthesis(ACCEL_XOUT_H); //���ٶ�X��	
	
	g_iGyroInputVoltage_Z_Axis   = DataSynthesis(GYRO_ZOUT_H) ; //������Y��	

}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: GyroRevise
** ��������: ������У������            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void GetGyroRevise()
{
	long int tempsum;
	int temp;
	for(temp=0;temp<500;temp++)
	{
		tempsum += DataSynthesis(GYRO_YOUT_H) ;
	}
	g_iGyroOffset = (int)(tempsum/500);
	tempsum=0;
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: SetMotorVoltageAndDirection
** ��������: ������ú���            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void SetMotorVoltageAndDirection(int iLeftVoltage,int iRightVoltage)
{
	int iLeftMotorValue;
	int iRighttMotorValue;	

    if(iLeftVoltage<0)
    {	
      AIN1 = 1;				      //�ҵ��ǰ��	�Ƕ�Ϊ��  �ٶ�Ϊ��
      AIN2 = 0;
      iLeftMotorValue = (-iLeftVoltage);
    }
    else 
    {	
      AIN1 = 0;				      //�ҵ������  �Ƕ�Ϊ��  �ٶ�Ϊ��
      AIN2 = 1; 
      iLeftMotorValue = iLeftVoltage;
    }

    if(iRightVoltage<0)
    {	
      BIN1 = 1;				      //����ǰ��  �Ƕ�Ϊ��  �ٶ�Ϊ��
      BIN2 = 0;
      iRighttMotorValue = (-iRightVoltage);
    }
    else
    {	
      BIN1 = 0;				      //��������  �Ƕ�Ϊ��  �ٶ�Ϊ��
      BIN2 = 1; 
      iRighttMotorValue = iRightVoltage;
    }

	iLeftMotorValue   = (1000 - iLeftMotorValue)  ;	   
	iRighttMotorValue = (1000 - iRighttMotorValue);
	
  	PWM3T1 = iLeftMotorValue  ;  
   	PWM4T1 = iRighttMotorValue;  

	#if IF_CAR_FALL		 /*�жϳ����Ƿ������������*/

	if(g_fCarAngle > 30 || g_fCarAngle < (-30))
	{
		AIN1 = 0;				      //�ҵ��ǰ��	�Ƕ�Ϊ��  �ٶ�Ϊ��
      	AIN2 = 0; 
		BIN1 = 0;				      //�ҵ��ǰ��	�Ƕ�Ϊ��  �ٶ�Ϊ��
      	BIN2 = 0;   
	}

#endif

}
/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: MotorOutput
** ��������: ����������            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: ��ֱ�����ơ��ٶȿ��ơ�������Ƶ���������е���,����
			 �����������������������������
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void MotorOutput(void)
{

	g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection + g_fDirectionDeviation;
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection - g_fDirectionDeviation;			
	
	/*������������*/
	if(g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if(g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if(g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if(g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*������ʹ����Ǳ�֤��������ᳬ��PWM�������̷�Χ��*/	
	if(g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if(g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if(g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if(g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;

    SetMotorVoltageAndDirection(g_fLeftMotorOut,g_fRightMotorOut);
}


/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: GetMotorPulse
** ��������: ���������庯��            
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void GetMotorPulse(void)
{
	g_iRightMotorPulse = (T4H<<8) + T4L;
	g_iLeftMotorPulse  = (T3H<<8) + T3L;
	T4T3M&=	0x77;
	T4H=T4L=0;
	T3H=T3L=0;
	T4T3M |= 0xCC;

	if(!MOTOR_LEFT_SPEED_POSITIVE)  g_iLeftMotorPulse  = -g_iLeftMotorPulse ; 
	if(!MOTOR_RIGHT_SPEED_POSITIVE) g_iRightMotorPulse = -g_iRightMotorPulse;
	
	g_iLeftMotorPulseSigma += g_iLeftMotorPulse;
	g_iRightMotorPulseSigma += g_iRightMotorPulse;	  
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: SpeedControl
** ��������: �ٶȻ����ƺ���           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void SpeedControl(void)
{  
	float fP, fDelta;
	float fI;
	
	g_fCarSpeed = (float)(g_iLeftMotorPulseSigma  + g_iRightMotorPulseSigma ) * 0.5f;
    g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;	  //ȫ�ֱ��� ע�⼰ʱ����

	/*��ͨ�˲�*/ 
    g_fCarSpeed = (float)(g_fCarSpeedOld * 0.2f + g_fCarSpeed * 0.8f) ;
	g_fCarSpeedOld = g_fCarSpeed;
		   														 
	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;
	fP = fDelta * g_fcSpeed_P;
	fI = fDelta * g_fcSpeed_I;
	g_fCarPosition += fI;

	/*������������*/			  
	if((int)g_fCarPosition > SPEED_CONTROL_OUT_MAX)    g_fCarPosition = SPEED_CONTROL_OUT_MAX;
	if((int)g_fCarPosition < SPEED_CONTROL_OUT_MIN)    g_fCarPosition = SPEED_CONTROL_OUT_MIN;
	g_fCarPosition += g_fBluetoothSpeed;

	g_fSpeedControlOut = fP + g_fCarPosition;

}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 2015��11��29��
** ��������: AngleControl
** ��������: �ǶȻ����ƺ���           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void AngleControl(void)	 
{  
	//ȥ�����ƫ��,����õ����ٶȴ������ĽǶȣ����ȣ�
	g_fGravityAngle = (float)(g_iAccelInputVoltage_X_Axis - GRAVITY_OFFSET) / 16384.0f;
	// 57.2957795=180/3.1415926535898 ����ת��Ϊ��
	g_fGravityAngle = g_fGravityAngle * 57.2957795f ;

	g_fGyroAngleSpeed = (g_iGyroInputVoltage_Y_Axis - GYRO_OFFSET) / GYROSCOPE_ANGLE_RATIO *(-1);
	//�����˲�
	g_fCarAngle = 0.99f*(g_fCarAngle + g_fGyroAngleSpeed * 0.008f) + 0.01f * g_fGravityAngle;
	//�ǶȻ�PID����
	g_fAngleControlOut = (CAR_ANGLE_SET - g_fCarAngle)* g_fcAngle_P + \
	(CAR_ANGULARSPEED_SET - g_fGyroAngleSpeed )* g_fcAngle_D ;	   
	 
}

/***************************************************************
** ��������: BluetoothControl
** ��������: �������ƺ���
             �ֻ���������
			 ǰ����0x01    ���ˣ�0x02
             ��  0x03    �ң�  0x04
             ֹͣ��0x10
             ���ܼ������Ա���λ��������չ����
             ��������0x07
			 ��������0x08
			 �����ǰ����0x09  ������ǰ����0x0A
			 �����غ��ˣ�0x0B  �����غ��ˣ�0x0C   Ѳ��������0x0D
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void BluetoothControl(void)	 
{
	unsigned char xdata ucBluetoothValue;

	//LED0=~LED0;

	ucBluetoothValue = UART2ReceiveByte();		
		
	switch (ucBluetoothValue)
	{

	  case 0x02 : g_fBluetoothSpeed =   40 ;  break;//����
	  case 0x01 : g_fBluetoothSpeed = (-40);  break;//ǰ��
	  case 0x09 : g_fBluetoothSpeed =   60;   break;
	  case 0x0A : g_fBluetoothSpeed = (-60);  break;
	  case 0x0B : g_fBluetoothSpeed =   20;  break;
	  case 0x0C : g_fBluetoothSpeed = (-20);  break;
	  case 0x03 : g_fBluetoothDirection =   200 ;  break;//��ת
	  case 0x04 : g_fBluetoothDirection = (-200);  break;//��ת
	  case 0x05 : g_iCarSpeedSet =   20 ; break ;
	  case 0x06 : g_iCarSpeedSet = (-20); break ;
	  case 0x07 : g_fBluetoothDirection =   400 ;  break;
	  case 0x08 : g_fBluetoothDirection = (-400);  break;
	  case 0x0D : g_iCarSpeedSet = 8;  break;	   //ǰ��
	  default : g_fBluetoothSpeed = 0; g_fBluetoothDirection = 0;g_iCarSpeedSet=0;break;
	}
}

void EliminateDirectionDeviation(void)
{
	int Delta=0;

	Delta = g_iLeftMotorPulseSigma - g_iRightMotorPulseSigma;

	g_fDirectionDeviation = Delta * g_fcDirection_P * (-1);


}