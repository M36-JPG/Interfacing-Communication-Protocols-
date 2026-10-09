#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "BIT_MATH.h"

#include "UART.h"
#include "SPI.h"


/* =========================================================
   DIP Switch
   ========================================================= */

#define DIP_MASK       0x0F
#define DIP_1          PC0
#define DIP_2          PC1
#define DIP_3          PC2
#define DIP_4          PC3
/*
 * Temperature = DIP + 15
 *
 * DIP = 0000 -> 15 C
 * DIP = 0001 -> 16 C
 * DIP = 0101 -> 20 C
 * DIP = 1010 -> 25 C
 * DIP = 1111 -> 30 C
 */

/* =========================================================
   Buttons
   ========================================================= */

#define B1_PIN         PD2
#define B2_PIN         PD3


/* =========================================================
   SPI
   ========================================================= */

#define SPI_START      0xAA
#define SS_PIN         PB2

/* =========================================================
   DIP Initialization
   ========================================================= */

void DIP_Init(void)
{
    /*
     * PC0-PC3 = INPUT
     */

//     DDRC &= ~(1 << PC0);
//     DDRC &= ~(1 << PC1);
//     DDRC &= ~(1 << PC2);
//     DDRC &= ~(1 << PC3);
       CLR_BIT(DDRC , PC0);
	    CLR_BIT(DDRC , PC1);
		 CLR_BIT(DDRC , PC2);
		  CLR_BIT(DDRC , PC3);

    /*
     * Internal Pull-ups
     */

//     PORTC |= (1 << PC0);
//     PORTC |= (1 << PC1);
//     PORTC |= (1 << PC2);
//     PORTC |= (1 << PC3);
       SET_BIT(PORTC , PC0);
	     SET_BIT(PORTC , PC1);
		   SET_BIT(PORTC , PC2);
		     SET_BIT(PORTC , PC3);
}


/* =========================================================
   Read DIP
   ========================================================= */

uint8_t DIP_Read(void)
{
    /*
     * Hardware:
     *
     * Switch ON  = PIN 0
     * Switch OFF = PIN 1
     *
     * Convert:
     *
     * ON  = 1
     * OFF = 0
     */

    return ((~PINC) & DIP_MASK); // to read the hole port 
}


/* =========================================================
   Convert DIP to Temperature
   ========================================================= */

uint8_t DIP_To_Temperature(void)
{
    uint8_t dip;
	dip = DIP_Read();
	return (dip + 15); // to make a representation for the temperature 
	// to control the min & max

}


/* =========================================================
   Buttons Initialization
   ========================================================= */

void Buttons_Init(void)
{
    /*
     * B1 = PD2
     * B2 = PD3
     */

    DDRD &= ~(1 << B1_PIN);
    DDRD &= ~(1 << B2_PIN);


    /*
     * Internal Pull-ups
     */

    PORTD |= (1 << B1_PIN);
    PORTD |= (1 << B2_PIN);
}


/* =========================================================
   Read B1
   ========================================================= */

uint8_t B1_Read(void)
{
    /*
     * Released = 1
     * Pressed  = 0
     */

    return ((PIND >> B1_PIN) & 1);
}


/* =========================================================
   Read B2
   ========================================================= */

uint8_t B2_Read(void)
{
    /*
     * Released = 1
     * Pressed  = 0
     */

    return ((PIND >> B2_PIN) & 1);
}


/* =========================================================
   Send SETMIN
   ========================================================= */

void Send_SetMin(uint8_t temperature)
{
	UART_SendString("Set Min ");
	UART_SendChar((temperature / 10) + '0');
	UART_SendChar((temperature % 10) + '0');
	UART_SendString("\r\n"); 
}


/* =========================================================
   Send SETMAX
   ========================================================= */

void Send_SetMax(uint8_t temperature)
{
	 UART_SendString("Set Max ");
	 UART_SendChar((temperature / 10) + '0');
	 UART_SendChar((temperature % 10) + '0');
	 UART_SendString("\r\n"); 
}


/* =========================================================
   Send SPI Panel Frame
   ========================================================= */

void SPI_Send_Panel(uint8_t dip , uint8_t b1 , uint8_t b2)
{
    CLR_BIT(PORTB , SS_PIN );
	
	SPI_u8Transceive(SPI_START);
	SPI_u8Transceive(dip);
	SPI_u8Transceive(b1);
	SPI_u8Transceive(b2);
	
	SET_BIT(PORTB , SS_PIN);
	
}


/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{ 
	uint8_t dip , temperature , b1 , b2 ;
	
	uint8_t prev_b1 = 1 , prev_b2 = 1;
    


    /* =====================================================
       Initialization
       ===================================================== */
	   UART_Init(9600);
	   SPI_voidMaster_Init();
	   DIP_Init();
	   Buttons_Init();
	   
   


    /* =====================================================
       Main Loop
       ===================================================== */

    while (1)
    {
        /* =================================================
           Read Buttons
           ================================================= */
         dip = DIP_Read();
		 temperature = DIP_To_Temperature();
		 b1 = B1_Read();
		 b2 = B2_Read();


        /* =================================================
           Read Temperature From DIP
           ================================================= */
           
        


        /* =================================================
           B1 ? SETMIN
           ================================================= */
           if(b1 == 0 && prev_b1 == 1){
			   _delay_ms(20);
			   if(B1_Read() == 0){
			     Send_SetMin(temperature);
			   }
		   }

        /* =================================================
           B2 ? SETMAX
           ================================================= */
           
		   if(b2 == 0 && prev_b2 == 1){
			   _delay_ms(20);
			   if(B2_Read() == 0){
				   Send_SetMax(temperature);
			   }
		   }
		   
        /* =================================================
           SPI Panel Status
           ================================================= */
           
		    SPI_Send_Panel(dip , b1 , b2);
           _delay_ms(100);


        /* =================================================
           Save Button State
           ================================================= */
           prev_b1 = b1;
		   prev_b2 = b2;
	}

    return 0;
}
