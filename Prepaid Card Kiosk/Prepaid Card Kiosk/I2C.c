/*
 * I2C.c
 *
 * Created: 9/30/2026 7:59:07 PM
 *  Author: S
 */ 
#include <avr/io.h>

#include "I2C.h"

#define F_CPU       16000000UL
#define I2C_FREQ    100000UL

void I2C_Init(void)
{
    /*
        SCL = 100 kHz

        TWBR = (F_CPU / F_SCL - 16) / 2
    */

    TWSR = 0x00;

    TWBR = (u8)((F_CPU / I2C_FREQ - 16) / 2);

    TWCR = (1 << TWEN);
}

u8 I2C_Start(u8 address)
{
    /*
        START condition
    */

    TWCR = (1 << TWINT) |
           (1 << TWSTA) |
           (1 << TWEN);

    while(!(TWCR & (1 << TWINT)));

    /*
        Send address
    */

    TWDR = address;

    TWCR = (1 << TWINT) |
           (1 << TWEN);

    while(!(TWCR & (1 << TWINT)));

    /*
        Check ACK
    */

    if(((TWSR & 0xF8) == 0x18) ||
       ((TWSR & 0xF8) == 0x40))
    {
        return TRUE;
    }

    return FALSE;
}

u8 I2C_Write(u8 data)
{
    TWDR = data;

    TWCR = (1 << TWINT) |
           (1 << TWEN);

    while(!(TWCR & (1 << TWINT)));

    if((TWSR & 0xF8) == 0x28)
    {
        return TRUE;
    }

    return FALSE;
}

u8 I2C_ReadACK(void)
{
    TWCR = (1 << TWINT) |
           (1 << TWEN) |
           (1 << TWEA);

    while(!(TWCR & (1 << TWINT)));

    return TWDR;
}

u8 I2C_ReadNACK(void)
{
    TWCR = (1 << TWINT) |
           (1 << TWEN);

    while(!(TWCR & (1 << TWINT)));

    return TWDR;
}

void I2C_Stop(void)
{
    TWCR = (1 << TWINT) |
           (1 << TWEN) |
           (1 << TWSTO);
}
