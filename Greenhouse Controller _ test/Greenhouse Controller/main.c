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

void Load_Settings(void)
{
    uint8_t magic;
	
	magic = EEPROM_Read( EEPROM_MAGIC_ADDR);

	min_temp = DEFAULT_MIN_TEMP;

	max_temp = DEFAULT_MAX_TEMP;

	EEPROM_Write(EEPROM_MIN_ADDR , min_temp);

	EEPROM_Write(EEPROM_MAX_ADDR , max_temp);

	EEPROM_Write(EEPROM_MAGIC_ADDR , EEPROM_MAGIC);

	
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

		   SET_BIT(PORTB , HEATER_PIN);
	   }

	else if (temperature >= (min_temp + HYSTERESIS )){

		CLR_BIT(PORTB , HEATER_PIN);
	}

    /* -----------------------------------------------------
       Fan
       ----------------------------------------------------- */
        if(temperature >  max_temp){

		   SET_BIT(PORTB , FAN_PIN);
	   }

	else if (temperature <= (max_temp - HYSTERESIS )){

		CLR_BIT(PORTB , FAN_PIN);
	}
}


/* =========================================================
   Read Temperature
   ========================================================= */

uint8_t Read_Temperature(uint8_t *temperature)
{
   
         
  
}


/* =========================================================
   High Temperature Warning
   ========================================================= */

void High_Temperature_Warning(uint8_t temperature)
{
    
}


/* =========================================================
   UART Command Parser
   ========================================================= */

void Parse_Command(char *data)
{
   

    /* -----------------------------------------------------
       SETMIN 20
       ----------------------------------------------------- */



    /* -----------------------------------------------------
       SETMAX 30
       ----------------------------------------------------- */

}

/* =========================================================
   SPI Receive Panel Frame
   ========================================================= */

void SPI_Receive_Panel(void)
{
    
    /*
     * Check if SPI transfer is complete
     */

  
        /*
         * Read received byte.
         * This clears the SPI flag.
         */

      


      
            /* =============================================
               Waiting for START
               ============================================= */

           
            /* =============================================
               Receive DIP
               ============================================= */

          

            /* =============================================
               Receive B1
               ============================================= */

           

            /* =============================================
               Receive B2
               ============================================= */

           
           

          
}


/* =========================================================
   Display Temperature
   ========================================================= */

void Display_Temperature(uint8_t temperature)
{
    LCD_Clear();


    /* -----------------------------------------------------
       Line 1  temp = 17c
	 
       ----------------------------------------------------- */
    LCD_SetCursor(0 , 0);
	LCD_String("TEMP : ");
	LCD_SetCursor(0 , 2);
	LCD_Number(17);
	LCD_Char('C');
	


    /* -----------------------------------------------------
       Line 2
	   fan off heater off buzzer off
       ----------------------------------------------------- */

   

}


/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
   

    /* =====================================================
       GPIO Initialization
       ===================================================== */

    /* Heater */
    


    /* Fan */



    /* Warning LED */



    /* Buzzer */



    /* =====================================================
       Initial Outputs OFF
       ===================================================== */

    

    /* =====================================================
       Driver Initialization
       ===================================================== */
        LCD_Init();
        UART_Init(9600);
        SPI_voidMaster_Init();
        I2C_Master_Init();
        


    /* =====================================================
       Load EEPROM Settings
       ===================================================== */

  

    /* =====================================================
       Main Loop
       ===================================================== */

    while (1)
    {
        /* =============================================
           1. SPI Panel
           ============================================= */



        /* =============================================
           2. Temperature
           ============================================= */

       

        /* =============================================
           3. UART
           ============================================= */
	}
        


    return 0;
}
