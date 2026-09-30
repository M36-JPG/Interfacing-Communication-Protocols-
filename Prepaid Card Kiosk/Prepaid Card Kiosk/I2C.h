/*
 * I2C.h
 *
 * Created: 9/30/2026 7:58:24 PM
 *  Author: S
 */ 


#ifndef I2C_H_
#define I2C_H_




#include "STD_TYPES.h"

#define I2C_WRITE  0
#define I2C_READ   1

void I2C_Init(void);

u8 I2C_Start(u8 address);

u8 I2C_Write(u8 data);

u8 I2C_ReadACK(void);

u8 I2C_ReadNACK(void);

void I2C_Stop(void);




#endif /* I2C_H_ */