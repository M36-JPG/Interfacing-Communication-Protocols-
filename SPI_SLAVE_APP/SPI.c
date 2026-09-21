
#include <avr/io.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SPI.h"



void SPI_voidMaster_Init(void)
{

    /* MOSI -> Output */
    SET_BIT(DDRB, PB3);

    /* MISO -> Input */
    CLR_BIT(DDRB, PB4);

    /* SCK -> Output */
    SET_BIT(DDRB, PB5);

    /* SS -> Output */
    SET_BIT(DDRB, PB2);

    /* SS initially HIGH */
    SET_BIT(PORTB, PB2);




    /* Master Mode */
    SET_BIT(SPCR, MSTR);

    /* MSB First */
    CLR_BIT(SPCR, DORD);

    /* Clock = F_CPU / 16
       SPR1 = 0
       SPR0 = 1
    */
    CLR_BIT(SPCR, SPR1);
    SET_BIT(SPCR, SPR0);

    /* CPOL = 0 ? Clock idle LOW */
    CLR_BIT(SPCR, CPOL);

    /* CPHA = 1 ? Sample on trailing edge */
    SET_BIT(SPCR, CPHA);
	
/* Enable SPI */
SET_BIT(SPCR, SPE);



}
void SPI_voidSlave_Init(void)
{
	/* MOSI -> Input */
	CLR_BIT(DDRB, PB3);

	/* MISO -> Output */
	SET_BIT(DDRB, PB4);

	/* SCK -> Input */
	CLR_BIT(DDRB, PB5);

	/* SS -> Input */
	CLR_BIT(DDRB, PB2);

	/* Slave Mode */
	CLR_BIT(SPCR, MSTR);

	/* MSB First */
	CLR_BIT(SPCR, DORD);

	/* CPOL = 0 */
	CLR_BIT(SPCR, CPOL);

	/* CPHA = 1 */
	SET_BIT(SPCR, CPHA);
	/* Enable SPI */
	SET_BIT(SPCR, SPE);
	
}
u8 SPI_u8Transceive(u8 data)
{
	SPDR = data;

	while(GET_BIT(SPSR, SPIF) == 0);

	return SPDR;
}
u8 SPI_u8Receive(void)
{
	
	while(GET_BIT(SPSR, SPIF) == 0);

	return SPDR;
}

