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
int   g_iAccelInputVoltage_Y_Axis ;	//加速度Y轴数据
int   g_iGyroInputVoltage_X_Axis  ;	//陀螺仪X轴数据
long int  g_liAccSum;
long int  g_liGyroSum;
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
float code g_fcAngle_P = 1000.0;//1300
float code g_fcAngle_D = 25.0;//25	
float code g_fcSpeed_P = 200.0 ; //200
float code g_fcSpeed_I = 0.08;		   
/******蓝牙控制参数******/
float xdata g_fBluetoothSpeed;
float xdata g_fBluetoothDirection;

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
	g_iAccelInputVoltage_Y_Axis = g_iGyroInputVoltage_X_Axis = 0;
	g_iLeftMotorPulse = g_iRightMotorPulse = 0;

	g_iCarSpeedSet=0;
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
#if 0 
    /*
	N次均值滤波，此处N取20。
	会降低频响，N值要适当，不宜过大过小。
	*/	
	unsigned char ucValue;   
	for(ucValue = 0 ; ucValue < 20 ; ucValue ++)
	{
		g_iAccelInputVoltage_Y_Axis  = DataSynthesis(ACCEL_YOUT_H);//加速度Y轴
		g_iGyroInputVoltage_X_Axis = DataSynthesis(GYRO_XOUT_H);//陀螺仪X轴
	
		g_liAccSum += g_iAccelInputVoltage_Y_Axis;
		g_liGyroSum += g_iGyroInputVoltage_X_Axis;			
						
	} 	

	g_iAccelInputVoltage_Y_Axis = g_liAccSum  / 20 ;
    g_iGyroInputVoltage_X_Axis  = g_liGyroSum / 20 ;

	g_liAccSum  = 0;	  /*滤波完全局变量要清零，下次调用才不会出错*/
    g_liGyroSum = 0;

#else 	/*不作任何滤波处理*/

		g_iGyroInputVoltage_X_Axis   = DataSynthesis(GYRO_XOUT_H) ; //陀螺仪X轴
        g_iAccelInputVoltage_Y_Axis  = DataSynthesis(ACCEL_YOUT_H); //加速度Y轴
		

#endif	
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
void GyroRevise()
{
	long int tempsum;
	int temp;
	for(temp=0;temp<200;temp++)
	{
		tempsum += DataSynthesis(GYRO_XOUT_H) ;
	}
	g_iGyroOffset = tempsum/200;
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
void SetMotorVoltageAndDirection(float fLeftVoltage,float fRightVoltage)
{
	int  iLeftMotorValue;
	int  iRighttMotorValue;
	
#if IF_CAR_FALL		 /*判断车辆是否跌倒*/

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
	iLeftMotorValue = (int)	fLeftVoltage;
	iRighttMotorValue = (int) fRightVoltage;

    if(iLeftMotorValue<0)
    {	     
		iLeftMotorValue *= (-1);

		iLeftMotorValue = 20000 - iLeftMotorValue ;

		PWM3T1=20000;
		PWM2T1=iLeftMotorValue;	 
    }
    else 
    {	
		iLeftMotorValue = 20000 - iLeftMotorValue ;

		PWM3T1=iLeftMotorValue;
		PWM2T1=20000;
    }

    if(iRighttMotorValue<0)
    {	
		iRighttMotorValue *= (-1);

		iRighttMotorValue = 20000 - iRighttMotorValue;

		PWM4T1=20000;
		PWM5T1=iRighttMotorValue;
    }
    else
    {	
		iRighttMotorValue = 20000 - iRighttMotorValue;

		PWM4T1=iRighttMotorValue;
		PWM5T1=20000;
    }

   

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

	g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection ;
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection ;
			
	
	/*增加死区常数*/
	if(g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if(g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if(g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if(g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*输出饱和处理，防止超出PWM范围*/	
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
	
	g_fCarSpeed = (g_iLeftMotorPulseSigma  + g_iRightMotorPulseSigma ) * 0.5;
    g_iLeftMotorPulseSigma = g_iRightMotorPulseSigma = 0;	  //全局变量 注意及时清零

	/*低通滤波*/
    g_fCarSpeed = g_fCarSpeedOld * 0.2 + g_fCarSpeed * 0.8 ;
	g_fCarSpeedOld = g_fCarSpeed;	   														 

	//g_fCarSpeed *= CAR_SPEED_CONSTANT;	 //单位：转/秒
	g_fCarPosition += g_fCarSpeed; 		 //路程  即速度积分
	//g_fCarPosition += g_fBluetoothSpeed;

		/*积分上限设限*/			  
	if((int)g_fCarPosition > SPEED_CONTROL_OUT_MAX)    g_fCarPosition = SPEED_CONTROL_OUT_MAX;
	if((int)g_fCarPosition < SPEED_CONTROL_OUT_MIN)    g_fCarPosition = SPEED_CONTROL_OUT_MIN;
							
	g_fSpeedControlOut = (CAR_SPEED_SET - g_fCarSpeed) * g_fcSpeed_P + \
	(CAR_POSITION_SET - g_fCarPosition) * g_fcSpeed_I; 

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
	g_fGravityAngle = (float)(g_iAccelInputVoltage_Y_Axis - GRAVITY_OFFSET) / 16384.0f;//去除零点偏移,计算得到角度（弧度）
	g_fGravityAngle = g_fGravityAngle * 57.2957795f ;// 180/3.1415926535898 弧度转换为度,
	g_fGyroAngleSpeed = (g_iGyroInputVoltage_X_Axis - GYRO_OFFSET) / GYROSCOPE_ANGLE_RATIO ;// 16.4;
	
	g_fCarAngle = 0.99*(g_fCarAngle + g_fGyroAngleSpeed * 0.008f) + 0.01*g_fGravityAngle;//互补滤波

	g_fAngleControlOut = (CAR_ANGLE_SET - g_fCarAngle)* g_fcAngle_P + \
	(CAR_ANGULARSPEED_SET - g_fGyroAngleSpeed )* g_fcAngle_D ;	   
	 
}

/***************************************************************
** 函数名称: BluetoothControl
** 功能描述: 蓝牙控制函数
             手机发送内容
			 上：00000010    下：00000001
             左：00000011    右：00000100
             停止：00000000
             功能键（可自编下位机程序扩展）：
             A:00000101      B:00000110
             C:00000111      D:00001000
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

	LED0=~LED0;

	ucBluetoothValue = UART2ReceiveByte();		
	switch (ucBluetoothValue)
	{
	  case 0x02 : g_iCarSpeedSet =   1500 ;  break;	   //前进
	  case 0x01 : g_iCarSpeedSet = (-1500);  break;	   //后退
	  case 0x03 : g_fBluetoothDirection =   5000 ;  break;//左转
	  case 0x04 : g_fBluetoothDirection = (-5000);  break;//右转
	  case 0x05 : g_fBluetoothSpeed =   130 ; break ;
	  case 0x06 : g_fBluetoothSpeed = (-130); break ;
	  case 0x07 : g_fBluetoothDirection =   15000 ;  break;
	  case 0x08 : g_fBluetoothDirection = (-15000);  break;
	  default : g_fBluetoothSpeed = 0; g_fBluetoothDirection = 0;g_iCarSpeedSet=0;break;
	}
	
}