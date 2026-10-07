#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;

} Clock_Time;


void Clock_Init(void);

void Clock_Tick(void);

Clock_Time Clock_GetTime(void);

#endif