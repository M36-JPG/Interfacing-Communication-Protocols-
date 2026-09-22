// /*
// * UART_APP2.c
// *
// * Created: 9/21/2026 2:32:02 PM
// * Author : Eman.Assem
// */
// #define F_CPU 16000000
// 
// #include <avr/io.h>
// #include "STD_TYPES.h"
// #include "BIT_MATH.h"
// #include "UART.h"
// 
// 
// int main(void)
// {
// 	u8 data=0;
// 	SET_BIT(DDRD,3);  //led
// 	
// 
// 	UART_init(9600);
// 	
// 	/* Replace with your application code */
// 	while (1)
// 	{
// 		
// 		data=UART_Recieve();
// 		if(data == 6)
// 		SET_BIT(PORTD,3);// led on
// 		else
// 		UART_Send(1);
// 		
// 	}
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


	/* LED on PD3 */
	SET_BIT(DDRD, 3);


	UART_init(9600);

	LCD_Init();


	while (1)
	{
		data = UART_Recieve();


		if (data == 6)
		{
			/* LED ON */
			SET_BIT(PORTD, 3);

			/* Display on LCD */
			//LCD_Clear();
			LCD_WriteString("Sama");
		}
		else
		{
			/* Send 1 to APP1 */
			UART_Send(1);

			/* Display received data */
			//LCD_Clear();
			LCD_WriteString("Sama");
			LCD_SetCursor(1, 0);
			LCD_WriteChar(data);
		}
	}


	return 0;
}
