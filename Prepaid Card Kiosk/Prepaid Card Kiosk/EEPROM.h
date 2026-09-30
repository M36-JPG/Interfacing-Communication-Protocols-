/*
 * EEPROM.h
 *
 * Created: 9/30/2026 8:00:38 PM
 *  Author: S
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_



#include "STD_TYPES.h"

#define EEPROM_ADDRESS  0x50

u8 EEPROM_WriteByte(u16 address,
u8 data);

u8 EEPROM_ReadByte(u16 address);

u8 EEPROM_WriteBlock(u16 address,
const u8 *data,
u16 length);

u8 EEPROM_ReadBlock(u16 address,
u8 *data,
u16 length);




#endif /* EEPROM_H_ */