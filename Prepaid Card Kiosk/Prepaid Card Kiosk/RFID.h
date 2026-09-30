/*
 * RFID.h
 *
 * Created: 9/30/2026 8:05:41 PM
 *  Author: S
 */ 


#ifndef RFID_H_
#define RFID_H_



#include "STD_TYPES.h"

u8 RFID_Init(void);

u8 RFID_IsCardPresent(void);

u8 RFID_ReadUID(u8 *uid);

void RFID_StopCrypto(void);





#endif /* RFID_H_ */