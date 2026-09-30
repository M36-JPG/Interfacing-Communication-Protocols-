/*
 * Prepaid Card Kiosk.c
 *
 * Created: 9/30/2026 7:53:17 PM
 * Author : S
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL

#include "Kiosk.h"

int main(void)
{
	Kiosk_Init();

	while(1)
	{
		Kiosk_Run();
	}

	return 0;
}

