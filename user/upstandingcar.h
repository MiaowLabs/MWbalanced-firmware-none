
#ifndef _UPSTANDINGCAR_H_
#define _UPSTANDINGCAR_H_

/******Target CPU相关宏定义******/
#define CLK_DIV_1() CLK_DIV &= 0x38 /*CPU工作频率等于内部R/C振荡时钟*/
/*
	  CLKS2 CLKS1 CLKS0         分频后CPU实际工作时钟
	    0     0     0        外部晶体时钟或内部R/C振荡时钟
	    0     0     1       (外部晶体时钟或内部R/C振荡时钟)/2
	    0     1     0       (外部晶体时钟或内部R/C振荡时钟)/4
	    0     1     1       (外部晶体时钟或内部R/C振荡时钟)/8
	    1     0     0       (外部晶体时钟或内部R/C振荡时钟)/16
        1     0     1       (外部晶体时钟或内部R/C振荡时钟)/32
        1     1     0       (外部晶体时钟或内部R/C振荡时钟)/64
        1     1     1       (外部晶体时钟或内部R/C振荡时钟)/128	
*/	

#define DisableInterrupts	EA=0
#define EnableInterrupts 	EA=1


/******角度控制相关宏定义******/
#define     CAR_ANGLE_SET	      0
#define     CAR_ANGULARSPEED_SET  0
#define     GRAVITY_OFFSET   (4000)//(2800)//(3100)       //加速度零点偏移值 
#define     GYRO_OFFSET      g_iGyroOffset//(265) //8          //陀螺仪零点偏移值
#define     GRAVITY_X_MIN        (-16880)
#define     GRAVITY_X_MAX         16070     
#define     CAR_ANGLE_RANGE	  	 180.0
#define     GRAVITY_ANGLE_RATIO	 (CAR_ANGLE_RANGE / ((float)GRAVITY_X_MAX - (float)GRAVITY_X_MIN))//0.005493f//  )
#define     GYROSCOPE_ANGLE_RATIO     0.135f//0.09	
#define 	GYROSCOPE_ANGLE_SIGMA_FREQUENCY 125	 //1/0.008=125
#define 	GRAVITY_ADJUST_TIME_CONSTANT 1 //0.6
/******速度控制相关宏定义******/
#define CAR_POSITION_SET      0
#define CAR_SPEED_SET         0
#define MOTOR_LEFT_SPEED_POSITIVE  ((int)g_fLeftMotorOut >0)
#define MOTOR_RIGHT_SPEED_POSITIVE ((int)g_fRightMotorOut>0)
#define OPTICAL_ENCODE_CONSTANT  (16*30)	//光电码盘刻度槽
//#define SPEED_CONTROL_PERIOD	 8.0F	    //速度环控制周期
#define CAR_SPEED_CONSTANT		0.0260416f//0.0260416f//(1000.0/SPEED_CONTROL_PERIOD/(float)OPTICAL_ENCODE_CONSTANT)
#define SPEED_CONTROL_OUT_MAX	(MOTOR_OUT_MAX * 2) 
#define SPEED_CONTROL_OUT_MIN	(MOTOR_OUT_MIN * 2) //40000
/******电机控制相关宏定义******/
#define MOTOR_OUT_DEAD_VAL       0//200 	   //死区值200
#define MOTOR_OUT_MAX           20000	   //占空比正最大值
#define MOTOR_OUT_MIN         (-20000)   //占空比负最大值
/******调试相关宏定义******/
#define IF_CAR_FALL	1		//判断车模是否跌倒		   


#define SPEED_CONTROL_PERIOD 80//0//ms

extern unsigned int flag_ok;
extern unsigned char xdata g_ucLEDCount;
extern unsigned char g_ucSpeedCountCarry;
extern int g_iLeftMotorPulse;
extern int g_iRightMotorPulse;
extern float g_fCarAngle;
extern float g_fGravityAngle;
extern float g_fGyroAngleSpeed ;
extern int g_iAccelInputVoltage_Y_Axis;
extern int g_iGyroInputVoltage_X_Axis ;
extern unsigned char xdata g_uc2msEventCount;
extern unsigned char g_ucSpeedControlPeriod ;
extern unsigned char g_ucSpeedControlCount ;
extern float g_fSpeedControlOutOld;
extern float g_fSpeedControlOutNew ;
extern float g_fSpeedControlOut;
extern float g_fCarSpeed;
extern float g_fCarPosition;
extern float g_fGyroscopeAngleIntegral;
extern int g_iGyroOffset;
extern int g_iLeftMotorPulseSigma  ;

void EXTINTInit(void);
int  DataSynthesis(unsigned char REG_Address);
void SampleInputVoltage(void);
void CarUpstandInit(void);
void AngleCalculate(void);
void SetMotorVoltageAndDirection(float fLeftVoltage,float fRightVoltage);
void MotorOutput(void);
void SpeedControl(void);
void AngleControl(void);
void BluetoothControl(void);	
void GetMotorPulse(void);
void SpeedControlOutput(void) ;
void GetGyroOffset();

#endif