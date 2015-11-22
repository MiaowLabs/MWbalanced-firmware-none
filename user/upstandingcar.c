

#include "includes.h"


unsigned int flag_ok;
unsigned char xdata g_ucLEDCount;
int g_iCarSpeedSet;
/******������Ʋ���******/
float g_fSpeedControlOut;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
/******�Ƕȿ��Ʋ���******/
int   g_iAccelInputVoltage_Y_Axis ;	//���ٶ�Y������
int   g_iGyroInputVoltage_X_Axis  ;	//������X������
long int  g_liAccAdd;
long int  g_liGyroAdd;
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
//unsigned char g_ucSpeedCountCarry;
float g_fSpeedControlOutNew	   ;
float g_fSpeedControlOutOld;
unsigned char g_ucSpeedControlPeriod ;
unsigned char g_ucSpeedControlCount ;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
float code g_fcAngle_P = 1300.0;//1300
float code g_fcAngle_D = 19.0;	
float code g_fcSpeed_P = 200.0 ; //200
float code g_fcSpeed_I = 5.5;		   
/******�������Ʋ���******/
float xdata g_fBluetoothSpeed;
float xdata g_fBluetoothDirection;
unsigned char  xdata g_uc2msEventCount;

/***************************************************************
** ��������: CarUpstandInit
** ��������: ȫ�ֱ�����ʼ������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void CarUpstandInit()
{
	g_iAccelInputVoltage_Y_Axis = g_iGyroInputVoltage_X_Axis = 0;
	g_iLeftMotorPulse = g_iRightMotorPulse = 0;

	g_fCarSpeed    = 0;
	g_fCarPosition = 0;
	g_fCarAngle    = 0;
	g_fGyroAngleSpeed = 0;
	g_fGravityAngle   = 0;
	g_fGyroscopeAngleIntegral = 0;

	g_fAngleControlOut = g_fSpeedControlOut = 0;

	g_fLeftMotorOut    = g_fRightMotorOut   = 0;
	g_fBluetoothSpeed  = g_fBluetoothDirection = 0;

    g_ucLEDCount = 0;
	g_ucSpeedCountCarry = 0;

	g_fSpeedControlOutNew = g_fSpeedControlOutOld = 0;

	g_iCarSpeedSet=0;

	flag_ok=0;

}
/***************************************************************
** ��������: DataSynthesis
** ��������: MPU6050���ݺϳɺ���
** �䡡��:   REG_Address : ��ַ
** �䡡��:   �ϳ�����
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
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
** ��������: SampleInputVoltage
** ��������: ��������             
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void SampleInputVoltage(void)
{	
#if 0 
    /*
	N�ξ�ֵ�˲����˴�Nȡ20��
	�ή��Ƶ�죬NֵҪ�ʵ������˹����С��
	*/	
	unsigned char ucValue;   
	for(ucValue = 0 ; ucValue < 20 ; ucValue ++)
	{
		g_iAccelInputVoltage_Y_Axis  = DataSynthesis(ACCEL_YOUT_H);//���ٶ�Y��
		g_iGyroInputVoltage_X_Axis = DataSynthesis(GYRO_XOUT_H);//������X��
	
		g_liAccAdd += g_iAccelInputVoltage_Y_Axis;
		g_liGyroAdd += g_iGyroInputVoltage_X_Axis;			
						
	} 	

	g_iAccelInputVoltage_Y_Axis = g_liAccAdd  / 20 ;
    g_iGyroInputVoltage_X_Axis  = g_liGyroAdd / 20 ;

	g_liAccAdd  = 0;	  /*�˲���ȫ�ֱ���Ҫ���㣬�´ε��òŲ������*/
    g_liGyroAdd = 0;

#else 	/*�����κ��˲�����*/
		g_iGyroInputVoltage_X_Axis   = DataSynthesis(GYRO_XOUT_H) ; //������X��
        g_iAccelInputVoltage_Y_Axis  = DataSynthesis(ACCEL_YOUT_H); //���ٶ�Y��
		

#endif	
}
void GetGyroOffset()
{
	long int aa;
	//int bb;
	int tt;
	for(tt=0;tt<1000;tt++)
	{
		aa+=DataSynthesis(GYRO_XOUT_H) ;
	}
	g_iGyroOffset = aa/1000;
	aa=0;
}
/***************************************************************
** ��������: SetMotorVoltageAndDirection
** ��������: ���ת�ټ�������ƺ���             
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void SetMotorVoltageAndDirection(float fLeftVoltage,float fRightVoltage)
{
	int  iLeftMotorValue;
	int  iRighttMotorValue;
	
	iLeftMotorValue = (int)	fLeftVoltage;
	iRighttMotorValue = (int) fRightVoltage;

	

    if(iLeftMotorValue<0)
    {	     
		iLeftMotorValue *= (-1);

		iLeftMotorValue = 20000 - iLeftMotorValue ;

		PWM2T1=20000;
		PWM3T1=iLeftMotorValue;	 
    }
    else 
    {	
		iLeftMotorValue = 20000 - iLeftMotorValue ;

		PWM2T1=iLeftMotorValue;
		PWM3T1=20000;
    }

    if(iRighttMotorValue<0)
    {	
		iRighttMotorValue *= (-1);

		iRighttMotorValue = 20000 - iRighttMotorValue;

		PWM5T1=20000;
		PWM4T1=iRighttMotorValue;
    }
    else
    {	
		iRighttMotorValue = 20000 - iRighttMotorValue;

		PWM5T1=iRighttMotorValue;
		PWM4T1=20000;
    }

   
#if IF_CAR_FALL		 /*�жϳ����Ƿ������������*/

	if(g_fCarAngle > 30 || g_fCarAngle < (-30))
	{
		PWM2T1=20000;
		PWM3T1=20000;
		PWM4T1=20000;
		PWM5T1=20000;
		PWM2T2=1;
		PWM3T2=1;
		PWM5T2=1;
		PWM4T2=1;
	}

