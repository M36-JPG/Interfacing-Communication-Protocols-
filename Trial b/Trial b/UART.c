
#include <avr/io.h>
#include "uart.h"

#define F_CPU 1000000
#include <util/delay.h>

void UART_Init(uint32_t baudrate)
{
	uint16_t ubrr;

	/* RX = PD0 Input */
	DDRD &= ~(1 << PD0);

	/* TX = PD1 Output */
	DDRD |= (1 << PD1);

	/* Double Speed Mode */
	UCSR0A = (1 << U2X0);

	/* Baud calculation for U2X mode */
	ubrr = (F_CPU / (8UL * baudrate)) - 1;

	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	/* Enable RX and TX */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* 8-bit, No parity, 1 stop bit */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void UART_SendChar(char data)
{
	/* Wait until transmit buffer is empty */
	while (!(UCSR0A & (1 << UDRE0)));
//	_delay_ms(5);

	UDR0 = data;
	//_delay_ms(5);

}

void UART_SendString(const char *str)
{
	while (*str)
	{
		UART_SendChar(*str);
		str++;
	}
}

char UART_ReceiveChar(void)
{
	/* Wait until data is received */
	while (!(UCSR0A & (1 << RXC0)));
//	_delay_ms(5);

	return UDR0;
}
void UART_SendHex(uint8_t data)
{
	const char hex[] = "0123456789ABCDEF";

	UART_SendChar(hex[(data >> 4) & 0x0F]);
	UART_SendChar(hex[data & 0x0F]);
}
void UART_ReceiveString(char *str)
{
	char data;
	uint8_t index = 0;

	while (1)
	{
		data = UART_ReceiveChar();

		/* End of string */
		if (data == '\n')
		{
			str[index] = '\0';
			break;
		}

		/* Ignore carriage return */
		if (data == '\r')
		{
			continue;
		}

		str[index] = data;
		index++;
	}
}
uint8_t UART_DataAvailable(void)
{
	return (UCSR0A & (1 << RXC0));
}