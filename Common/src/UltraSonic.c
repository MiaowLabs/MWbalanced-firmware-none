#include "includes.h"


sbit Trig  = P3^6;
sbit Echo  = P3^2;

unsigned short  xdata T0Temp=0;
bit EchoFlag=0;

/*

* ģ��˵�� *

* ��ģ��ʹ�õ�Ƭ�� ��ʱ��0 �� �ⲿ�ж�0���������ģ�鲻���ظ�ʹ��

* ʹ�÷���: 1�� ����UltraInit()���г�ʼ��
		    2�� ����UltraTrig()����һ�β�ද�������ε���ʱ����60�������ϣ�
		        ��ֹ�����źŶ��ϴλ���Ӱ��
		    3�� ����UltraDis()��ȡ����������Ϊһ�ֽ�ʮ������������λΪ���ס�
		   	    ����0xff����δ��ɻ�ʧ�ܡ�

* Ϊ��֤��ȡ���ݳɹ������ڴ�����������60ms�����ٶ�ȡ�����
*/


void UltraInit(void)
{
	Trig = 0;
	IT0  = 0;//�����ػ��½��ش���
	EX0 = 0;
}

void UltraTrig(void)
{
	//����һ�������ź�
	Trig = 1;
	Delay10us();
	Delay10us();
	Trig = 0;

	EX0 = 1; 
	EchoFlag=0;
}

unsigned char  UltraDis(void) //���ؾ��� ��λ������  ���255����
{
	unsigned char  Distance=0;
	unsigned long temp=0;

	if(EchoFlag==1)
	{
		temp = T0Temp; // ������ת��Ϊ4�ֽڱ����������������п��ܶ�ʧ���ֽ�
		Distance = (temp*6*170)/100000;  // c * T * 340 / 2 , T = 0.0000006 s 
	}
	else
		return 0xFF;//���������ʧ��

	return Distance;
}
	
void Int0Pro(void) interrupt 0
{
	if(Echo==1)//��⵽�����أ���ʱ�����㲢����
	{
		TL0=0;
		TH0=0;
		TR0 = 1;
		EchoFlag=0;
	}
	else //��⵽�½��أ�ֹͣ��ʱ�����������ֵ����λ�����ɱ�־λ EchoFlag
	{
		TR0=0;
		T0Temp  = TH0;
		T0Temp  <<= 8;
		T0Temp  += TL0;
		EchoFlag = 1;
	}
}






