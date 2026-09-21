/*
 * Master.c
 *
 * Created: 9/20/2026 3:06:55 PM
 *  Author: Mariam.Kotb
 */ 
#include<avr/io.h>
#include"Master.h"

#define MISO PB4
#define MOSI PB3
#define SCK PB5
#define SS PB2

void SPI_Master_Init(void){
	//SPI ENABLE
	SET_BIT(SPCR , SPE);
	//DATA ORDER
	SET_BIT(SPCR , DORD);
	//MASTER SELECT
	SET_BIT(SPCR , MSTR);
	//CLOCK POLARITY & CLOCK PHASE
	CLR_BIT(SPCR , CPOL);
	SET_BIT(SPCR , CPHA);
	//CLOCK
	CLR_BIT(SPCR , SPR1);
	SET_BIT(SPCR , SPR0);
	CLR_BIT(SPSR , SPI2X);
	//MOSI
	SET_BIT(DDRB , PB3);
	//MISO
	CLR_BIT(DDRB , PB4);
	//SCK
	SET_BIT(DDRB , PB5);
	//SS
	SET_BIT(DDRB , PB2);
	
}

void SPI_Transmit(u8 data){
	SPDR = data;
	while(!(GET_BIT(SPSR , SPIF))){
		
	}
}