/*
 * LCD.h
 *
 * Created: 9/22/2026 10:56:38 AM
 *  Author: Sama.Adel
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "STD_TYPES.h"

#define LCD_RS     PB0
#define LCD_EN     PB1
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR  DDRB

/* Data port */
#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR  DDRB

/* Commands */
#define LCD_CLEAR        0x01
#define LCD_HOME         0x02
#define LCD_ENTRY_MODE   0x06
#define LCD_ON           0x0C
#define LCD_8BIT_MODE    0x38
void LCD_Init(void);

void LCD_WriteCommand(u8 command);

void LCD_WriteData(u8 data);

void LCD_WriteChar(u8 data);

void LCD_WriteString(u8 *str);

void LCD_Clear(void);

void LCD_SetCursor(u8 row, u8 col);




#endif /* LCD_H_ */