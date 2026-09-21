#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SPI.h"

int main(void)
{
	SPI_voidMaster_Init();

	while(1)
	{
		

		/* Send 'A' */
		SPI_u8Transceive('A');

		
	}
}