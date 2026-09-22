

#define  F_CPU 16000000
#include <avr/io.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "UART.h"




void UART_init( unsigned int baudRate)
{
	
	CLR_BIT(DDRD,0); //RX
	SET_BIT(DDRD,1); //TX
	
    unsigned int ubrr = (F_CPU / (16UL * baudRate)) - 1;

	
	//Asynchronous USART
	CLR_BIT(UCSR0C,6);
	CLR_BIT(UCSR0C,7);
	//Disabled parity
	CLR_BIT(UCSR0C,4);
	CLR_BIT(UCSR0C,5);
	//Stop Bit(s) 1
	CLR_BIT (UCSR0C,3);
	//8 bits
	SET_BIT(UCSR0C,1);
	SET_BIT(UCSR0C,2);
	CLR_BIT(UCSR0B,2);
	//baud rate 9600
	//UBRR0L=103;
UBRR0H = (ubrr >> 8);
UBRR0L = ubrr;

	//• Bit 4 – RXENn: Receiver Enable n
	SET_BIT(UCSR0B,4);
	//• Bit 3 – TXENn: Transmitter Enable n
	SET_BIT(UCSR0B,3);
	

}
void UART_Send(u8 data)
{

//BUFFER UNTIL EMPTY
while(GET_BIT(UCSR0A,5) == 0);
//SEND DATA
UDR0=data;


//TX COMPLETE
//data
//WAIT TX FLAG
	
}
u8 UART_Recieve(void)
{
	while(GET_BIT(UCSR0A,7) == 0);
	return UDR0;

}