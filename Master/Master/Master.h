/*
 * Master.h
 *
 * Created: 9/21/2026 9:05:57 AM
 *  Author: Sama.Adel
 */ 


#ifndef MASTER_H_
#define MASTER_H_
#include "BIT_MATH.h"
#include "STD_TYPE.h"

void SPI_Master_Int(void);
void SPI_Slave_Int(void);
u8 SPI_Transmite(u8 data);
u8 SPI_Resiver(void);


#endif /* MASTER_H_ */