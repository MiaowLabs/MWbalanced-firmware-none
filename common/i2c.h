
/*****************************************************

  ��ͨIOģ��I2Cͨ��
  

*****************************************************/

#ifndef  __I2C_H__
#define  __I2C_H__


#include "IAP15W4K61S4.h"	
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
#include <intrins.H>



//********�˿ڶ���**********************************
sbit    SCL = P4^6;			//I2Cʱ�����Ŷ���
sbit    SDA = P4^5;			//I2C�������Ŷ���

#define	SlaveAddress 0xD0   //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


//********������ʼ����******************************
void  Delay5us();
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(bit ack);
bit   I2C_RecvACK();
void  I2C_SendByte(unsigned char dat);
unsigned char I2C_RecvByte();
unsigned char Single_ReadI2C(unsigned char REG_Address);						//��ȡI2C����
void  Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data);	    //��I2Cд������






#endif