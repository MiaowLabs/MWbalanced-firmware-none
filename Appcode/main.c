/********************************************************************
作者：Songyimiao
建立日期: 20151129
版本：V2.0
喵呜实验室版权所有
/********************************************************************/
#include "includes.h"

/***************************************************************
** 作　  者: Songyimiao
** 官    网：http://www.miaowlabs.com
** 淘    宝：http://miaowlabs.taobao.com
** 日　  期: 2015年11月29日
** 函数名称: main()
** 功能描述: 主函数            
** 输　  入:   
** 输　  出:   
** 备    注: 
********************喵呜实验室版权所有**************************
***************************************************************/
void main()
{
	DisableInterrupts;//禁止总中断

	DriversInit();	  //MCU底层驱动初始化
	MPU6050Init();	  //MPU6050配置初始化
	CarStandInit();	  //应用程序变量初始化

#if ULTRA_EN //超声波和舵机驱动公用 定时器0，因此不能同时使用两个模块
	UltraInit();	  //超声波配置初始化
#else
	ServoInit();	  //舵机驱动初始化
#endif

	Delaynms(50);	  //延时50ms 
	ON_LED0;		  //指示灯亮起，表示初始化完毕
	
	EnableInterrupts; //使能总中断	 

	while(1)
	{ 					
	
		STC_ISP();
		//UART1SendByte(0x22);
		Delaynms(1);	        //延时5ms 
		if(g_ucUart2Flag>=1)
		{
			setValues(g_cUart2Buffer);	//蓝牙遥控函数
			g_ucUart2Flag = 0;
		//	UART1SendByte(0x11);
		}
		BatteryChecker();		//电池电量检测（电量不足时，亮红灯）
		if(EchoFlag)			//判断是否测距完成
		{ 
			g_ucUltraDis=UltraDis();//读取超声波测距结果
		}
		 
#if DEBUG_UART  //调试启用 预编译命令 
//若要观察波形进行调试，需将DEBUG_UART设置成1，该版本不需要注释蓝牙控制函数
	
   		OutData[0] = g_fCarAngle;
   		OutData[1] = g_fGyroAngleSpeed;
   		OutData[2] = g_fGravityAngle;
   		OutData[3] = g_fAngleControlOut;
   		OutPut_Data();		
		 	  
#endif	 		
					
	}
}
