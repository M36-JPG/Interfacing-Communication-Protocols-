/*
 * OLED.h
 *
 * Created: 9/30/2026 8:38:36 PM
 *  Author: S
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_ADDRESS 0x3C

#include "STD_TYPES.h"

/* OLED I2C Address */
#define OLED_ADDRESS    0x3C

/* OLED size */
#define OLED_WIDTH      128
#define OLED_HEIGHT     64

void OLED_Init(void);
void OLED_Clear(void);

void OLED_SetCursor(u8 row, u8 col);
void OLED_WriteChar(u8 data);
void OLED_WriteString(const char *str);




#endif /* OLED_H_ */