/*
 * Master.c
 *
 * Created: 9/21/2026 9:05:22 AM
 * Author : Sama.Adel
 */ 

#define F_CPU 16000000
#include <util/delay.h>

#include <avr/io.h>
#include "BIT_MATH.h"


#include "Master.h"

int main(void)
{
	u8 data=0;
	/**LED */
	SET_BIT(DDRC,0);
	
	SPI_Slave_Int();



	
	
    /* Replace with your application code */
    while (1) 
    {
		
		data = SPI_Resiver();
		if(data == 'A')
		SET_BIT(PORTC,0);
			
    }
}


