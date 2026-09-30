/*
 * DIO.h
 *
 * Created: 9/30/2026 7:55:44 PM
 *  Author: S
 */ 


#ifndef DIO_H_
#define DIO_H_



#include <avr/io.h>
#include "STD_TYPES.h"

#define DIO_INPUT   0
#define DIO_OUTPUT  1

#define DIO_LOW     0
#define DIO_HIGH    1

void DIO_SetPinDirection(volatile u8 *DDR,
u8 pin,
u8 direction);

void DIO_WritePin(volatile u8 *PORT,
u8 pin,
u8 value);

u8 DIO_ReadPin(volatile u8 *PIN,
u8 pin);

void DIO_TogglePin(volatile u8 *PORT,
u8 pin);






#endif /* DIO_H_ */