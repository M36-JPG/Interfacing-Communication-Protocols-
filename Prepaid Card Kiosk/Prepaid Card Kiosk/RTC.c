/*
 * RTC.c
 *
 * Created: 9/30/2026 8:03:15 PM
 *  Author: S
 */ 
#include "I2C.h"

#include "RTC.h"

#define RTC_ADDRESS 0x68

u8 RTC_BCDToDec(u8 value)
{
    return ((value >> 4) * 10) +
           (value & 0x0F);
}

u8 RTC_DecToBCD(u8 value)
{
    return ((value / 10) << 4) |
           (value % 10);
}

void RTC_Init(void)
{
    I2C_Start((RTC_ADDRESS << 1) |
              I2C_WRITE);

    I2C_Write(0x00);

    I2C_Write(0x00);

    I2C_Stop();
}

u8 RTC_SetTime(const RTC_Time *time)
{
    if(!I2C_Start((RTC_ADDRESS << 1) |
                  I2C_WRITE))
    {
        return FALSE;
    }

    I2C_Write(0x00);

    I2C_Write(RTC_DecToBCD(time->sec));

    I2C_Write(RTC_DecToBCD(time->min));

    I2C_Write(RTC_DecToBCD(time->hour));

    I2C_Write(0x01);

    I2C_Write(RTC_DecToBCD(time->day));

    I2C_Write(RTC_DecToBCD(time->month));

    I2C_Write(RTC_DecToBCD(time->year));

    I2C_Stop();

    return TRUE;
}

u8 RTC_GetTime(RTC_Time *time)
{
    if(!I2C_Start((RTC_ADDRESS << 1) |
                  I2C_WRITE))
    {
        return FALSE;
    }

    I2C_Write(0x00);

    if(!I2C_Start((RTC_ADDRESS << 1) |
                  I2C_READ))
    {
        return FALSE;
    }

    time->sec =
        RTC_BCDToDec(I2C_ReadACK());

    time->min =
        RTC_BCDToDec(I2C_ReadACK());

    time->hour =
        RTC_BCDToDec(I2C_ReadACK());

    /*
        Day of week
    */

    I2C_ReadACK();

    time->day =
        RTC_BCDToDec(I2C_ReadACK());

    time->month =
        RTC_BCDToDec(I2C_ReadACK());

    time->year =
        RTC_BCDToDec(I2C_ReadNACK());

    I2C_Stop();

    return TRUE;
}

u8 RTC_IsPeak(const RTC_Time *time)
{
    /*
        Morning peak:
        06:00 - 09:00

        Evening peak:
        16:00 - 19:00
    */

    if((time->hour >= 6 &&
        time->hour < 9) ||

       (time->hour >= 16 &&
        time->hour < 19))
    {
        return TRUE;
    }

    return FALSE;
}
