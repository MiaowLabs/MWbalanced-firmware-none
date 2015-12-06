
#ifndef _UART_H_  
#define _UART_H_  
 
#include "IAP15W4K61S4.h"

void Uart1Init(void);
void Uart2Init(void);
unsigned char UART1ReceiveByte(void);
unsigned char UART2ReceiveByte(void);
void UART1SendByte(unsigned char TxD1); 
void UART2SendByte(unsigned char TxD2); 


#endif  