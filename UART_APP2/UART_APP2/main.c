/*
* UART_APP2.c
*
* Created: 9/21/2026 2:32:02 PM
* Author : Eman.Assem
*/
#define F_CPU 16000000

#include <avr/io.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "UART.h"


int main(void)
{
	u8 data=0;
	SET_BIT(DDRD,3);  //led
	

	UART_init(9600);
	
	/* Replace with your application code */
	while (1)
	{
		
		data=UART_Recieve();
		if(data == 6)
		SET_BIT(PORTD,3);// led on
		else
		UART_Send(1);
		
	}
}

