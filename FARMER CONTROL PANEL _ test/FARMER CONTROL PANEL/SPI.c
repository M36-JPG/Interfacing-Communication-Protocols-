#include "SPI.h"
#include <avr/io.h>


#define SPI_SS      PB2
#define SPI_MOSI    PB3
#define SPI_MISO    PB4
#define SPI_SCK     PB5


/* =========================================
   MASTER INITIALIZATION
   ========================================= */

void SPI_voidMaster_Init(void)
{
	SET_BIT(DDRB , SPI_MOSI); //output 
	
	CLR_BIT(DDRB , SPI_MISO); 
	
	SET_BIT(PORTB , SPI_SS);
	SET_BIT(DDRB , SPI_SS);
	
	SET_BIT(DDRB , SPI_SCK);
	
	//Master
	SET_BIT(SPCR , 4);
	
	//LSB
	SET_BIT(SPCR , 5);
	
	//clock polarity
	CLR_BIT(SPCR , 3);
	
	//clock phase 
	SET_BIT(SPCR , 2);
	
	
	//freq = 16
	SET_BIT(SPCR , 0);
	CLR_BIT(SPCR , 1);
	CLR_BIT(SPSR, 0);
	
	
     /* SPI Enable - Master Mode */
     SET_BIT(SPCR , SPE);
}


/* =========================================
   SLAVE INITIALIZATION
   ========================================= */

void SPI_voidSlave_Init(void)
{
    /* MISO -> OUTPUT */
    DDRB |= (1 << SPI_MISO);

    /* MOSI, SCK, SS -> INPUT */
    DDRB &= ~(1 << SPI_MOSI);
    DDRB &= ~(1 << SPI_SCK);
    DDRB &= ~(1 << SPI_SS);
 
 // Slave
 CLR_BIT(SPCR , 4);
 
 //LSB
 SET_BIT(SPCR , 5);
 
 //clock polarity
 CLR_BIT(SPCR , 3);
 
 //clock phase
 SET_BIT(SPCR , 2);
 
 
 //freq = 16
 SET_BIT(SPCR , 0);
 CLR_BIT(SPCR , 1);
 CLR_BIT(SPSR, 0);
 

    /* SPI Enable - Slave Mode */
    SPCR = (1 << SPE);
}


/* =========================================
   SEND
   ========================================= */

u8 SPI_u8Transceive(uint8_t data)
{
   SPDR = data ;
   while(!GET_BIT(SPSR , 7));
   return data;
}


/* =========================================
   RECEIVE
   ========================================= */

u8 SPI_u8Receive(void)
{
    while(!GET_BIT(SPSR , 7));
	return SPDR;
}
