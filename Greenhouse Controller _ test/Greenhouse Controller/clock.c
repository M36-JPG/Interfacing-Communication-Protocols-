#include "CLOCK.h"

static Clock_Time time;


void Clock_Init(void)
{
	time.hours = 0;
	time.minutes = 0;
	time.seconds = 0;
}


void Clock_Tick(void)
{
	time.seconds++;

	if (time.seconds >= 60)
	{
		time.seconds = 0;

		time.minutes++;

		if (time.minutes >= 60)
		{
			time.minutes = 0;

			time.hours++;

			if (time.hours >= 24)
			{
				time.hours = 0;
			}
		}
	}
}


Clock_Time Clock_GetTime(void)
{
	return time;
}