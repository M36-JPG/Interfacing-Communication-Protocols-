///Header Gaurd

#ifndef UART_H_
#define UART_H_


void UART_init( unsigned int baudRate);
void UART_Send(u8 data);
u8 UART_Recieve(void);



#endif 