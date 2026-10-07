
#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

/* EEPROM I2C Address */



void EEPROM_Write(uint16_t address, uint8_t data);

uint8_t EEPROM_Read(uint16_t address);

#endif
