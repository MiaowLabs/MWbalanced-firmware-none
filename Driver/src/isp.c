
#include "isp.h"


/***************************************************************
** 函数名称: STC_ISP
** 功能描述: 免断电烧录程序
** 输　入:   
** 输　出:   
** 全局变量: 
** 作　者:   喵呜实验室
** 淘  宝：  Http://miaowlab.taobao.com
** 日　期:   2014年08月01日
***************************************************************/
 void STC_ISP()
 {
    
	IN_OFF=1; 

    if(!IN_OFF){
	IAP_CONTR=0x60;} //判断串口是否有数据过来，然后软件复位，系统重新从ISP代码区开始运行程序
  
 }


