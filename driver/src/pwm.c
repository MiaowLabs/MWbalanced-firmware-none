
#include "PWM.h"

/***************************************************************
** 函数名称: PWMInit
** 功能描述: PWM初始化函数
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
void PWMInit()
{	

	P_SW2|=0x80;    //使能访问XSFR

	PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
	PWMCKS = 0x00;                  //选择PWM的时钟为Fosc/(0+1)=20M
	PWMIF=0x00; 
	PWMFDCR=0x00; 

	PWMC = 20001;                   //单位：脉冲时钟 设置PWM周期(最大值为32767)	PWM频率=20M/10000=2k

	PWM2T1 = 20000;
	PWM2T2 = 20001;

	PWM3T1 = 20000;                 //设置PWM3第1次反转的PWM计数
    PWM3T2 = 20001;    			 	//设置PWM3第2次反转的PWM计数
									//占空比为(PWM3T2-PWM3T1)/PWMC
									//初始PWM3占空比为100%

	PWM4T1 = 20000;                 //设置PWM4第1次反转的PWM计数
    PWM4T2 = 20001;    				//设置PWM4第2次反转的PWM计数
									//占空比为(PWM4T2-PWM4T1)/PWMC
									//初始PWM4占空比为100%
	PWM5T1 = 20000;
	PWM5T2 = 20001;

	PWM2CR=0x00; 
	PWM3CR=0x00;
	PWM4CR=0x00; 
	PWM5CR=0x00; 

    PWMCR = 0x8f;                   //使能PWM信号
   
	//P_SW2 &= ~0x80;
 }

void PCAInit()
{
	CMOD = 0x04;//选择定时器0的溢出脉冲作为计数源
	AUXR1 |= 0x20;	//切换引脚
	PCA_PWM0 = 0x00 ;//8位PWM,无中断
	PCA_PWM1= 0x00;
	CL = 0;         //自由递增计数的16位PCA定时器的值,初始化为0
	CH = 0;
	CCAP0H = CCAP0L = 0x00;	//用于控制占空比，占空比=(255-CCAPnL)/255*100%
  	CCAP1H = CCAP1L = 0x00; //初始化占空比为50%
	CR = 1;	//PCA计数器阵列开始										 
	 //PWM的频率=PCA时钟输入源频率/256
}


