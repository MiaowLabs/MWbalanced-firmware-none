/********************************************************************
作者：Songyimiao
建立日期: 20151129
版本：V2.0
喵呜实验室版权所有
/********************************************************************/
#include "includes.h"

unsigned int xdata g_uiStartCount;
unsigned char xdata g_ucLEDCount;
/******电机控制参数******/
int   g_iCarSpeedSet;
float g_fSpeedControlOut;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
/******角度控制参数******/
int   g_iAccelInputVoltage_X_Axis ;	//加速度X轴数据
int   g_iGyroInputVoltage_Y_Axis  ;	//陀螺仪Y轴数据
int   g_iGyroInputVoltage_Z_Axis  ;	//陀螺仪Y轴数据

float g_fCarAngle;         			//车模倾角
float g_fGyroAngleSpeed;			//角速度      			
float g_fGyroscopeAngleIntegral;	//角速度积分值
float g_fGravityAngle;				//加速度初步计算得到的倾角
int g_iGyroOffset;
/******速度控制参数******/
int   g_iLeftMotorPulse;
int   g_iRightMotorPulse;
int   g_iLeftMotorPulseSigma;
int   g_iRightMotorPulseSigma;
float g_fCarSpeed;
float g_fCarSpeedOld;
float g_fCarPosition;
unsigned char g_ucSpeedControlPeriod ;
unsigned char g_ucSpeedControlCount ;

/*-----角度环和速度环PID控制参数-----*/
float code g_fcAngle_P = 90.0;  //  85
float code g_fcAngle_D = 3.0;   // 2.8	
float code g_fcSpeed_P = 12.0 ; //  15
float code g_fcSpeed_I = 1.4;   // 1.7
float code g_fcDirection_P = 5.0;		   
/******蓝牙控制参数******/
float xdata g_fBluetoothSpeed;
float xdata g_fBluetoothDirection;

float xdata g_fDirectionDeviation;

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: DriversInit
** 功能描述: 底层驱动初始化            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
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
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: CarStandInit
** 功能描述: 直立参数初始化            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
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
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: DataSynthesis
** 功能描述: 数据合成函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
int DataSynthesis(unsigned char REG_Address)	
{
	char idata uiHighByte; /*高八位*/
	char idata ucLowByte; /*低八位*/

	uiHighByte = Single_ReadI2C(REG_Address)  ;
	ucLowByte  = Single_ReadI2C(REG_Address+1);

	return ((uiHighByte << 8) + ucLowByte);   /*返回合成数据*/
}

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: SampleInputVoltage
** 功能描述: MPU6050采样函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void SampleInputVoltage(void)
{	

	g_iGyroInputVoltage_Y_Axis   = DataSynthesis(GYRO_YOUT_H) ; //陀螺仪Y轴
    g_iAccelInputVoltage_X_Axis  = DataSynthesis(ACCEL_XOUT_H); //加速度X轴	
	
	g_iGyroInputVoltage_Z_Axis   = DataSynthesis(GYRO_ZOUT_H) ; //陀螺仪Y轴	

}

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: GyroRevise
** 功能描述: 陀螺仪校正函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
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
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: SetMotorVoltageAndDirection
** 功能描述: 电机设置函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void SetMotorVoltageAndDirection(int iLeftVoltage,int iRightVoltage)
{
	int iLeftMotorValue;
	int iRighttMotorValue;	

    if(iLeftVoltage<0)
    {	
      AIN1 = 1;				      //右电机前进	角度为负  速度为正
      AIN2 = 0;
      iLeftMotorValue = (-iLeftVoltage);
    }
    else 
    {	
      AIN1 = 0;				      //右电机后退  角度为正  速度为负
      AIN2 = 1; 
      iLeftMotorValue = iLeftVoltage;
    }

    if(iRightVoltage<0)
    {	
      BIN1 = 1;				      //左电机前进  角度为负  速度为正
      BIN2 = 0;
      iRighttMotorValue = (-iRightVoltage);
    }
    else
    {	
      BIN1 = 0;				      //左电机后退  角度为正  速度为负
      BIN2 = 1; 
      iRighttMotorValue = iRightVoltage;
    }

	iLeftMotorValue   = (1000 - iLeftMotorValue)  ;	   
	iRighttMotorValue = (1000 - iRighttMotorValue);
	
  	PWM3T1 = iLeftMotorValue  ;  
   	PWM4T1 = iRighttMotorValue;  

	#if IF_CAR_FALL		 /*判断车辆是否跌倒，调试用*/

	if(g_fCarAngle > 30 || g_fCarAngle < (-30))
	{
		AIN1 = 0;				      //右电机前进	角度为负  速度为正
      	AIN2 = 0; 
		BIN1 = 0;				      //右电机前进	角度为负  速度为正
      	BIN2 = 0;   
	}

#endif

}
/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: MotorOutput
** 功能描述: 电机输出函数            
** 输　  入:   
** 输　  出:   
** 备    注: 将直立控制、速度控制、方向控制的输出量进行叠加,并加
			 入死区常量，对输出饱和作出处理。
