/*
 * DIO.c
 *
 * Created: 9/30/2026 7:56:33 PM
 *  Author: S
 */ 

#include "DIO.h"
#include "BIT_MATH.h"

void DIO_SetPinDirection(volatile u8 *DDR,
u8 pin,
u8 direction)
{
	if(direction == DIO_OUTPUT)
	{
		SET_BIT(*DDR, pin);
	}
	else
	{
		CLR_BIT(*DDR, pin);
	}
}

void DIO_WritePin(volatile u8 *PORT,
u8 pin,
u8 value)
{
	if(value == DIO_HIGH)
	{
		SET_BIT(*PORT, pin);
	}
	else
	{
		CLR_BIT(*PORT, pin);
	}
}

u8 DIO_ReadPin(volatile u8 *PIN,
u8 pin)
{
	return GET_BIT(*PIN, pin);
}

void DIO_TogglePin(volatile u8 *PORT,
u8 pin)
{
	TOG_BIT(*PORT, pin);
}
