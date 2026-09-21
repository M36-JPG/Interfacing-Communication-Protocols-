/*
 * UART.c
 *
 * Created: 9/18/2026 1:16:09 AM
 *  Author: user
 */ 

#include<avr/io.h>
#include"USART.h"

#define F_CPU 1600000UL

#define BAUD 9600UL

#define UBRR_VALUE ((F_CPU /(16UL *BAUD))-1)
// UCSR0A
#define RXC0 7
#define TXC0 6
#define UDRE0 5

//UCSR0B
#define RXEN0 4
#define TXEN0 3

//UCSR0C
#define UMSEL01 7
#define UMSEL00 6
#define UCSZ01 2
#define UCSZ00 1
void UART_Init(void){
	
	//Set the baud rate 
	UBRR0H = (u8)(UBRR_VALUE >> 8);
	UBRR0L = (u8)UBRR_VALUE;
	
	//Enable Transmit and Receive
	SET_BIT(UCSR0B , TXEN0);
	SET_BIT(UCSR0B , RXEN0);
	
	//Make the data 8-bit
	SET_BIT(UCSR0C,UCSZ00);
	SET_BIT(UCSR0C,UCSZ01);
	
	//No Parity
	CLR_BIT(UCSR0C ,UPM00);
	CLR_BIT(UCSR0C ,UPM01);
	
	//1 Stop bit
	CLR_BIT(UCSR0C , USBS0);
	
	// Asynch mode
	CLR_BIT(UCSR0C , UMSEL00);
	CLR_BIT(UCSR0C , UMSEL01);

}


void UART_SendChar(u8 data){
	
	while(GET_BIT(UCSR0A , UDRE0)==0);
	
	UDRE0 = data;
	
}


u8 UART_Recieve(void){
	
	while(GET_BIT(UCSR0A , RXC0)==0);
	
	return UDRE0;
	
}


void UART_SendString( u8 *str){
	
	while(*str != '\0'){
		UART_SendChar(*str);
		str++;
	}
}

