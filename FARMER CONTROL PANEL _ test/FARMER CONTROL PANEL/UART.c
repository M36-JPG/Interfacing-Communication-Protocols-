
#include <avr/io.h>
#include "uart.h"

#define F_CPU 1000000UL
#include <util/delay.h>

void UART_Init(uint32_t baudrate)
{
	//baud rate 
  UBRR0H = (u8)(baudrate >> 8);
  UBRR0L = (u8)baudrate;
  
  //RX & TX Enable
  SET_BIT(UCSR0B , 4);
  SET_BIT(UCSR0B , 3);
  
  //Asynch
  CLR_BIT(UCSR0C , 7);
  CLR_BIT(UCSR0C , 6);
  
  //No parity 
  CLR_BIT(UCSR0C , 5);
  CLR_BIT(UCSR0C , 4);
  
  // 1 stop bit 
  CLR_BIT(UCSR0C , 4);
  
  // 8 data bit 
  SET_BIT(UCSR0C , 2);
  SET_BIT(UCSR0C , 1);
  CLR_BIT(UCSR0B , 1);
  
}


void UART_SendChar(char data)
{
   while(GET_BIT(UCSR0A , 5));
   UDR0 = data ;
}

void UART_SendString(const char *str)
{
   while(GET_BIT(UCSR0A , 5));
   UART_SendChar(*str);
}

char UART_ReceiveChar(void)
{
	while(GET_BIT(UCSR0A , 7));
	return UDR0;
}

void UART_SendHex(uint8_t data)
{
	const char hex[] = "0123456789ABCDEF";

	UART_SendChar(hex[(data >> 4) & 0x0F]);
	UART_SendChar(hex[data & 0x0F]);
}

void UART_ReceiveString(char buff , u8 max)     //read full line until enter
{
	u8 i ; int data ;  
	while (i < max -1 ){
		data =	UART_ReceiveChar();
		if (data == -1){
			continue;
		}
		if(data == '\n' || data == '\r'){ break;}
			buff[i]==(char) data;
			i++;
	}
	buff[i]='\0';
}
uint8_t UART_DataAvailable(void)
{
	return (UCSR0A & (1 << RXC0));
}

