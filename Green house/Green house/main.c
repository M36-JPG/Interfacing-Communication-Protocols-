#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "I2C.h"
#include "LCD.h"
#include "LM75.h"
#include "EEPROM.h"
#include "UART.h"
#include "SPI.h"
#include "BIT_MATH.h"
#define F_CPU 1000000

#define HEATER_PIN          PB0
#define FAN_PIN             PB1

#define WARNING_LED_PIN     PD4
#define BUZZER_PIN          PD5



#define EEPROM_MIN_ADDR     0x0000
#define EEPROM_MAX_ADDR     0x0001
#define EEPROM_MAGIC_ADDR   0x0002

#define EEPROM_MAGIC        0xA5

#define DEFAULT_MIN_TEMP    18
#define DEFAULT_MAX_TEMP    28

#define HYSTERESIS          1
#define HIGH_TEMP           35

#define SPI_START           0xAA

#define SPI_WAIT_START      0
#define SPI_READ_DIP        1
#define SPI_READ_B1         2
#define SPI_READ_B2         3

u8 min_temp = DEFAULT_MIN_TEMP;
u8 max_temp = DEFAULT_MAX_TEMP;

u8 sensor_error_count = 0;
u8 high_temp_warning = 0;

u8 panel_dip = 0;
u8 panel_b1 = 1;
u8 panel_b2 = 1;

u8 spi_state = SPI_WAIT_START;

void EEPROM_Init(void)
{
	min_temp = EEPROM_Read(EEPROM_MIN_ADDR);
	max_temp = EEPROM_Read(EEPROM_MAX_ADDR);

	if (min_temp == 0xFF || max_temp == 0xFF)
	{
		min_temp = DEFAULT_MIN_TEMP;
		max_temp = DEFAULT_MAX_TEMP;

		EEPROM_Write(EEPROM_MIN_ADDR, min_temp);
		EEPROM_Write(EEPROM_MAX_ADDR, max_temp);
	}
}

void Greenhouse_Control(u8 temperature)
{
    if(temperature < min_temp){
		SET_BIT(DDRB, HEATER_PIN);
	}
	else if (temperature >= min_temp + HYSTERESIS){
		CLR_BIT(DDRB , HEATER_PIN);
		_delay_ms(1000);
	}

	if(temperature > max_temp){
		SET_BIT(DDRB,FAN_PIN);
		
	}
	else if (temperature <=  max_temp - HYSTERESIS){
		CLR_BIT(DDRB , FAN_PIN);
		_delay_ms(1000);
	}
}



u8 Read_Temperature(u8 *temperature)
{
	u8 data;

	data = LM75_ReadTemperature();

	*temperature = data;

	return temperature;
}

void High_Temperature_Warning(u8 temperature)
{
	if(high_temp_warning == 1){
		if(sensor_error_count == 1){
			CLR_BIT(DDRB , FAN_PIN);
			CLR_BIT(DDRB , HEATER_PIN);
			SET_BIT(DDRD, BUZZER_PIN);
			_delay_ms(300);
			CLR_BIT(DDRB , BUZZER_PIN);
		}
	}
	
	if(sensor_error_count >= 3){
		CLR_BIT(DDRB , FAN_PIN);
		CLR_BIT(DDRB , HEATER_PIN);
		CLR_BIT(DDRB , BUZZER_PIN);
	    YELLOW_Temperature_Warning(temperature);
	}
	sensor_error_count = 0;
	LCD_SetCursor(1,0);
	LCD_Clear();
	LCD_String("Sensor Fault");
	_delay_ms(5000);
	LCD_Clear();
 
}

void YELLOW_Temperature_Warning(u8 temperature)
{
	
	SET_BIT(DDRD, WARNING_LED_PIN);
	LCD_Clear(); 
    LCD_SetCursor(1,0);
   // LCD_String("FAN_OFF , _HEATER_OFF");
	
}

void Parse_Command(char data)
{
	if (data == 'M')
	{
		min_temp = EEPROM_Read(EEPROM_MIN_ADDR);
	}
	else if (data == 'X')
	{
		max_temp = EEPROM_Read(EEPROM_MAX_ADDR);
	}
}

void Display_Temperature(u8 temperature)
{
	LCD_Clear();
	LCD_SetCursor(0,0);
	LCD_String("TEMP:");
	LCD_SetCursor(0,6);
	LCD_Number(LM75_ReadTemperature());
	_delay_ms(500);
	LCD_Clear();
}

void LOW_Temperature_Warning()
{
	    SET_BIT(DDRB,HEATER_PIN);
		//SET_BIT(DDRD, BUZZER_PIN); 
		CLR_BIT(DDRB,FAN_PIN);
		LCD_Clear();
		//LCD_SetCursor(1,0);
	    //LCD_String("FAN_OFF , _HR_ON");
}





int main(void)
{
	u8 temperature;
DDRB = (1 << FAN_PIN) | (1 << HEATER_PIN) ;
DDRD = (1 << WARNING_LED_PIN) | (1 << BUZZER_PIN);
PORTB = (1 << FAN_PIN) | (1 << HEATER_PIN);
PORTD = (1 << WARNING_LED_PIN) | (1 << BUZZER_PIN);



	UART_Init(9600);
	SPI_voidSlave_Init();
	I2C_Master_Init();
	LCD_Init();

    EEPROM_Init();	
	while (1)
	{
		temperature = LM75_ReadTemperature();
		Display_Temperature(temperature);
		 if(temperature <= min_temp){
			 SET_BIT(DDRB, HEATER_PIN);
// 			 CLR_BIT(DDRB , FAN_PIN);
// 			 CLR_BIT(DDRD , WARNING_LED_PIN);
// 			 CLR_BIT(DDRD , BUZZER_PIN);
		 }
		 else if (temperature >= min_temp + HYSTERESIS){
			 CLR_BIT(DDRB , HEATER_PIN);
// 			  CLR_BIT(DDRB , FAN_PIN);
			  CLR_BIT(DDRD , WARNING_LED_PIN);
			  CLR_BIT(DDRD , BUZZER_PIN);
		 }

		 if(temperature >= max_temp){
			 SET_BIT(DDRB,FAN_PIN);
// 			 CLR_BIT(DDRB , HEATER_PIN);
			  CLR_BIT(DDRD , WARNING_LED_PIN);
 			  CLR_BIT(DDRD , BUZZER_PIN);
		 }
		 else if (temperature <=  max_temp - HYSTERESIS){
			 CLR_BIT(DDRB , FAN_PIN);
// 			 CLR_BIT(DDRB , HEATER_PIN);
		 CLR_BIT(DDRD , WARNING_LED_PIN);

if(temperature >= min_temp || temperature <= max_temp){
	SET_BIT(DDRD , WARNING_LED_PIN);
				
	}
	
	if(High_Temperature_Warning(temperature) == 1 ){
		High_Temperature_Warning(temperature);
	}
	return 0; 
}