#endif
}

/***************************************************************
** ��������: MotorOutput
** ��������: ����������
             ��ֱ�����ơ��ٶȿ��ơ�������Ƶ���������е���,����
			 �����������������������������
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void MotorOutput(void)
{

	g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection ;
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection ;
			
	
	/*������������*/
	if(g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if(g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if(g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if(g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*������ʹ�����ֹ����PWM��Χ*/	
	if(g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if(g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if(g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if(g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;

    SetMotorVoltageAndDirection(g_fLeftMotorOut,g_fRightMotorOut);
}

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
** ��������: SpeedControl
** ��������: �ٶȻ����ƺ���
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void SpeedControl(void)
{  
	
	g_fCarSpeed = (g_iLeftMotorPulseSigma  + g_iRightMotorPulseSigma ) * 0.5;
    g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;	  //ȫ�ֱ��� ע�⼰ʱ����

	/*��ͨ�˲�*/
    g_fCarSpeed = g_fCarSpeedOld * 0.2 + g_fCarSpeed * 0.8 ;
	g_fCarSpeedOld = g_fCarSpeed;	   														 

	//g_fCarSpeed *= CAR_SPEED_CONSTANT;	 //��λ��ת/��
	g_fCarPosition += g_fCarSpeed; 		 //·��  ���ٶȻ���
	g_fCarPosition += g_fBluetoothSpeed;

		/*������������*/			  
	if((int)g_fCarPosition > 20000)    g_fCarPosition = 20000;
	if((int)g_fCarPosition < (-20000))    g_fCarPosition = (-20000);
							
	g_fSpeedControlOut = (CAR_SPEED_SET - g_fCarSpeed) * g_fcSpeed_P + \
	(CAR_POSITION_SET - g_fCarPosition) * g_fcSpeed_I; 

}

/***************************************************************
** ��������: AngleControl
** ��������: �ǶȻ����ƺ���
** �䡡��:   
** �䡡��:   
** ȫ�ֱ���: 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
***************************************************************/
void AngleControl(void)	 
{  
	g_fGravityAngle = (float)(g_iAccelInputVoltage_Y_Axis - GRAVITY_OFFSET) / 16384.0f;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
	g_fGravityAngle = g_fGravityAngle * 57.2957795f ;// 180/3.1415926535898//����ת��Ϊ��,
	g_fGyroAngleSpeed = (g_iGyroInputVoltage_X_Axis  - GYRO_OFFSET) /10.0;// / 16.4;

	g_fCarAngle = 0.991*(g_fCarAngle + g_fGyroAngleSpeed * 0.008f) + 0.009*g_fGravityAngle;
	g_fAngleControlOut = (CAR_ANGLE_SET - g_fCarAngle)* g_fcAngle_P + \
	(CAR_ANGULARSPEED_SET - g_fGyroAngleSpeed )* g_fcAngle_D ;	   
	 
}

/***************************************************************
** ��������: BluetoothControl
** ��������: �������ƺ���
             �ֻ���������
			 �ϣ�00000010    �£�00000001
             ��00000011    �ң�00000100
             ֹͣ��00000000
             ���ܼ������Ա���λ��������չ����
             A:00000101      B:00000110
             C:00000111      D:00001000
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

	LED0=~LED0;

	ucBluetoothValue = UART2ReceiveByte();		
	switch (ucBluetoothValue)
	{
	  case 0x02 : g_fBluetoothSpeed =   100 ;  break;	   //ǰ��
	  case 0x01 : g_fBluetoothSpeed = (-100);  break;	   //����
	  case 0x03 : g_fBluetoothDirection =   5000 ;  break;//��ת
	  case 0x04 : g_fBluetoothDirection = (-5000);  break;//��ת
	  case 0x05 : g_fBluetoothSpeed =   130 ; break ;
	  case 0x06 : g_fBluetoothSpeed = (-130); break ;
	  case 0x07 : g_fBluetoothDirection =   15000 ;  break;
	  case 0x08 : g_fBluetoothDirection = (-15000);  break;
	  default : g_fBluetoothSpeed = 0; g_fBluetoothDirection = 0;g_iCarSpeedSet=0;break;
	}
	
}