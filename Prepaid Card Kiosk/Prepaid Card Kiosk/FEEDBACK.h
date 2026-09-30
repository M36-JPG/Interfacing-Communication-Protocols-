#ifndef FEEDBACK_H_
#define FEEDBACK_H_

#include "STD_TYPES.h"

/* LED Pins */
#define GREEN_LED_PIN    0
#define RED_LED_PIN      1
#define YELLOW_LED_PIN   2

/* Buzzer Pin */
#define BUZZER_PIN       3

/* Functions */
void Feedback_Init(void);

void Feedback_OK(void);
void Feedback_Denied(void);
void Feedback_Warning(void);

void Buzzer_BeepShort(void);
void Buzzer_BeepLong(void);

#endif /* FEEDBACK_H_ */