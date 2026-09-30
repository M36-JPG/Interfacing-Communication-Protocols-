/*
 * EEPROM.c
 *
 * Created: 9/30/2026 8:01:55 PM
 *  Author: S
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "EEPROM.h"
#include "I2C.h"
#include <util/delay.h>



u8 EEPROM_WriteByte(u16 address,
                    u8 data)
{
    if(!I2C_Start((EEPROM_ADDRESS << 1) |
                  I2C_WRITE))
    {
        return FALSE;
    }

    I2C_Write((u8)(address >> 8));

    I2C_Write((u8)address);

    I2C_Write(data);

    I2C_Stop();

    /*
        EEPROM write cycle
    */

    _delay_ms(5);

    /*
        Verification
    */

    if(EEPROM_ReadByte(address) == data)
    {
        return TRUE;
    }

    return FALSE;
}

u8 EEPROM_ReadByte(u16 address)
{
    u8 data;

    data = 0;

    /*
        Send memory address
    */

    if(!I2C_Start((EEPROM_ADDRESS << 1) |
                  I2C_WRITE))
    {
        return 0;
    }

    I2C_Write((u8)(address >> 8));

    I2C_Write((u8)address);

    /*
        Repeated START
    */

    if(!I2C_Start((EEPROM_ADDRESS << 1) |
                  I2C_READ))
    {
        return 0;
    }

    data = I2C_ReadNACK();

    I2C_Stop();

    return data;
}

u8 EEPROM_WriteBlock(u16 address,
                     const u8 *data,
                     u16 length)
{
    u16 i;

    for(i = 0; i < length; i++)
    {
        if(!EEPROM_WriteByte(address + i,
                             data[i]))
        {
            return FALSE;
        }
    }

    return TRUE;
}

u8 EEPROM_ReadBlock(u16 address,
                    u8 *data,
                    u16 length)
{
    u16 i;

    for(i = 0; i < length; i++)
    {
        data[i] =
            EEPROM_ReadByte(address + i);
    }

    return TRUE;
}
