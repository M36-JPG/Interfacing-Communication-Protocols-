#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "BIT_MATH.h"
#include "UART.h"
#include "SPI.h"

#define F_CPU 1000000

#define DIP_MASK       0x0F
#define DIP_1          PC0
#define DIP_2          PC1
#define DIP_3          PC2
#define DIP_4          PC3

#define B1_PIN         PD2
#define B2_PIN         PD3

#define SPI_START      0xAA
#define SS_PIN         PB2
 

void DIP_Init(void)
{
       CLR_BIT(DDRC , PC0);
	    CLR_BIT(DDRC , PC1);
		 CLR_BIT(DDRC , PC2);
		  CLR_BIT(DDRC , PC3);

       SET_BIT(PORTC , PC0);
	     SET_BIT(PORTC , PC1);
		   SET_BIT(PORTC , PC2);
		     SET_BIT(PORTC , PC3);
}



u8 DIP_Read(void)
{
    return ((~PINC) & DIP_MASK); // to read the hole port 
}


u8 DIP_To_Temperature(void)
{
    u8 dip;
	dip = DIP_Read();
	return (dip + 15); 
	// to control the min & max

}


void Buttons_Init(void)
{
    DDRD &= ~(1 << B1_PIN);
    DDRD &= ~(1 << B2_PIN);

    PORTD |= (1 << B1_PIN);
    PORTD |= (1 << B2_PIN);
}


u8 B1_Read(void)
{
    return ((PIND >> B1_PIN) & 1);
}


u8 B2_Read(void)
{
    return ((PIND >> B2_PIN) & 1);
}

void Send_SetMin(u8 temperature)
{
	UART_SendString("Set Min ");
	UART_SendChar((temperature / 10) + '0');
	UART_SendChar((temperature % 10) + '0');
	UART_SendString("\r\n"); 
}


void Send_SetMax(uint8_t temperature)
{
	 UART_SendString("Set Max ");
	 UART_SendChar((temperature / 10) + '0');
	 UART_SendChar((temperature % 10) + '0');
	 UART_SendString("\r\n"); 
}

void SPI_Send_Panel(uint8_t dip , uint8_t b1 , uint8_t b2)
{
    CLR_BIT(PORTB , SS_PIN );
	
	SPI_u8Transceive(SPI_START);
	SPI_u8Transceive(dip);
	SPI_u8Transceive(b1);
	SPI_u8Transceive(b2);
	
	SET_BIT(PORTB , SS_PIN);
	
}

int main(void)
{ 
	u8 dip , temperature , b1 , b2 ;
	
	u8 prev_b1 = 1 , prev_b2 = 1;
    
	   UART_Init(9600);
	   SPI_voidMaster_Init();
	   DIP_Init();
	   Buttons_Init();
	 
    while (1)
    {
         dip = DIP_Read();
		 temperature = DIP_To_Temperature();
		 b1 = B1_Read();
		 b2 = B2_Read();


           if(b1 == 0 && prev_b1 == 1){
			   _delay_ms(20);
			   if(B1_Read() == 0){
			     Send_SetMin(temperature);
			   }
		   }

		   if(b2 == 0 && prev_b2 == 1){
			   _delay_ms(20);
			   if(B2_Read() == 0){
				   Send_SetMax(temperature);
			   }
		   }
		
		    SPI_Send_Panel(dip , b1 , b2);
           _delay_ms(100);


           prev_b1 = b1;
		   prev_b2 = b2;
	}

    return 0;
}
