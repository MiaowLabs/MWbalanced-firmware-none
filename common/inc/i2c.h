
/*****************************************************

  普通IO模拟I2C通信
  

*****************************************************/

#ifndef  __I2C_H__
#define  __I2C_H__

#include "IAP15W4K61S4.h"
#include "IOConfig.h"
	
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
#include <intrins.H>





#define	SlaveAddress 0xD0   //IIC写入时的地址字节数据，+1为读取


//********函数初始声明******************************
void  Delay5us();
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(bit ack);
bit   I2C_RecvACK();
void  I2C_SendByte(unsigned char dat);
unsigned char I2C_RecvByte();
unsigned char Single_ReadI2C(unsigned char REG_Address);						//读取I2C数据
void  Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data);	    //向I2C写入数据






#endif