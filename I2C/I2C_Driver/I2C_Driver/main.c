/*
* I2C_Driver.c
*
* Created: 9/28/2026 8:08:30 AM
* Author : Eman.Assem
*/
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"

#define SLAVE_ADDRESS  0x23

int main(void)
{
	I2C_Master_Init();

	while (1)
	{
		/*
		* Start communication
		* 0x50 = Slave address
		* 0 = Write
		*/

		I2C_Start_condition();
		I2C_Send_SLA_W(SLAVE_ADDRESS);

		// Send letter A
		I2C_Write('B');
		_delay_ms(1000);
		I2C_Write('O');
		_delay_ms(1000);

		I2C_Write('B');
		_delay_ms(1000);

		I2C_Write('O');
		_delay_ms(1000);

		I2C_Stop();
	}
}
