/*
 * SPI_SLAVE_APP.c
 *
 * Created: 9/17/2026 2:37:44 PM
 * Author : Eman.Assem
 */ 
#define F_CPU 16000000
#include <util/delay.h>

#include <avr/io.h>
#include "BIT_MATH.h"
#include "STD_TYPES.h"

#include "SPI.h"

int main(void)
{
	u8 receivedData=0;
	/**LED */
	SET_BIT(DDRC,1);
	
	SPI_voidSlave_Init();

		SET_BIT(DDRB,1);
		SET_BIT(PORTB,1);

	
	
    /* Replace with your application code */
    while (1) 
    {
		
		receivedData = SPI_u8Receive();
		if(receivedData == 'A')
		SET_BIT(PORTC,1);
			
    }
}

