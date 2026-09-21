/*
 * Master.c
 *
 * Created: 9/21/2026 9:06:53 AM
 *  Author: Sama.Adel
 */ 
#include <avr/io.h>
#include "Master.h"
void SPI_Master_Int(void){
	
	//mosi = masret output slave input //B3
	SET_BIT(DDRB , 3);
	//miso =masret input slave output //B4
	CLR_BIT(DDRB , 4);
	//SCK= clock //B5
	SET_BIT(DDRB , 5);
	//SS= select slave //B2
	SET_BIT(DDRB , 2); // output
	SET_BIT(PORTB , 2); //pull up
	
	//Reg
	//SPCR
	//• Bit 7 – SPIE: SPI Interrupt Enable 
	CLR_BIT(SPCR , 7);
	
	//• Bit 5 – DORD: Data Order //LSB
	SET_BIT(SPCR , 5);
	
	//• Bit 4 – MSTR: Master/Slave Select // MASTER
	SET_BIT(SPCR , 4);
	//• Bit 3 – CPOL: Clock Polarity // FALLING TO RAISING
	SET_BIT(SPCR , 3);
	
	//• Bit 2 – CPHA: Clock Phase //SETUP
	SET_BIT(SPCR , 2);
	
	//• Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0 //64
	
	CLR_BIT(SPCR ,0);
	SET_BIT(SPCR , 1);
	CLR_BIT(SPSR , 0);
	//• Bit 6 – SPE: SPI Enable
	SET_BIT(SPCR , 6);
	
}

void SPI_Slave_Int(void){
	
	//mosi = masret output slave input //B3
	CLR_BIT(DDRB , 3);
	//miso =masret input slave output //B4
	SET_BIT(DDRB , 4);
	//SCK= clock //B5
	CLR_BIT(DDRB , 5);
	//SS= select slave //B2
	CLR_BIT(DDRB , 2); // output
	CLR_BIT(PORTB , 2); //pull up
	
	//Reg
	//SPCR
	//• Bit 7 – SPIE: SPI Interrupt Enable
	CLR_BIT(SPCR , 7);
	
	//• Bit 5 – DORD: Data Order //LSB
	SET_BIT(SPCR , 5);
	
	//• Bit 4 – MSTR: Master/Slave Select // SLAVE
	CLR_BIT(SPCR , 4);
	//• Bit 3 – CPOL: Clock Polarity // FALLING TO RAISING
	SET_BIT(SPCR , 3);
	
	//• Bit 2 – CPHA: Clock Phase //SETUP
	SET_BIT(SPCR , 2);
	
	//• Bit 6 – SPE: SPI Enable
	SET_BIT(SPCR , 6);
	
}

u8 SPI_Transmite(u8 data){
	
	SPDR = data;
	while(GET_BIT(SPSR , 7)==0);
		return SPDR;
	}
u8 SPI_Resiver(void){
	while(GET_BIT(SPSR , 7)==0);
		return SPDR;
	
}




