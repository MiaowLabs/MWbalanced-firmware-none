#include "includes.h"


sbit Trig  = P3^6;
sbit Echo  = P3^2;

unsigned short  xdata T0Temp=0;
bit EchoFlag=0;

/*

* 模块说明 *

* 该模块使用单片机 定时器0 和 外部中断0，因此其他模块不可重复使用

* 使用方法: 1、 调用UltraInit()进行初始化
		    2、 调用UltraTrig()触发一次测距动作。两次调用时间间隔60毫秒以上，
		        防止发射信号对上次回响影响
		    3、 调用UltraDis()读取测距结果，结果为一字节十六进制数，单位为厘米。
		   	    返回0xff则测距未完成或失败。

* 为保证读取数据成功，请在触发动作后间隔60ms以上再读取测距结果
*/


void UltraInit(void)
{
	Trig = 0;
	IT0  = 0;//上升沿或下降沿触发
	EX0 = 0;
}

void UltraTrig(void)
{
	//发送一个触发信号
	Trig = 1;
	Delay10us();
	Delay10us();
	Trig = 0;

	EX0 = 1; 
	EchoFlag=0;
}

unsigned char  UltraDis(void) //返回距离 单位：厘米  最大255厘米
{
	unsigned char  Distance=0;
	unsigned long temp=0;

	if(EchoFlag==1)
	{
		temp = T0Temp; // 必须先转化为4字节变量，否则计算过程中可能丢失高字节
		Distance = (temp*6*170)/100000;  // c * T * 340 / 2 , T = 0.0000006 s 
	}
	else
		return 0xFF;//超声波测距失败

	return Distance;
}
	
void Int0Pro(void) interrupt 0
{
	if(Echo==1)//检测到上升沿，定时器清零并启动
	{
		TL0=0;
		TH0=0;
		TR0 = 1;
		EchoFlag=0;
	}
	else //检测到下降沿，停止计时并保存计数器值，置位检测完成标志位 EchoFlag
	{
		TR0=0;
		T0Temp  = TH0;
		T0Temp  <<= 8;
		T0Temp  += TL0;
		EchoFlag = 1;
	}
}






