/********************************************************************
���ߣ�Songyimiao
��������: 20151129
�汾��V2.0
����ʵ���Ұ�Ȩ����
/********************************************************************/
#ifndef _CARSTAND_H_
#define _CARSTAND_H_

/******�Ƕȿ�����غ궨��******/
#define	CAR_ANGLE_SET	      0					//�Ƕ�Ԥ��ֵ
#define CAR_ANGULARSPEED_SET  0					//���ٶ�Ԥ��ֵ
#define GRAVITY_OFFSET   (2000)      			//���ٶ����ƫ��ֵ 
#define GYRO_OFFSET      0      				//���������ƫ��ֵ 
#define	GYROSCOPE_ANGLE_RATIO 30.5		        //�����Ǳ�������
/******�ٶȿ�����غ궨��******/
#define CAR_POSITION_SET  0						//·��Ԥ��ֵ
#define CAR_SPEED_SET     g_iCarSpeedSet	    //�ٶ�Ԥ��ֵ
#define MOTOR_LEFT_SPEED_POSITIVE  ((int)g_fLeftMotorOut >0)   //�����ٶȷ����ж�
#define MOTOR_RIGHT_SPEED_POSITIVE ((int)g_fRightMotorOut>0)   //�����ٶȷ����ж�
#define SPEED_CONTROL_OUT_MAX	MOTOR_OUT_MAX 	
#define SPEED_CONTROL_OUT_MIN	MOTOR_OUT_MIN

/******���������غ궨��******/
#define MOTOR_OUT_DEAD_VAL       0	   //����ֵ
#define MOTOR_OUT_MAX           1000   //ռ�ձ������ֵ
#define MOTOR_OUT_MIN         (-1000)  //ռ�ձȸ����ֵ		   

extern unsigned char xdata g_ucIRFlag;
extern unsigned int xdata g_uiStartCount;
extern unsigned char xdata g_ucLEDCount;
extern int g_iLeftMotorPulse;
extern int g_iRightMotorPulse;
extern float g_fCarAngle;
extern float g_fGravityAngle;
extern float g_fGyroAngleSpeed ;
extern int g_iAccelInputVoltage_X_Axis;
extern int g_iGyroInputVoltage_Y_Axis ;
extern unsigned char g_ucSpeedControlCount ;
extern float g_fSpeedControlOut;
extern float g_fCarSpeed;
extern float g_fCarPosition;
extern float g_fGyroscopeAngleIntegral;
extern int g_iGyroOffset;
extern int g_iLeftMotorPulseSigma  ;
extern float g_fAngleControlOut;
extern float g_fLeftMotorOut;
extern float g_fPower;
extern unsigned char xdata g_ucRxd2;
extern unsigned char xdata g_ucUart2Flag;

void DirectionControl(void);
void EliminateDirectionDeviation(void);
void DriversInit(void);
void SampleInputVoltage(void);
void CarStandInit(void);
void SpeedControl(void);
void AngleControl(void);
void BluetoothControl(void);	
void GetMotorPulse(void);
void GetGyroRevise(void);
void MotorOutput(void);
void BatteryChecker();

#endif