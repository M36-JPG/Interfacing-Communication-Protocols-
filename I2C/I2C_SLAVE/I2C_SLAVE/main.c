/*
 * I2C_SLAVE.c
 *
 * Created: 9/28/2026 8:15:47 AM
 * Author : Eman.Assem
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include "I2C.h"

#define SLAVE_ADDRESS  0x23

int main(void)
{
	uint8_t data;

	// PB0 = LED
	DDRC |= (1 << PORTC);

	// Initially LED OFF
	PORTC &= ~(1 << PC0);

	// Slave address = 0x50
	I2C_Slave_Init(SLAVE_ADDRESS);

	while (1)
	{
		data = I2C_Slave_Receive();

		if (data == 'B')
		{
			// LED ON
			PORTC |= (1 << PC0);
		}
		 if (data == 'O')
		{
			// LED OFF
			PORTC &= ~(1 << PC0);
		}
	}
}