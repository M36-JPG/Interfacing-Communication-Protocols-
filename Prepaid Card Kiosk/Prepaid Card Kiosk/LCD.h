/*
 * LCD.h
 *
 * Created: 9/30/2026 8:03:44 PM
 *  Author: S
 */ 


#ifndef LCD_H_
#define LCD_H_




#include "STD_TYPES.h"

void LCD_Init(void);

void LCD_Command(u8 command);

void LCD_Char(u8 data);

void LCD_String(const char *str);

void LCD_Clear(void);

void LCD_Goto(u8 row,
u8 column);




#endif /* LCD_H_ */