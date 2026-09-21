/*
 * MASTER.c
 *
 * Created: 9/20/2026 3:05:04 PM
 * Author : Mariam.Kotb
 */ 

#include <avr/io.h>
#include"Master.h"

int main(void)
{
	u8 recieved_data;
	SPI_Master_Init();
	SET_BIT(DDRB , PB0);//LED PIN
	
	CLR_BIT(PORTB , PB0);// TURN OFF THE LED
	/* Replace with your application code */
	while (1)
	{
		recieved_data = SPI_Recieve();
		if(recieved_data == 1){
			SET_BIT(PORTB , PB0);
		}
		else{
			CLR_BIT(PORTB , PB0);
		}
		
	}
}


