/*
 * UART.h
 *
 * Created: 9/18/2026 1:16:25 AM
 *  Author: user
 */ 


#ifndef UART_H_
#define UART_H_

#include"STD_TYPES.h"
#include"BIT_MATH.h"

void UART_Init(void);

void UART_SendChar(u8 data);

u8 UART_Recieve(void);

void UART_SendString(u8 *str);


#endif /* UART_H_ */