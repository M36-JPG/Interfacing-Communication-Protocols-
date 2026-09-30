/*
 * RTC.h
 *
 * Created: 9/30/2026 8:02:33 PM
 *  Author: S
 */ 


#ifndef RTC_H_
#define RTC_H_



#include "STD_TYPES.h"

typedef struct
{
	u8 sec;
	u8 min;
	u8 hour;

	u8 day;
	u8 month;
	u8 year;

} RTC_Time;

void RTC_Init(void);

u8 RTC_SetTime(const RTC_Time *time);

u8 RTC_GetTime(RTC_Time *time);

u8 RTC_IsPeak(const RTC_Time *time);

u8 RTC_BCDToDec(u8 value);

u8 RTC_DecToBCD(u8 value);





#endif /* RTC_H_ */