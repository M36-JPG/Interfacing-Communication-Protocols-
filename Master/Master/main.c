/*
 * Master.c
 *
 * Created: 9/21/2026 9:05:22 AM
 * Author : Sama.Adel
 */ 
#define  F_CPU 16000000UL
#include <avr/io.h>
#include "Master.h"

int main(void)
{
	SPI_Master_Int();
    /* Replace with your application code */
	
    while (1) 
    {
		SPI_Transmite('A');
    }
}

