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


/* =========================================================
   GPIO
   ========================================================= */

#define HEATER_PIN          PB0
#define FAN_PIN             PB1

#define WARNING_LED_PIN     PD4
#define BUZZER_PIN          PD5


/* =========================================================
   EEPROM
   ========================================================= */

#define EEPROM_MIN_ADDR     0x0000
#define EEPROM_MAX_ADDR     0x0001
#define EEPROM_MAGIC_ADDR   0x0002

#define EEPROM_MAGIC        0xA5


/* =========================================================
   Temperature Settings
   ========================================================= */

#define DEFAULT_MIN_TEMP    18
#define DEFAULT_MAX_TEMP    28

#define HYSTERESIS          1
#define HIGH_TEMP           35


/* =========================================================
   SPI Frame
   ========================================================= */

#define SPI_START           0xAA

#define SPI_WAIT_START      0
#define SPI_READ_DIP        1
#define SPI_READ_B1         2
#define SPI_READ_B2         3


/* =========================================================
   Global Variables
   ========================================================= */

uint8_t min_temp;
uint8_t max_temp;

uint8_t sensor_error_count = 0;
uint8_t high_temp_warning = 0;


/* =========================================================
   SPI Panel Data
   ========================================================= */

uint8_t panel_dip = 0;
uint8_t panel_b1 = 1;
uint8_t panel_b2 = 1;

uint8_t spi_state = SPI_WAIT_START;


/* =========================================================
   Load Settings From EEPROM
   ========================================================= */

void EEPROM_init(void)
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

/* =========================================================
   Greenhouse Control
   ========================================================= */

void Greenhouse_Control(uint8_t temperature)
{
    /* -----------------------------------------------------
       Heater
       ----------------------------------------------------- */
	if(temperature < min_temp){
		SET_BIT(DDRB, PB0);
		SET_BIT(DDRD,PD5);
	}
	else if (temperature >= min_temp + HYSTERESIS){
		CLR_BIT(DDRB , HEATER_PIN);
	}

    /* -----------------------------------------------------
       Fan
       ----------------------------------------------------- */
	if(temperature > max_temp){
		SET_BIT(DDRB,PB1);
		SET_BIT(DDRD,PD5);
	}
	else if (temperature <=  max_temp - HYSTERESIS){
		CLR_BIT(DDRB , FAN_PIN);
	}
}


/* =========================================================
   Read Temperature
   ========================================================= */

uint8_t Read_Temperature(uint8_t *temperature)
{
	uint8_t data;

	data = LM75_ReadTemperature();

	*temperature = data;

	return temperature;
}


/* =========================================================
   High Temperature Warning
   ========================================================= */

void High_Temperature_Warning(uint8_t temperature)
{
   SET_BIT(DDRB,PB1);
    SET_BIT(PORTD, PD5); // buzzer

 	SET_BIT(PORTB,FAN_PIN);
	CLR_BIT(PORTB,HEATER_PIN);
	LCD_SetCursor(1,0);
	LCD_String("FAN_ON , _HEATER_OFF");
 
}

void YELLOW_Temperature_Warning(uint8_t temperature)
{
	
	SET_BIT(PORTD, PD4); // yellow
LCD_SetCursor(1,0);
LCD_String("FAN_OFF , _HEATER_OFF");
	
}

/* =========================================================
   UART Command Parser
   ========================================================= */

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
/* =========================================================
   Display Temperature
   ========================================================= */

void Display_Temperature(uint8_t temperature)
{


    /* -----------------------------------------------------
       Line 1  temp = 17c
       ----------------------------------------------------- */
	LCD_Clear();
	LCD_SetCursor(0,0);
	LCD_String("TEMP:");
	LCD_Number(temperature);
    /* -----------------------------------------------------
       Line 2
	   fan off heater off buzzer off
       ----------------------------------------------------- */
	LCD_SetCursor(1,0);
	LCD_String("HEATER OFF BUZZER OFF FAN OFF");

}

void LOW_Temperature_Warning()
{
	SET_BIT(DDRB,PB0);
		SET_BIT(PORTD, PD5); // buzzer
		SET_BIT(PORTB,PB0); //heaterPORTD
		CLR_BIT(PORTB,FAN_PIN);
		LCD_SetCursor(1,0);
			LCD_String("FAOFF , _hR_ON");
}

/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
	uint8_t temperature;

	/* GPIO Initialization */
	SET_BIT(DDRB, HEATER_PIN);
	SET_BIT(DDRB, FAN_PIN);
	SET_BIT(DDRD, WARNING_LED_PIN);
	SET_BIT(DDRD, BUZZER_PIN);

	/* Initialize Drivers */
	UART_Init(9600);
	SPI_voidMaster_Init();
	I2C_Master_Init();
	LCD_Init();

	/* Load Temperature Settings */
		
		
	while (1)
	{
		LCD_SetCursor(0,0);
		LCD_String("TEMP:");
		LCD_SetCursor(0,6);
		LCD_Number(LM75_ReadTemperature());
		_delay_ms(500);
		LCD_Clear();
		if(Read_Temperature(LM75_ReadTemperature()) <= 20)
		{
			LOW_Temperature_Warning(LM75_ReadTemperature());
		}
		if(Read_Temperature(LM75_ReadTemperature()) >= 30)
		{
			High_Temperature_Warning(LM75_ReadTemperature());
		}
		if(Read_Temperature(LM75_ReadTemperature())  >=20 &&  Read_Temperature(LM75_ReadTemperature()) <=30)
		{
			YELLOW_Temperature_Warning( LM75_ReadTemperature());
		}
		
		
		
	}
	return 0; // HEATER PB0 FAN PB1
}
