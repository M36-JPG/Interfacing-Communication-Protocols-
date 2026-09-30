/*
 * OLED.c
 *
 * Created: 9/30/2026 8:39:31 PM
 *  Author: S
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "OLED.h"
#include "I2C.h"


/* =========================================================
   SSD1306 Commands
   ========================================================= */

#define OLED_CMD_MODE      0x00
#define OLED_DATA_MODE     0x40


/* =========================================================
   Private Functions
   ========================================================= */

static void OLED_WriteCommand(u8 command)
{
    I2C_Start((OLED_ADDRESS << 1) | 0);

    I2C_Write(OLED_CMD_MODE);
    I2C_Write(command);

    I2C_Stop();
}


static void OLED_WriteData(u8 data)
{
    I2C_Start((OLED_ADDRESS << 1) | 0);

    I2C_Write(OLED_DATA_MODE);
    I2C_Write(data);

    I2C_Stop();
}


/* =========================================================
   Small 5x7 Font
   Characters needed by the Kiosk
   ========================================================= */

static void OLED_GetCharPattern(char c, u8 *pattern)
{
    u8 i;

    for(i = 0; i < 5; i++)
    {
        pattern[i] = 0x00;
    }

    switch(c)
    {
        /* Numbers */

        case '0':
            pattern[0] = 0x3E;
            pattern[1] = 0x51;
            pattern[2] = 0x49;
            pattern[3] = 0x45;
            pattern[4] = 0x3E;
            break;

        case '1':
            pattern[0] = 0x00;
            pattern[1] = 0x42;
            pattern[2] = 0x7F;
            pattern[3] = 0x40;
            pattern[4] = 0x00;
            break;

        case '2':
            pattern[0] = 0x42;
            pattern[1] = 0x61;
            pattern[2] = 0x51;
            pattern[3] = 0x49;
            pattern[4] = 0x46;
            break;

        case '3':
            pattern[0] = 0x21;
            pattern[1] = 0x41;
            pattern[2] = 0x45;
            pattern[3] = 0x4B;
            pattern[4] = 0x31;
            break;

        case '4':
            pattern[0] = 0x18;
            pattern[1] = 0x14;
            pattern[2] = 0x12;
            pattern[3] = 0x7F;
            pattern[4] = 0x10;
            break;

        case '5':
            pattern[0] = 0x27;
            pattern[1] = 0x45;
            pattern[2] = 0x45;
            pattern[3] = 0x45;
            pattern[4] = 0x39;
            break;

        case '6':
            pattern[0] = 0x3C;
            pattern[1] = 0x4A;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x30;
            break;

        case '7':
            pattern[0] = 0x01;
            pattern[1] = 0x71;
            pattern[2] = 0x09;
            pattern[3] = 0x05;
            pattern[4] = 0x03;
            break;

        case '8':
            pattern[0] = 0x36;
            pattern[1] = 0x49;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x36;
            break;

        case '9':
            pattern[0] = 0x06;
            pattern[1] = 0x49;
            pattern[2] = 0x49;
            pattern[3] = 0x29;
            pattern[4] = 0x1E;
            break;


        /* A */

        case 'A':
            pattern[0] = 0x7E;
            pattern[1] = 0x09;
            pattern[2] = 0x09;
            pattern[3] = 0x09;
            pattern[4] = 0x7E;
            break;

        case 'B':
            pattern[0] = 0x7F;
            pattern[1] = 0x49;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x36;
            break;

        case 'C':
            pattern[0] = 0x3E;
            pattern[1] = 0x41;
            pattern[2] = 0x41;
            pattern[3] = 0x41;
            pattern[4] = 0x22;
            break;

        case 'D':
            pattern[0] = 0x7F;
            pattern[1] = 0x41;
            pattern[2] = 0x41;
            pattern[3] = 0x22;
            pattern[4] = 0x1C;
            break;

        case 'E':
            pattern[0] = 0x7F;
            pattern[1] = 0x49;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x41;
            break;

        case 'F':
            pattern[0] = 0x7F;
            pattern[1] = 0x09;
            pattern[2] = 0x09;
            pattern[3] = 0x09;
            pattern[4] = 0x01;
            break;

        case 'G':
            pattern[0] = 0x3E;
            pattern[1] = 0x41;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x7A;
            break;

        case 'H':
            pattern[0] = 0x7F;
            pattern[1] = 0x08;
            pattern[2] = 0x08;
            pattern[3] = 0x08;
            pattern[4] = 0x7F;
            break;

        case 'I':
            pattern[0] = 0x00;
            pattern[1] = 0x41;
            pattern[2] = 0x7F;
            pattern[3] = 0x41;
            pattern[4] = 0x00;
            break;

        case 'J':
            pattern[0] = 0x20;
            pattern[1] = 0x40;
            pattern[2] = 0x41;
            pattern[3] = 0x3F;
            pattern[4] = 0x01;
            break;

        case 'K':
            pattern[0] = 0x7F;
            pattern[1] = 0x08;
            pattern[2] = 0x14;
            pattern[3] = 0x22;
            pattern[4] = 0x41;
            break;

        case 'L':
            pattern[0] = 0x7F;
            pattern[1] = 0x40;
            pattern[2] = 0x40;
            pattern[3] = 0x40;
            pattern[4] = 0x40;
            break;

        case 'M':
            pattern[0] = 0x7F;
            pattern[1] = 0x02;
            pattern[2] = 0x0C;
            pattern[3] = 0x02;
            pattern[4] = 0x7F;
            break;

        case 'N':
            pattern[0] = 0x7F;
            pattern[1] = 0x04;
            pattern[2] = 0x08;
            pattern[3] = 0x10;
            pattern[4] = 0x7F;
            break;

        case 'O':
            pattern[0] = 0x3E;
            pattern[1] = 0x41;
            pattern[2] = 0x41;
            pattern[3] = 0x41;
            pattern[4] = 0x3E;
            break;

        case 'P':
            pattern[0] = 0x7F;
            pattern[1] = 0x09;
            pattern[2] = 0x09;
            pattern[3] = 0x09;
            pattern[4] = 0x06;
            break;

        case 'Q':
            pattern[0] = 0x3E;
            pattern[1] = 0x41;
            pattern[2] = 0x51;
            pattern[3] = 0x21;
            pattern[4] = 0x5E;
            break;

        case 'R':
            pattern[0] = 0x7F;
            pattern[1] = 0x09;
            pattern[2] = 0x19;
            pattern[3] = 0x29;
            pattern[4] = 0x46;
            break;

        case 'S':
            pattern[0] = 0x46;
            pattern[1] = 0x49;
            pattern[2] = 0x49;
            pattern[3] = 0x49;
            pattern[4] = 0x31;
            break;

        case 'T':
            pattern[0] = 0x01;
            pattern[1] = 0x01;
            pattern[2] = 0x7F;
            pattern[3] = 0x01;
            pattern[4] = 0x01;
            break;

        case 'U':
            pattern[0] = 0x3F;
            pattern[1] = 0x40;
            pattern[2] = 0x40;
            pattern[3] = 0x40;
            pattern[4] = 0x3F;
            break;

        case 'V':
            pattern[0] = 0x1F;
            pattern[1] = 0x20;
            pattern[2] = 0x40;
            pattern[3] = 0x20;
            pattern[4] = 0x1F;
            break;

        case 'W':
            pattern[0] = 0x7F;
            pattern[1] = 0x20;
            pattern[2] = 0x18;
            pattern[3] = 0x20;
            pattern[4] = 0x7F;
            break;

        case 'X':
            pattern[0] = 0x63;
            pattern[1] = 0x14;
            pattern[2] = 0x08;
            pattern[3] = 0x14;
            pattern[4] = 0x63;
            break;

        case 'Y':
            pattern[0] = 0x07;
            pattern[1] = 0x08;
            pattern[2] = 0x70;
            pattern[3] = 0x08;
            pattern[4] = 0x07;
            break;

        case 'Z':
            pattern[0] = 0x61;
            pattern[1] = 0x51;
            pattern[2] = 0x49;
            pattern[3] = 0x45;
            pattern[4] = 0x43;
            break;


        /* Symbols */

        case ':':
            pattern[0] = 0x00;
            pattern[1] = 0x36;
            pattern[2] = 0x36;
            pattern[3] = 0x00;
            pattern[4] = 0x00;
            break;

        case '-':
            pattern[0] = 0x08;
            pattern[1] = 0x08;
            pattern[2] = 0x08;
            pattern[3] = 0x08;
            pattern[4] = 0x08;
            break;

        case '.':
            pattern[0] = 0x00;
            pattern[1] = 0x60;
            pattern[2] = 0x60;
            pattern[3] = 0x00;
            pattern[4] = 0x00;
            break;

        case ' ':
        default:
            pattern[0] = 0x00;
            pattern[1] = 0x00;
            pattern[2] = 0x00;
            pattern[3] = 0x00;
            pattern[4] = 0x00;
            break;
    }
}


