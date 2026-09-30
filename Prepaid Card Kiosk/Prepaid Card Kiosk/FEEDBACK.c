#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "FEEDBACK.h"

#include <avr/io.h>
#include <util/delay.h>


/* ================= GPIO Definitions ================= */

#define FEEDBACK_DDR     DDRC
#define FEEDBACK_PORT    PORTC


/* ================= Initialization ================= */

void Feedback_Init(void)
{
    /* PC0 -> Green LED
       PC1 -> Red LED
       PC2 -> Yellow LED
       PC3 -> Buzzer
    */

    FEEDBACK_DDR |= (1 << GREEN_LED_PIN)
                  | (1 << RED_LED_PIN)
                  | (1 << YELLOW_LED_PIN)
                  | (1 << BUZZER_PIN);

    /* Turn everything OFF */
    FEEDBACK_PORT &= ~((1 << GREEN_LED_PIN)
                     | (1 << RED_LED_PIN)
                     | (1 << YELLOW_LED_PIN)
                     | (1 << BUZZER_PIN));
}


/* ================= LEDs ================= */

void Feedback_OK(void)
{
    /* Turn OFF all LEDs first */
    FEEDBACK_PORT &= ~((1 << GREEN_LED_PIN)
                     | (1 << RED_LED_PIN)
                     | (1 << YELLOW_LED_PIN));

    /* Green LED ON */
    FEEDBACK_PORT |= (1 << GREEN_LED_PIN);

    /* One short beep */
    Buzzer_BeepShort();
}


void Feedback_Denied(void)
{
    /* Turn OFF all LEDs first */
    FEEDBACK_PORT &= ~((1 << GREEN_LED_PIN)
                     | (1 << RED_LED_PIN)
                     | (1 << YELLOW_LED_PIN));

    /* Red LED ON */
    FEEDBACK_PORT |= (1 << RED_LED_PIN);

    /* One long beep */
    Buzzer_BeepLong();
}


void Feedback_Warning(void)
{
    /* Turn OFF all LEDs first */
    FEEDBACK_PORT &= ~((1 << GREEN_LED_PIN)
                     | (1 << RED_LED_PIN)
                     | (1 << YELLOW_LED_PIN));

    /* Yellow LED ON */
    FEEDBACK_PORT |= (1 << YELLOW_LED_PIN);
}


/* ================= Buzzer ================= */

void Buzzer_BeepShort(void)
{
    FEEDBACK_PORT |= (1 << BUZZER_PIN);

    _delay_ms(100);

    FEEDBACK_PORT &= ~(1 << BUZZER_PIN);
}


void Buzzer_BeepLong(void)
{
    FEEDBACK_PORT |= (1 << BUZZER_PIN);

    _delay_ms(700);

    FEEDBACK_PORT &= ~(1 << BUZZER_PIN);
}