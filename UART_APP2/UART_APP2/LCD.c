/*
 * LCD.c
 *
 * Created: 9/22/2026 10:56:52 AM
 *  Author: Sama.Adel
 */ 
#include <avr/io.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "LCD.h"


/* LCD Control Pins */
#define LCD_RS     0
#define LCD_EN     1

/* LCD Data Pins */
#define LCD_D4     2
#define LCD_D5     3
#define LCD_D6     4
#define LCD_D7     5


//Enable puls
static void LCD_Enable(void)
{
    SET_BIT(LCD_CTRL_PORT, LCD_EN);

    CLR_BIT(LCD_CTRL_PORT, LCD_EN);
}


//Write 4 Bits
static void LCD_Write4Bits(u8 data)
{
    /* D4 */
    if (GET_BIT(data, 0) == 1)
    {
        SET_BIT(LCD_DATA_PORT, LCD_D4);
    }
    else
    {
        CLR_BIT(LCD_DATA_PORT, LCD_D4);
    }


    /* D5 */
    if (GET_BIT(data, 1) == 1)
    {
        SET_BIT(LCD_DATA_PORT, LCD_D5);
    }
    else
    {
        CLR_BIT(LCD_DATA_PORT, LCD_D5);
    }


    /* D6 */
    if (GET_BIT(data, 2) == 1)
    {
        SET_BIT(LCD_DATA_PORT, LCD_D6);
    }
    else
    {
        CLR_BIT(LCD_DATA_PORT, LCD_D6);
    }


    /* D7 */
    if (GET_BIT(data, 3) == 1)
    {
        SET_BIT(LCD_DATA_PORT, LCD_D7);
    }
    else
    {
        CLR_BIT(LCD_DATA_PORT, LCD_D7);
    }


    /* Enable */
    LCD_Enable();
}


//LCD Initialization
void LCD_Init(void)
{
    /* RS output */
    SET_BIT(DDRB, LCD_RS);

    /* EN output */
    SET_BIT(DDRB, LCD_EN);

    /* D4-D7 output */
    SET_BIT(DDRB, LCD_D4);
    SET_BIT(DDRB, LCD_D5);
    SET_BIT(DDRB, LCD_D6);
    SET_BIT(DDRB, LCD_D7);


    /* RS = 0 */
    CLR_BIT(PORTB, LCD_RS);


    /* 4-bit initialization */

    LCD_Write4Bits(0x03);

    LCD_Write4Bits(0x03);

    LCD_Write4Bits(0x03);

    LCD_Write4Bits(0x02);


    /* 4-bit, 2 lines, 5x8 font */
    LCD_WriteCommand(0x28);


    /* Display ON, Cursor OFF */
    LCD_WriteCommand(0x0C);


    /* Clear Display */
    LCD_WriteCommand(0x01);


    /* Entry Mode */
    LCD_WriteCommand(0x06);
}


//Write Command
void LCD_WriteCommand(u8 command)
{
    /* RS = 0 */
    CLR_BIT(PORTB, LCD_RS);


    /* Higher Nibble */
    LCD_Write4Bits(command >> 4);


    /* Lower Nibble */
    LCD_Write4Bits(command & 0x0F);
}


//Write Data
void LCD_WriteData(u8 data)
{
    /* RS = 1 */
    SET_BIT(PORTB, LCD_RS);


    /* Higher Nibble */
    LCD_Write4Bits(data >> 4);


    /* Lower Nibble */
    LCD_Write4Bits(data & 0x0F);
}


//Write Character
void LCD_WriteChar(u8 data)
{
    LCD_WriteData(data);
}


//Write String
void LCD_WriteString(u8 *str)
{
    while (*str != '\0')
    {
        LCD_WriteData(*str);

        str++;
    }
}

//Clear LCD
void LCD_Clear(void)
{
    LCD_WriteCommand(0x01);
}


//Set Cursor
void LCD_SetCursor(u8 row, u8 col)
{
    u8 address;

    if (row == 0)
    {
        address = 0x80 + col;
    }
    else
    {
        address = 0xC0 + col;
    }

    LCD_WriteCommand(address);
}
