
#include "adc.h"
 #include "intrins.h"

void ADCInit(void)
{
    P1ASF |= 0x20;                  //P1.5口作为模拟功能A/D使用
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = 0x80;				//20M/540=37kHz转换速度

}

/*----------------------------
读取ADC结果
----------------------------*/
unsigned char GetADCResult(void)
{

    ADC_CONTR = 0x8D;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x10));//等待ADC转换完成
    ADC_CONTR &= ~0x10;         //Close ADC

    return ADC_RES;                 //返回ADC结果

}