********************喵呜实验室版权所有**************************
***************************************************************/
void MotorOutput(void)
{

	g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection + g_fDirectionDeviation;
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection - g_fDirectionDeviation;			
	
	/*增加死区常数*/
	if(g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if(g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if(g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if(g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*输出饱和处理是保证输出量不会超出PWM的满量程范围。*/	
	if(g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if(g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if(g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if(g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;

    SetMotorVoltageAndDirection(g_fLeftMotorOut,g_fRightMotorOut);
}


/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: GetMotorPulse
** 功能描述: 捕获电机脉冲函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
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
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: SpeedControl
** 功能描述: 速度环控制函数           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void SpeedControl(void)
{  
	float fP, fDelta;
	float fI;
	
	g_fCarSpeed = (float)(g_iLeftMotorPulseSigma  + g_iRightMotorPulseSigma ) * 0.5f;
    g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;	  //全局变量 注意及时清零

	/*低通滤波*/ 
    g_fCarSpeed = (float)(g_fCarSpeedOld * 0.2f + g_fCarSpeed * 0.8f) ;
	g_fCarSpeedOld = g_fCarSpeed;
		   														 
	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;
	fP = fDelta * g_fcSpeed_P;
	fI = fDelta * g_fcSpeed_I;
	g_fCarPosition += fI;

	/*积分上限设限*/			  
	if((int)g_fCarPosition > SPEED_CONTROL_OUT_MAX)    g_fCarPosition = SPEED_CONTROL_OUT_MAX;
	if((int)g_fCarPosition < SPEED_CONTROL_OUT_MIN)    g_fCarPosition = SPEED_CONTROL_OUT_MIN;
	g_fCarPosition += g_fBluetoothSpeed;

	g_fSpeedControlOut = fP + g_fCarPosition;

}

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: AngleControl
** 功能描述: 角度环控制函数           
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void AngleControl(void)	 
{  
	//去除零点偏移,计算得到加速度传感器的角度（弧度）
	g_fGravityAngle = (float)(g_iAccelInputVoltage_X_Axis - GRAVITY_OFFSET) / 16384.0f;
	// 57.2957795=180/3.1415926535898 弧度转换为度
	g_fGravityAngle = g_fGravityAngle * 57.2957795f ;

	g_fGyroAngleSpeed = (g_iGyroInputVoltage_Y_Axis - GYRO_OFFSET) / GYROSCOPE_ANGLE_RATIO *(-1);
	//互补滤波
	g_fCarAngle = 0.99f*(g_fCarAngle + g_fGyroAngleSpeed * 0.008f) + 0.01f * g_fGravityAngle;
	//角度环PID控制
	g_fAngleControlOut = (CAR_ANGLE_SET - g_fCarAngle)* g_fcAngle_P + \
	(CAR_ANGULARSPEED_SET - g_fGyroAngleSpeed )* g_fcAngle_D ;	   
	 
}

/***************************************************************
** 函数名称: BluetoothControl
** 功能描述: 蓝牙控制函数
             手机发送内容
			 前进：0x01    后退：0x02
             左：  0x03    右：  0x04
             停止：0x10
             功能键（可自编下位机程序扩展）：
             左自旋：0x07
			 右自旋：0x08
			 更快地前进：0x09  更慢地前进：0x0A
			 更慢地后退：0x0B  更慢地后退：0x0C   巡线启动：0x0D
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
void BluetoothControl(void)	 
{
	unsigned char xdata ucBluetoothValue;

	//LED0=~LED0;

	ucBluetoothValue = UART2ReceiveByte();		
		
	switch (ucBluetoothValue)
	{

	  case 0x02 : g_fBluetoothSpeed =   40 ;  break;//后退
	  case 0x01 : g_fBluetoothSpeed = (-40);  break;//前进
	  case 0x09 : g_fBluetoothSpeed =   60;   break;
	  case 0x0A : g_fBluetoothSpeed = (-60);  break;
	  case 0x0B : g_fBluetoothSpeed =   20;  break;
	  case 0x0C : g_fBluetoothSpeed = (-20);  break;
	  case 0x03 : g_fBluetoothDirection =   200 ;  break;//左转
	  case 0x04 : g_fBluetoothDirection = (-200);  break;//右转
	  case 0x05 : g_iCarSpeedSet =   20 ; break ;
	  case 0x06 : g_iCarSpeedSet = (-20); break ;
	  case 0x07 : g_fBluetoothDirection =   400 ;  break;
	  case 0x08 : g_fBluetoothDirection = (-400);  break;
	  case 0x0D : g_iCarSpeedSet = 8;  break;	   //前进
	  default : g_fBluetoothSpeed = 0; g_fBluetoothDirection = 0;g_iCarSpeedSet=0;break;
	}
}

void EliminateDirectionDeviation(void)
{
	int Delta=0;

	Delta = g_iLeftMotorPulseSigma - g_iRightMotorPulseSigma;

	g_fDirectionDeviation = Delta * g_fcDirection_P * (-1);


}