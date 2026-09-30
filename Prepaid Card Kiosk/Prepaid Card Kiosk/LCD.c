/*
 * LCD.c
 *
 * Created: 9/30/2026 8:04:00 PM
 *  Author: S
 */ 
#include <util/delay.h>

#include "I2C.h"

#include "LCD.h"

#define LCD_ADDRESS 0x27

#define LCD_RS 0x01
#define LCD_EN 0x04
#define LCD_BL 0x08

static void LCD_Write4Bits(u8 data,
u8 rs)
{
	u8 value;

	value = data |
	LCD_BL;

	if(rs)
	{
		value |= LCD_RS;
	}

	I2C_Start((LCD_ADDRESS << 1) |
	I2C_WRITE);

	I2C_Write(value | LCD_EN);

	I2C_Write(value & ~LCD_EN);

	I2C_Stop();
}

static void LCD_Send(u8 data,
u8 rs)
{
	LCD_Write4Bits(data & 0xF0,
	rs);

	LCD_Write4Bits((data << 4) & 0xF0,
	rs);
}

void LCD_Init(void)
{
	_delay_ms(50);

	LCD_Write4Bits(0x30,0);

	_delay_ms(5);

	LCD_Write4Bits(0x30,0);

	_delay_us(150);

	LCD_Write4Bits(0x30,0);

	LCD_Write4Bits(0x20,0);

	LCD_Command(0x28);

	LCD_Command(0x0C);

	LCD_Command(0x06);

	LCD_Clear();
}

void LCD_Command(u8 command)
{
	LCD_Send(command,0);

	_delay_ms(2);
}

void LCD_Char(u8 data)
{
	LCD_Send(data,1);
}

void LCD_String(const char *str)
{
	while(*str)
	{
		LCD_Char(*str);

		str++;
	}
}

void LCD_Clear(void)
{
	LCD_Command(0x01);

	_delay_ms(2);
}

void LCD_Goto(u8 row,
u8 column)
{
	if(row == 0)
	{
		LCD_Command(0x80 + column);
	}
	else
	{
		LCD_Command(0xC0 + column);
	}
}