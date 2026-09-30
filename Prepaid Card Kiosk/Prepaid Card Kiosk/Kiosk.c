/*
 * Kiosk.c
 *
 * Created: 9/30/2026 8:07:24 PM
 *  Author: S
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "UART.h"
#include "I2C.h"

#include "RFID.h"
#include "EEPROM.h"
#include "RTC.h"
#include "OLED.h"
#include "FEEDBACK.h"

#include "Kiosk.h"


#define CARD_BASE       0x0100

#define CARD_SIZE       64

#define HISTORY_BASE    0x0500

#define HISTORY_RECORD_SIZE  16


typedef struct
{
    u8 type;

    u16 amount;

    u16 balance;

    u8 hour;

    u8 min;

} Transaction;


static char commandBuffer[64];

static u8 commandIndex = 0;

static u16 eventNumber = 0;


/*--------------------------------------------------
                    UID
--------------------------------------------------*/

static u8 UID_Equal(
    u8 *uid1,
    u8 *uid2)
{
    u8 i;

    for(i = 0; i < 4; i++)
    {
        if(uid1[i] != uid2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}


/*--------------------------------------------------
                Number To String
--------------------------------------------------*/

static void NumberToString(
    u16 number,
    char *string)
{
    char temp[6];

    u8 i = 0;

    u8 j = 0;

    if(number == 0)
    {
        string[0] = '0';

        string[1] = '\0';

        return;
    }

    while(number > 0)
    {
        temp[i++] =
            '0' + (number % 10);

        number /= 10;
    }

    while(i > 0)
    {
        string[j++] =
            temp[--i];
    }

    string[j] = '\0';
}


/*--------------------------------------------------
                    Slot Address
--------------------------------------------------*/

static u16 GetCardAddress(
    u8 slot)
{
    return CARD_BASE +
           ((u16)slot * CARD_SIZE);
}


/*--------------------------------------------------
                    Find Card
--------------------------------------------------*/

static u8 FindCard(
    u8 *uid)
{
    u8 slot;

    u8 i;

    u8 same;

    for(slot = 0;
        slot < MAX_CARDS;
        slot++)
    {
        same = TRUE;

        for(i = 0;
            i < 4;
            i++)
        {
            if(EEPROM_ReadByte(
                GetCardAddress(slot) + i)
                != uid[i])
            {
                same = FALSE;
            }
        }

        if(same)
        {
            return slot;
        }
    }

    return 0xFF;
}


/*--------------------------------------------------
                  Create New Card
--------------------------------------------------*/

static u8 CreateCard(
    u8 *uid)
{
    u8 slot;

    u8 i;

    for(slot = 0;
        slot < MAX_CARDS;
        slot++)
    {
        /*
            Byte +5 is our valid flag.
        */

        if(EEPROM_ReadByte(
            GetCardAddress(slot) + 5)
            != 0xA5)
        {
            for(i = 0;
                i < 4;
                i++)
            {
                EEPROM_WriteByte(
                    GetCardAddress(slot) + i,
                    uid[i]
                );
            }

            /*
                Balance = 0
            */

            EEPROM_WriteByte(
                GetCardAddress(slot) + 6,
                0
            );

            EEPROM_WriteByte(
                GetCardAddress(slot) + 7,
                0
            );

            /*
                Valid
            */

            EEPROM_WriteByte(
                GetCardAddress(slot) + 5,
                0xA5
            );

            return slot;
        }
    }

    return 0xFF;
}


/*--------------------------------------------------
                  Get Balance
--------------------------------------------------*/

static u16 GetBalance(
    u8 slot)
{
    u16 address;

    u16 balance;

    address =
        GetCardAddress(slot) + 6;

    balance =
        EEPROM_ReadByte(address);

    balance |=
        ((u16)EEPROM_ReadByte(
            address + 1) << 8);

    return balance;
}


/*--------------------------------------------------
                  Save Balance
--------------------------------------------------*/

static u8 SaveBalance(
    u8 slot,
    u16 balance)
{
    u16 address;

    address =
        GetCardAddress(slot) + 6;

    /*
        Low byte
    */

    if(!EEPROM_WriteByte(
        address,
        (u8)balance))
    {
        return FALSE;
    }

    /*
        High byte
    */

    if(!EEPROM_WriteByte(
        address + 1,
        (u8)(balance >> 8)))
    {
        return FALSE;
    }

    /*
        R7:
        Read back and verify.
    */

    if(GetBalance(slot) != balance)
    {
        return FALSE;
    }

    return TRUE;
}


/*--------------------------------------------------
                    UID String
--------------------------------------------------*/

static void UIDToString(
    u8 *uid,
    char *string)
{
    sprintf(
        string,
        "%02X%02X%02X%02X",
        uid[0],
        uid[1],
        uid[2],
        uid[3]
    );
}


/*--------------------------------------------------
                  Send Event
--------------------------------------------------*/

static void SendEvent(
    const char *type,
    const char *data)
{
    RTC_Time time;

    char number[8];

    char timeString[16];

    char line[100];

    u8 checksum = 0;

    u8 i;

    char checksumString[5];


    if(!RTC_GetTime(&time))
    {
        return;
    }


    NumberToString(
        eventNumber++,
        number
    );


    sprintf(
        timeString,
        "%02u:%02u:%02u",
        time.hour,
        time.min,
        time.sec
    );


    sprintf(
        line,
        "$EVT,%s,%s,%s,%s*",
        number,
        timeString,
        type,
        data
    );


    for(i = 0;
        line[i] != '\0';
        i++)
    {
        checksum ^=
            (u8)line[i];
    }


    sprintf(
        checksumString,
        "%02X",
        checksum
    );


    UART_SendString(line);

    UART_SendString(
        checksumString
    );

    UART_SendString(
        "\r\n"
    );
}


/*--------------------------------------------------
              Log Transaction
--------------------------------------------------*/

static void LogTransaction(
    u8 slot,
    u8 type,
    u16 amount,
    u16 balance)
{
    RTC_Time time;

    Transaction transaction;

    u8 index;

    u16 address;


    if(!RTC_GetTime(&time))
    {
        return;
    }


    index =
        EEPROM_ReadByte(
            GetCardAddress(slot) + 4
        );


    if(index >= HISTORY_COUNT)
    {
        index = 0;
    }


    address =
        HISTORY_BASE +

        ((u16)slot *
         HISTORY_COUNT *
         HISTORY_RECORD_SIZE) +

        ((u16)index *
         HISTORY_RECORD_SIZE);


    transaction.type =
        type;

    transaction.amount =
        amount;

    transaction.balance =
        balance;

    transaction.hour =
        time.hour;

    transaction.min =
        time.min;


    EEPROM_WriteBlock(
        address,
        (u8*)&transaction,
        sizeof(Transaction)
    );


    index++;

    if(index >= HISTORY_COUNT)
    {
        index = 0;
    }


    EEPROM_WriteByte(
        GetCardAddress(slot) + 4,
        index
    );
}


/*--------------------------------------------------
                 Display Balance
--------------------------------------------------*/

static void DisplayBalance(
    u16 balance)
{
    char text[8];

    OLED_Clear();

    OLED_WriteString(
        "Balance: "
    );

    NumberToString(
        balance,
        text
    );

    OLED_WriteString(text);


    if(balance < LOW_BALANCE)
    {
        OLED_SetCursor(1, 0);

        OLED_WriteString(
            "Low balance"
        );

        Feedback_Warning();
    }
}


/*--------------------------------------------------
                    TOPUP
--------------------------------------------------*/

static void ProcessTopup(
    char *uidString,
    u16 amount)
{
    u8 uid[4];

    u8 i;

    u8 slot;

    u16 balance;

    char data[40];


    if(strlen(uidString) != 8)
    {
        UART_SendString(
            "ERR\r\n"
        );

        return;
    }


    /*
        Convert UID string
        A1B2C3D4
        into 4 bytes
    */

    for(i = 0;
        i < 4;
        i++)
    {
        unsigned int value;

        sscanf(
            uidString + i * 2,
            "%2X",
            &value
        );

        uid[i] =
            (u8)value;
    }


    slot =
        FindCard(uid);


    if(slot == 0xFF)
    {
        slot =
            CreateCard(uid);
    }


    if(slot == 0xFF)
    {
        UART_SendString(
            "ERR FULL\r\n"
        );

        return;
    }


    balance =
        GetBalance(slot);


    /*
        Maximum = 999
    */

    if(amount >
       (MAX_BALANCE - balance))
    {
        UART_SendString(
            "ERR MAX\r\n"
        );

        return;
    }


    balance += amount;


    if(!SaveBalance(
        slot,
        balance))
    {
        SendEvent(
            "ERROR",
            "EEPROM"
        );

        return;
    }


    LogTransaction(
        slot,
        2,
        amount,
        balance
    );


    sprintf(
        data,
        "%s,%u",
        uidString,
        amount
    );


    SendEvent(
        "TOPUP",
        data
    );


    UART_SendString(
        "OK\r\n"
    );
}


/*--------------------------------------------------
                   BALANCE
--------------------------------------------------*/

static void ProcessBalance(
    char *uidString)
{
    u8 uid[4];

    u8 i;

    u8 slot;

    u16 balance;

    char text[8];


    for(i = 0;
        i < 4;
        i++)
    {
        unsigned int value;

        sscanf(
            uidString + i * 2,
            "%2X",
            &value
        );

        uid[i] =
            (u8)value;
    }


    slot =
        FindCard(uid);


    if(slot == 0xFF)
    {
        UART_SendString(
            "ERR NOTFOUND\r\n"
        );

        return;
    }


    balance =
        GetBalance(slot);


    NumberToString(
        balance,
        text
    );


    UART_SendString(
        "BALANCE "
    );

    UART_SendString(
        text
    );

    UART_SendString(
        "\r\n"
    );
}


/*--------------------------------------------------
                   SETTIME
--------------------------------------------------*/

static void ProcessSetTime(
    char *text)
{
    RTC_Time time;

    unsigned int year;


    /*
        YYYY-MM-DDTHH:MM:SS
    */

    if(sscanf(
        text,
        "%u-%hhu-%hhuT%hhu:%hhu:%hhu",
        &year,
        &time.month,
        &time.day,
        &time.hour,
        &time.min,
        &time.sec
    ) == 6)
    {
        time.year =
            year % 100;


        if(RTC_SetTime(
            &time))
        {
            UART_SendString(
                "OK\r\n"
            );
        }
        else
        {
            UART_SendString(
                "ERR\r\n"
            );
        }
    }
    else
    {
        UART_SendString(
            "ERR\r\n"
        );
    }
}


/*--------------------------------------------------
                 HISTORY
--------------------------------------------------*/

static void ProcessHistory(
    char *uidString)
{
    u8 uid[4];

    u8 i;

    u8 slot;

    u8 index;

    u8 count;

    Transaction transaction;

    char line[50];


    for(i = 0;
        i < 4;
        i++)
    {
        unsigned int value;

        sscanf(
            uidString + i * 2,
            "%2X",
            &value
        );

        uid[i] =
            (u8)value;
    }


    slot =
        FindCard(uid);


    if(slot == 0xFF)
    {
        UART_SendString(
            "ERR NOTFOUND\r\n"
        );

        return;
    }


    index =
        EEPROM_ReadByte(
            GetCardAddress(slot) + 4
        );


    /*
        Read newest ? oldest
    */

    for(count = 0;
        count < HISTORY_COUNT;
        count++)
    {
        if(index == 0)
        {
            index =
                HISTORY_COUNT - 1;
        }
        else
        {
            index--;
        }


        EEPROM_ReadBlock(
            HISTORY_BASE +

            ((u16)slot *
             HISTORY_COUNT *
             HISTORY_RECORD_SIZE) +

            ((u16)index *
             HISTORY_RECORD_SIZE),

            (u8*)&transaction,

            sizeof(Transaction)
        );


        if(transaction.type != 0)
        {
            sprintf(
                line,
                "%u,%u,%u,%02u:%02u\r\n",

                transaction.type,

                transaction.amount,

                transaction.balance,

                transaction.hour,

                transaction.min
            );

            UART_SendString(
                line
            );
        }
    }
}


/*--------------------------------------------------
                 Command Parser
--------------------------------------------------*/

static void ProcessCommand(
    char *command)
{
    if(strncmp(
        command,
        "TOPUP ",
        6) == 0)
    {
        char uid[12];

        unsigned int amount;


        if(sscanf(
            command + 6,
            "%11s %u",
            uid,
            &amount
        ) == 2)
        {
            ProcessTopup(
                uid,
                (u16)amount
            );
        }
        else
        {
            UART_SendString(
                "ERR\r\n"
            );
        }
    }


    else if(strncmp(
        command,
        "BALANCE ",
        8) == 0)
    {
        ProcessBalance(
            command + 8
        );
    }


    else if(strncmp(
        command,
        "HISTORY ",
        8) == 0)
    {
        ProcessHistory(
            command + 8
        );
    }


    else if(strncmp(
        command,
        "SETTIME ",
        8) == 0)
    {
        ProcessSetTime(
            command + 8
        );
    }


    else
    {
        UART_SendString(
            "ERR CMD\r\n"
        );
    }
}


/*--------------------------------------------------
                    INIT
--------------------------------------------------*/

void Kiosk_Init(void)
{
    I2C_Init();

    UART_Init(9600);

    Feedback_Init();

    OLED_Init();

    RTC_Init();

    RFID_Init();


    OLED_Clear();

    OLED_WriteString(
        "Prepaid Kiosk"
    );


    SendEvent(
        "BOOT",
        "FW1.0"
    );
}


/*--------------------------------------------------
                     MAIN APP
--------------------------------------------------*/

void Kiosk_Run(void)
{
    u8 uid[4];

    u8 slot;

    u8 fare;

    u16 balance;

    RTC_Time time;

    char uidString[12];

    char data[50];


    /*------------------------------------------
                PC COMMANDS
    ------------------------------------------*/

    if(UART_Available())
    {
        u8 received;

        received =
            UART_Receive();


        if(received == '\r' ||
           received == '\n')
        {
            if(commandIndex > 0)
            {
                commandBuffer[
                    commandIndex
                ] = '\0';


                ProcessCommand(
                    commandBuffer
                );


                commandIndex = 0;
            }
        }

        else
        {
            if(commandIndex < 63)
            {
                commandBuffer[
                    commandIndex++
                ] = received;
            }
        }
    }


    /*------------------------------------------
                    RFID
    ------------------------------------------*/

    if(RFID_IsCardPresent())
    {
        if(RFID_ReadUID(uid))
        {
            slot =
                FindCard(uid);


            if(slot == 0xFF)
            {
                slot =
                    CreateCard(uid);
            }


            if(slot != 0xFF)
            {
                balance =
                    GetBalance(slot);


                if(RTC_GetTime(
                    &time))
                {
                    /*
                        Determine fare
                    */

                    if(RTC_IsPeak(
                        &time))
                    {
                        fare =
                            PEAK_FARE;
                    }
                    else
                    {
                        fare =
                            OFFPEAK_FARE;
                    }


                    UIDToString(
                        uid,
                        uidString
                    );


                    /*----------------------
                         ENOUGH BALANCE
                    ----------------------*/

                    if(balance >= fare)
                    {
                        balance -= fare;


                        if(SaveBalance(
                            slot,
                            balance))
                        {
                            LogTransaction(
                                slot,
                                1,
                                fare,
                                balance
                            );


                            sprintf(
                                data,
                                "%s,%u,%u",
                                uidString,
                                fare,
                                balance
                            );


                            SendEvent(
                                "PAY",
                                data
                            );


                            DisplayBalance(
                                balance
                            );


                            Feedback_OK();
                        }
                        else
                        {
                            SendEvent(
                                "ERROR",
                                "EEPROM"
                            );

                            Feedback_Warning();
                        }
                    }


                    /*----------------------
                        NOT ENOUGH BALANCE
                    ----------------------*/

                    else
                    {
                        sprintf(
                            data,
                            "%s,LOW_BALANCE",
                            uidString
                        );


                        SendEvent(
                            "REFUSED",
                            data
                        );


                        OLED_Clear();

                        OLED_WriteString(
                            "Insufficient"
                        );

                        OLED_SetCursor(1, 0);

                        OLED_WriteString(
                            "Balance: "
                        );


                        char balanceText[8];


                        NumberToString(
                            balance,
                            balanceText
                        );


                        OLED_WriteString(
                            balanceText
                        );


                        Feedback_Denied();
                    }
                }
                else
                {
                    SendEvent(
                        "ERROR",
                        "RTC"
                    );

                    Feedback_Warning();
                }
            }
            else
            {
                SendEvent(
                    "ERROR",
                    "EEPROM"
                );

                Feedback_Warning();
            }


            RFID_StopCrypto();

            _delay_ms(1000);
        }
    }
}