#include "lcd.h"
#include <util/delay.h>


static void LCD_PulseEnable(void)
{
    LCD_EN_PORT |= (1 << LCD_EN_PIN);
    _delay_us(1);

    LCD_EN_PORT &= ~(1 << LCD_EN_PIN);
    _delay_us(100);
}


static void LCD_SendNibble(uint8_t data)
{
    /* D4 */
    if (data & 0x01)
        LCD_D4_PORT |= (1 << LCD_D4_PIN);
    else
        LCD_D4_PORT &= ~(1 << LCD_D4_PIN);

    /* D5 */
    if (data & 0x02)
        LCD_D5_PORT |= (1 << LCD_D5_PIN);
    else
        LCD_D5_PORT &= ~(1 << LCD_D5_PIN);

    /* D6 */
    if (data & 0x04)
        LCD_D6_PORT |= (1 << LCD_D6_PIN);
    else
        LCD_D6_PORT &= ~(1 << LCD_D6_PIN);

    /* D7 */
    if (data & 0x08)
        LCD_D7_PORT |= (1 << LCD_D7_PIN);
    else
        LCD_D7_PORT &= ~(1 << LCD_D7_PIN);

    LCD_PulseEnable();
}


static void LCD_SendByte(uint8_t data)
{
    LCD_SendNibble(data >> 4);
    LCD_SendNibble(data & 0x0F);
}


void LCD_Command(uint8_t cmd)
{
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);

    LCD_SendByte(cmd);

    if (cmd == 0x01 || cmd == 0x02)
        _delay_ms(2);
}


void LCD_Data(uint8_t data)
{
    LCD_RS_PORT |= (1 << LCD_RS_PIN);

    LCD_SendByte(data);
}


void LCD_Init(void)
{
    /* Configure pins as outputs */
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_EN_DDR |= (1 << LCD_EN_PIN);

    LCD_D4_DDR |= (1 << LCD_D4_PIN);
    LCD_D5_DDR |= (1 << LCD_D5_PIN);
    LCD_D6_DDR |= (1 << LCD_D6_PIN);
    LCD_D7_DDR |= (1 << LCD_D7_PIN);

    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    LCD_EN_PORT &= ~(1 << LCD_EN_PIN);

    _delay_ms(20);

    /*
     * HD44780 initialization sequence
     * Start in 8-bit mode, then switch to 4-bit.
     */
    LCD_SendNibble(0x03);
    _delay_ms(5);

    LCD_SendNibble(0x03);
    _delay_us(150);

    LCD_SendNibble(0x03);
    _delay_us(150);

    LCD_SendNibble(0x02);

    /* 4-bit, 2 lines, 5x8 font */
    LCD_Command(0x28);

    /* Display OFF */
    LCD_Command(0x08);

    /* Clear display */
    LCD_Command(0x01);
    _delay_ms(2);

    /* Entry mode */
    LCD_Command(0x06);

    /* Display ON, cursor OFF, blink OFF */
    LCD_Command(0x0C);
}


void LCD_Clear(void)
{
    LCD_Command(0x01);
    _delay_ms(2);
}


void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0)
        address = 0x00 + col;
    else
        address = 0x40 + col;

    LCD_Command(0x80 | address);
}


void LCD_Print(const char *str)
{
    while (*str)
    {
        LCD_Data(*str++);
    }
}


void LCD_PrintNumber(int number)
{
    char buffer[12];

    /* Simple integer conversion */
    int i = 0;
    int j;
    char temp;

    if (number == 0)
    {
        LCD_Data('0');
        return;
    }

    if (number < 0)
    {
        LCD_Data('-');
        number = -number;
    }

    while (number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }

    for (j = 0; j < i / 2; j++)
    {
        temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    for (j = 0; j < i; j++)
        LCD_Data(buffer[j]);
}
