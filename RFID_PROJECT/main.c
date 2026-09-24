/*
 * RFID_PROJECT.c
 *
 * Created: 9/24/2026 10:21:45 AM
 * Author : Eman.Assem
 */ 


#define F_CPU 16000000UL //unsigned long

#include <avr/io.h>
#include <util/delay.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SPI.h"
#include "UART.h"
#include "RFID.h"

int main(void)
{
	RFID_Card card;
	uint8_t i;

	SPI_Init();
	UART_Init(9600);
	RFID_Init();

	UART_SendString("RFID System Started\r\n");

	while (1)
	{
		if (RFID_IsCardPresent() == RFID_OK)
		{
			if (RFID_ReadUID(&card) == RFID_OK)
			{
				UART_SendString("Card UID: ");

				for (i = 0; i < card.size; i++)
				{
					UART_SendHex(card.uid[i]);
					UART_SendChar(' ');
				}

				UART_SendString("\r\n");
			}
			else
			{
				UART_SendString("UID Read Error\r\n");
			}

			RFID_Halt();

			_delay_ms(500);
		}
	}
}
