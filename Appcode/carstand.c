/********************************************************************
���ߣ�Songyimiao
��������: 20151129
�汾��V2.0
����ʵ���Ұ�Ȩ����
/********************************************************************/
#include "includes.h"


unsigned int xdata g_uiStartCount;
unsigned char xdata g_ucLEDCount;
unsigned char xdata g_ucIRFlag = 0;

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
float g_fDeltaOld;
float g_fCarSpeed;
float g_fCarSpeedOld;
float g_fCarPosition;
unsigned char g_ucSpeedControlPeriod ;
unsigned char g_ucSpeedControlCount ;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
float code g_fcAngle_P = 90.0;  	 //�ǶȻ�P����
float code g_fcAngle_D = 3.0;   	 //�ǶȻ�D����
float code g_fcSpeed_P = 15.0 ; 	 //�ٶȻ�P����	15
float code g_fcSpeed_I = 1.4;   	 //�ٶȻ�I����	1.4
//float code g_fcSpeed_D = 0.0;		 //�ٶȻ�D����
float code g_fcDirection_P = 300.0;	 //����P����
float code g_fcEliminate_P= 0.0;	 //�̾����������P����  
/******�������Ʋ���******/
float xdata g_fBluetoothSpeed;
float xdata g_fBluetoothDirection;

float xdata g_fDirectionDeviation;
float xdata g_fDirectionControlOut;

float xdata g_fPower;

unsigned char xdata g_ucRxd2;
unsigned char xdata g_ucUart2Flag=0;
char xdata g_cUart2Buffer[UART2_DATA];
unsigned char xdata g_ucUart2MoveIndex;
unsigned char g_ucUart2Count=0;	

