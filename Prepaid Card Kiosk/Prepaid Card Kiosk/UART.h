/*
 * UART.h
 *
 * Created: 9/30/2026 7:59:35 PM
 *  Author: S
 */ 


#ifndef UART_H_
#define UART_H_



#include "STD_TYPES.h"

void UART_Init(u16 baudRate);

void UART_Send(u8 data);

u8 UART_Receive(void);

void UART_SendString(const char *str);

u8 UART_Available(void);




#endif /* UART_H_ */