/* =========================================================
   OLED Set Cursor
   row = 0..7
   col = 0..20
   ========================================================= */

void OLED_SetCursor(u8 row, u8 col)
{
    u8 x;

    x = col * 6;

    OLED_WriteCommand(0xB0 + row);

    OLED_WriteCommand(0x00 | (x & 0x0F));
    OLED_WriteCommand(0x10 | ((x >> 4) & 0x0F));
}


/* =========================================================
   Write Character
   ========================================================= */

void OLED_WriteChar(u8 data)
{
    u8 pattern[5];
    u8 i;

    OLED_GetCharPattern((char)data, pattern);

    for(i = 0; i < 5; i++)
    {
        OLED_WriteData(pattern[i]);
    }

    /* Space between characters */
    OLED_WriteData(0x00);
}


/* =========================================================
   Write String
   ========================================================= */

void OLED_WriteString(const char *str)
{
    while(*str != '\0')
    {
        OLED_WriteChar(*str);
        str++;
    }
}


/* =========================================================
   Clear OLED
   ========================================================= */

void OLED_Clear(void)
{
    u8 page;
    u8 column;

    for(page = 0; page < 8; page++)
    {
        OLED_WriteCommand(0xB0 + page);

        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);

        for(column = 0; column < 128; column++)
        {
            OLED_WriteData(0x00);
        }
    }
}


/* =========================================================
   OLED Initialization
   ========================================================= */

void OLED_Init(void)
{
    _delay_ms(100);

    OLED_WriteCommand(0xAE); /* Display OFF */

    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0x20);
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0xA1);

    OLED_WriteCommand(0xC8);

    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0x7F);

    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0xA4);

    OLED_WriteCommand(0xA6);

    OLED_WriteCommand(0xAF); /* Display ON */

    OLED_Clear();

    OLED_SetCursor(0, 0);
    OLED_WriteString("PREPAID KIOSK");
}