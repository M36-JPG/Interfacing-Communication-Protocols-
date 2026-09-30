/*
 * UART.c
 *
 * Created: 9/30/2026 8:00:04 PM
 *  Author: S
 */ 
#include <avr/io.h>

#include "UART.h"

void UART_Init(u16 baudRate)
{
    u16 ubrr;

    ubrr = (u16)
           ((16000000UL /
           (16UL * baudRate)) - 1UL);

    /*
        Baud Rate
    */

    UBRR0H = (u8)(ubrr >> 8);

    UBRR0L = (u8)ubrr;

    /*
        Enable TX + RX
    */

    UCSR0A = 0;

    UCSR0B =
        (1 << RXEN0) |
        (1 << TXEN0);

    /*
        8-bit
        No parity
        1 stop bit
    */

    UCSR0C =
        (1 << UCSZ01) |
        (1 << UCSZ00);
}

void UART_Send(u8 data)
{
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}

u8 UART_Receive(void)
{
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

void UART_SendString(const char *str)
{
    while(*str)
    {
        UART_Send(*str);
        str++;
    }
}

u8 UART_Available(void)
{
    if(UCSR0A & (1 << RXC0))
    {
        return TRUE;
    }

    return FALSE;
}