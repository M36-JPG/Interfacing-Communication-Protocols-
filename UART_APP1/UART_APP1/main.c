// /*
//  * UART_APP1.c
//  *
//  * Created: 9/21/2026 2:02:04 PM
//  * Author : Eman.Assem
//  */ 
// 
// #define F_CPU 16000000
// 
// #include <avr/io.h>
// #include "STD_TYPES.h"
// #include "BIT_MATH.h"
// #include "UART.h"
// 
// 
// 
// 
// int main(void)
// {
// 	u8 data=0;
// 	UART_init(9600);
// 	SET_BIT(DDRC,0);// led on
// 
// 	UART_Send(6);
//     /* Replace with your application code */
//     while (1) 
//     {
// 		data=UART_Recieve();
// 		if(data== 1)
// 		SET_BIT(PORTC,0);// led on
// 
//     }
// }
// 
#define F_CPU 16000000

#include <avr/io.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "UART.h"
#include "LCD.h"


int main(void)
{
	u8 data = 0;

	//UART_init(9600);

	LCD_Init();
		LCD_Print("Hello Sama");


	while (1)
	{
		//data = UART_Recieve();

		//LCD_WriteString(data);
	}


	return 0;
}