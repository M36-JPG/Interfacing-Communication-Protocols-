/*
 * SPI.h
 *
 * Created: 9/20/2026 12:10:39 PM
 *  Author: Mariam.Kotb
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "STD_TYPES.h"
#include "BIT_MATH.h"

void SPI_Master_Init(void);
void SPI_Slave_Init(void);

void SPI_Transmit(u8 data);
u8 SPI_Recieve(void);

void SPI_SendString(u8 *str);



#endif /* SPI_H_ */