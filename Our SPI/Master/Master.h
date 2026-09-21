/*
 * Master.h
 *
 * Created: 9/20/2026 3:06:40 PM
 *  Author: Mariam.Kotb
 */ 


#ifndef MASTER_H_
#define MASTER_H_
#include "STD_TYPES.h"
#include "BIT_MATH.h"

void SPI_Master_Init(void);
void SPI_Transmit(u8 data);

void SPI_SendString(u8 *str);

#endif /* MASTER_H_ */