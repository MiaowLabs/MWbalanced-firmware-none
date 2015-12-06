
#include "MPU6050.H"

/***************************************************************
** 函数名称: MPU6050Init
** 功能描述: MPU6050初始化函数
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlabs.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
void MPU6050Init(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00)  ; //解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07)  ; //陀螺仪125hz
	Single_WriteI2C(CONFIG, 0x04)      ; //Accelerometer:21hz 8.5ms ; Gyroscope:20hz 8.3ms
	//Single_WriteI2C(CONFIG, 0x00); 	 //0x06
	Single_WriteI2C(GYRO_CONFIG, 0x18) ; // ±2000°/s  
	Single_WriteI2C(ACCEL_CONFIG, 0x01); // ±2g
}








