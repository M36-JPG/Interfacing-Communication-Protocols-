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
    /* MOSI, SCK, SS -> OUTPUT */
    DDRB |= (1 << SPI_MOSI);
    DDRB |= (1 << SPI_SCK);
    DDRB |= (1 << SPI_SS);

    /* MISO -> INPUT */
    DDRB &= ~(1 << SPI_MISO);

    /* SS HIGH */
    PORTB |= (1 << SPI_SS);

    /*
     * SPI Enable
     * Master Mode
     * Clock = F_CPU / 16
     * Mode 0
     * MSB First
     */

    SPCR =
        (1 << SPE)  |
        (1 << MSTR) |
        (1 << SPR0);

    SPSR &= ~(1 << SPI2X);
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

    /* SPI Enable - Slave Mode */
    SPCR = (1 << SPE);
}


/* =========================================
   SEND
   ========================================= */

void SPI_u8Transceive(uint8_t data)
{
    /* Put data in SPDR */
    SPDR = data;

    /* Wait for transfer complete */
    while (!(SPSR & (1 << SPIF)))
    {
    }

    /* Clear SPIF */
    (void)SPDR;
}


/* =========================================
   RECEIVE
   ========================================= */

u8 SPI_u8Receive(void)
{
    /* Wait for transfer complete */
    while (!(SPSR & (1 << SPIF)))
    {
    }

    /* Reading SPDR completes SPIF clearing */
    return SPDR;
}