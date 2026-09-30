/*
 * SPI.c
 *
 * Created: 9/30/2026 7:57:43 PM
 *  Author: S
 */ 
#include <avr/io.h>

#include "SPI.h"

void SPI_Init(void)
{
    /*
        ATmega328P SPI

        PB2 = SS
        PB3 = MOSI
        PB4 = MISO
        PB5 = SCK
    */

    DDRB |= (1 << PB2);
    DDRB |= (1 << PB3);
    DDRB |= (1 << PB5);

    DDRB &= ~(1 << PB4);

    /*
        Enable SPI
        Master mode
        SPI Mode 0
        Clock = F_CPU / 4
    */

    SPCR = (1 << SPE) |
           (1 << MSTR);

    SPSR = 0;
}

u8 SPI_Transfer(u8 data)
{
    SPDR = data;

    while(!(SPSR & (1 << SPIF)));

    return SPDR;
}