unsigned char xdata g_ucUltraDis;
float xdata g_fUltraSpeed;
float sppData1,sppData2;

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
	ADCInit();
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

	g_fDirectionDeviation = g_fDirectionControlOut = 0;

	g_fPower = 0;
	g_fDeltaOld = 0;
	g_ucRxd2 = g_ucUart2Flag = 0;
	g_fUltraSpeed = 0;


	UART2SendStr("AT+NAMEMWBalanced\r\n");//���������豸����Ϊ MWBalanced
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

	if((int)g_fCarAngle > 25 || (int)g_fCarAngle < (-25))
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

	g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection + g_fDirectionControlOut;
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection - g_fDirectionControlOut;			
	
	/*������������*/
	if(g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if(g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if(g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if(g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*������ʹ����Ǳ�֤��������ᳬ��PWM�������̷�Χ*/	
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
	float fDelta;//,fDeriv;
	float fP, fI;//, fD;
	
	g_fCarSpeed = (float)(g_iLeftMotorPulseSigma  +  g_iRightMotorPulseSigma ) * 0.5f;
    g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;	  //ȫ�ֱ��� ע�⼰ʱ����

	/*��ͨ�˲�*/
    g_fCarSpeed = (float)(g_fCarSpeedOld * 0.2f + g_fCarSpeed * 0.8f) ;
	g_fCarSpeedOld = g_fCarSpeed;
	   														 
	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;
//	fDeriv = fDelta - g_fDeltaOld;
	fP = fDelta * g_fcSpeed_P;
	fI = fDelta * g_fcSpeed_I;
	g_fCarPosition += fI;
//	fD = fDeriv * g_fcSpeed_D;
//	g_fDeltaOld = fDelta;
	
	g_fCarPosition += g_fUltraSpeed;

	/*������������*/			  
	if((int)g_fCarPosition > SPEED_CONTROL_OUT_MAX)    g_fCarPosition = SPEED_CONTROL_OUT_MAX;
	if((int)g_fCarPosition < SPEED_CONTROL_OUT_MIN)    g_fCarPosition = SPEED_CONTROL_OUT_MIN;
		
	g_fCarPosition += g_fBluetoothSpeed;

	g_fSpeedControlOut = fP + g_fCarPosition;//+ fD;

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

	g_fGyroAngleSpeed = (g_iGyroInputVoltage_Y_Axis - GYRO_OFFSET) / GYROSCOPE_ANGLE_RATIO *(-1.0);
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
	unsigned char xdata ucBluetoothValue = 0;

	ucBluetoothValue = g_ucRxd2;		
		
	switch(ucBluetoothValue)
	{

	  case 0x02 : g_fBluetoothSpeed =   50 ;  break;//����
	  case 0x01 : g_fBluetoothSpeed = (-50);  break;//ǰ��
//	  case 0x02 : g_iCarSpeedSet =   30 ;  break;//����
//	  case 0x01 : g_iCarSpeedSet = (-30);  break;//ǰ��	  
	  case 0x03 : g_fBluetoothDirection =   200 ;  break;//��ת
	  case 0x04 : g_fBluetoothDirection = (-200);  break;//��ת
	  case 0x05 : 
	  {	//�����ת
	   	ServoCtr(0x19, 0);
		ServoCtr(0x0d, 1); 
	  }break ;
	  case 0x06 :
	  {//�����ת	
	  	ServoCtr(0x0d, 0);
		ServoCtr(0x19, 1);
	  }break ;
	  case 0x07 : g_fBluetoothDirection =   400 ;  break;
	  case 0x08 : g_fBluetoothDirection = (-400);  break;
	  case 0x0D : g_iCarSpeedSet = 8; g_ucIRFlag = 1; break;	   //Ѳ������
	  default :
	  {
	   g_fBluetoothSpeed = 0; 
	   g_fBluetoothDirection = 0;
	   g_iCarSpeedSet=0;
	   ServoCtr(0x13, 0);
	   ServoCtr(0x13, 1);
	   g_ucIRFlag = 0;
	  }break;
	}
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 
** ��������: EliminateDirectionDeviation
** ��������: �̾���ֱ�߾������ƺ���           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/

void EliminateDirectionDeviation(void)
{
	int Delta = 0;

	Delta = g_iLeftMotorPulseSigma - g_iRightMotorPulseSigma;

	g_fDirectionDeviation = Delta * g_fcEliminate_P * (-1);


}


/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 20160408
** ��������: DirectionControl
** ��������: ����ѭ��������ƺ���           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void DirectionControl(void)
{ 
  int iLeft,iRight;
  
  iLeft = LeftIR;
  iRight = RightIR;

  if(iLeft==0&&iRight==1)
  {
  	g_fDirectionControlOut = g_fcDirection_P;	
  }
  else if(iLeft==1&&iRight==0)
  {
  	g_fDirectionControlOut = (-1) * g_fcDirection_P;	
  }
  else if(iLeft==0&&iRight==0)
  {
  	g_fDirectionControlOut = 0;	
  }
  else if(iLeft==1&&iRight==1)
  {
  	g_fDirectionControlOut = 0;	
  }
}

/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 20160415
** ��������: BatteryChecker
** ��������: ������⣨���������㣬�������ƣ�           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void BatteryChecker()
{

	g_fPower = GetADCResult();	 				//max8.4*510/(1000+510)/3.3*256=220
	g_fPower = g_fPower / 220* 8400;	 		 
	if((int)g_fPower <= 7499)						//low7.4*510/(1000+510)=2.499v
	{
		ON_LED1;
	}

}


/***************************************************************
** ����  ��: Songyimiao
** ��    ����http://www.miaowlabs.com
** ��    ����http://miaowlabs.taobao.com
** �ա�  ��: 20160415
** ��������: BatteryChecker
** ��������: ����������           
** �䡡  ��:   
** �䡡  ��:   
** ��    ע: 
********************����ʵ���Ұ�Ȩ����**************************
***************************************************************/
void UltraFollow(void)
{
	if((g_ucUltraDis>=0)&&(g_ucUltraDis<=10))
	{//����С��10cm����ˣ������ٶ������ɷ��ȣ��Һ�������ٶ�40����СΪ20
		g_fUltraSpeed = -(10-g_ucUltraDis)*10;
		if(g_fUltraSpeed > -20)g_fUltraSpeed = -20; 
		else if(g_fUltraSpeed < -40)g_fUltraSpeed = -40;
	}
	else if((g_ucUltraDis>20)&&(g_ucUltraDis<=30))	
	{//�������20cm��С��30��ǰ����ǰ���ٶ����������ȣ���ǰ������ٶ�40����СΪ20
		g_fUltraSpeed =  (g_ucUltraDis-20)*10;
		if(g_fUltraSpeed < 20)g_fUltraSpeed = 20; 
		else if(g_fUltraSpeed > 40)g_fUltraSpeed = 40;
	}
	else
		g_fUltraSpeed = 0;
}

float scale(float input, float inputMin, float inputMax, float outputMin, float outputMax) { 
  float output;
  if (inputMin < inputMax)
    output = (input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin));
  else
    output = (inputMin - input) / ((inputMin - inputMax) / (outputMax - outputMin));
  if (output > outputMax)
    output = outputMax;
  else if (output < outputMin)
    output = outputMin;
  return output;
}

void steer(enum Command command){
	if(command == stop){
		g_fBluetoothSpeed = 0;
		g_fBluetoothDirection = 0;
	}
	else if(command == joystick){
		if(sppData2>0)//ǰ��
		g_fBluetoothSpeed = scale(sppData2, 0, 1, 0, 50);
		else if(sppData2<0)//����
		g_fBluetoothSpeed = -scale(sppData2, 0, -1, 0, 50);
		if (sppData1 > 0) // ��ת
      	g_fBluetoothDirection = scale(sppData1, 0, 1, 0, 400);
    	else if (sppData1 < 0) // ��ת
      	g_fBluetoothDirection = -scale(sppData1, 0, -1, 0, 400);						
	}else if(command == imu){
		if(sppData1>0)//ǰ��
		g_fBluetoothSpeed = scale(sppData1, 0, 20, 0, 50);
		else if(sppData1<0)//����
		g_fBluetoothSpeed = -scale(sppData1, 0, -20, 0, 50);
		if (sppData2 > 0) // ��ת
      	g_fBluetoothDirection = scale(sppData2, 0, 40, 0, 400);
    	else if (sppData2 < 0) // ��ת
      	g_fBluetoothDirection = -scale(sppData2, 0, -40, 0, 400);
		}
}


void setValues(char *dataInput)
{
	char i;
	if(g_ucUart2Flag == 0) return;
 	for(i=0;i<g_ucUart2Count;i++)
	{
		UART1SendByte(dataInput[i]);
	}  			 		   			
	if(dataInput[0] == 'C'){		   
			if(dataInput[1] == 'J'){
				strtok(dataInput,",");
				sppData1 = atof(strtok(NULL,","));//X������
				sppData2 = atof(strtok(NULL,";"));//Y������
				steer(joystick);
			  	
			}
			else if(dataInput[1] == 'M') {
				strtok(dataInput, ","); 
      			sppData1 = atof(strtok(NULL, ",")); // Pitch
      			sppData2 = atof(strtok(NULL, ";")); // Roll
      			steer(imu);

			}
			else if(dataInput[1] == 'S'){
				steer(stop);
			}
	}

	g_ucUart2Flag = 0;
	g_ucUart2Count =0;

}