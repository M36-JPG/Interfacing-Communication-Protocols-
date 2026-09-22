#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/*
 * Change these pins for your microcontroller.
 *
 * Example below:
 * LCD_RS -> PORTB0
 * LCD_EN -> PORTB1
 * LCD_D4 -> PORTB2
 * LCD_D5 -> PORTB3
 * LCD_D6 -> PORTB4
 * LCD_D7 -> PORTB5
 */
#include <avr/io.h>
#define LCD_RS_PORT   PORTB
#define LCD_RS_DDR    DDRB
#define LCD_RS_PIN    PB0

#define LCD_EN_PORT   PORTB
#define LCD_EN_DDR    DDRB
#define LCD_EN_PIN    PB1

#define LCD_D4_PORT   PORTB
#define LCD_D4_DDR    DDRB
#define LCD_D4_PIN    PB2

#define LCD_D5_PORT   PORTB
#define LCD_D5_DDR    DDRB
#define LCD_D5_PIN    PB3

#define LCD_D6_PORT   PORTB
#define LCD_D6_DDR    DDRB
#define LCD_D6_PIN    PB4

#define LCD_D7_PORT   PORTB
#define LCD_D7_DDR    DDRB
#define LCD_D7_PIN    PB5


void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);
void LCD_PrintNumber(int number);

#endif
