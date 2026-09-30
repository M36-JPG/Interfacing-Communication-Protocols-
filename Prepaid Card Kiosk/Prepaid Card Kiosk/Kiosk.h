/*
 * Kiosk.h
 *
 * Created: 9/30/2026 8:06:46 PM
 *  Author: S
 */ 


#ifndef KIOSK_H_
#define KIOSK_H_



#include "STD_TYPES.h"

#define MAX_BALANCE     999
#define LOW_BALANCE     5

#define PEAK_FARE       3
#define OFFPEAK_FARE    2

#define MAX_CARDS       16
#define HISTORY_COUNT   5

void Kiosk_Init(void);

void Kiosk_Run(void);





#endif /* KIOSK_H_ */