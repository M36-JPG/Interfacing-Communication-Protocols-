/*
 * RFID.c
 *
 * Created: 9/30/2026 8:06:08 PM
 *  Author: S
 */ 
#include <avr/io.h>
#include <util/delay.h>

#include "SPI.h"

#include "RFID.h"

/* MFRC522 Registers */

#define COMMAND_REG       0x01
#define COM_IEN_REG       0x02
#define COM_IRQ_REG       0x04
#define ERROR_REG         0x06
#define FIFO_DATA_REG     0x09
#define FIFO_LEVEL_REG    0x0A
#define CONTROL_REG       0x0C
#define BIT_FRAMING_REG   0x0D
#define MODE_REG          0x11
#define TX_CONTROL_REG    0x14
#define TX_ASK_REG        0x15
#define VERSION_REG       0x37

/* Commands */

#define IDLE_CMD          0x00
#define TRANSCEIVE_CMD    0x0C

#define REQA              0x26
#define ANTICOLL          0x93

/* Pins */

#define RFID_SS_LOW() \
    PORTB &= ~(1 << PB2)

#define RFID_SS_HIGH() \
    PORTB |= (1 << PB2)

#define RFID_RST_LOW() \
    PORTD &= ~(1 << PD7)

#define RFID_RST_HIGH() \
    PORTD |= (1 << PD7)


static void RFID_WriteRegister(u8 reg,
                               u8 value)
{
    RFID_SS_LOW();

    SPI_Transfer((reg << 1) & 0x7E);

    SPI_Transfer(value);

    RFID_SS_HIGH();
}

static u8 RFID_ReadRegister(u8 reg)
{
    u8 value;

    RFID_SS_LOW();

    SPI_Transfer(((reg << 1) & 0x7E) |
                 0x80);

    value = SPI_Transfer(0);

    RFID_SS_HIGH();

    return value;
}

static void RFID_AntennaOn(void)
{
    u8 value;

    value =
        RFID_ReadRegister(TX_CONTROL_REG);

    if(!(value & 0x03))
    {
        RFID_WriteRegister(
            TX_CONTROL_REG,
            value | 0x03
        );
    }
}

u8 RFID_Init(void)
{
    u8 version;

    /*
        SS
    */

    DDRB |= (1 << PB2);

    RFID_SS_HIGH();

    /*
        RESET
    */

    DDRD |= (1 << PD7);

    RFID_RST_HIGH();

    _delay_ms(50);

    RFID_RST_LOW();

    _delay_ms(2);

    RFID_RST_HIGH();

    _delay_ms(50);

    /*
        SPI
    */

    SPI_Init();

    /*
        Soft reset
    */

    RFID_WriteRegister(
        COMMAND_REG,
        0x0F
    );

    _delay_ms(50);

    /*
        Configure transmitter
    */

    RFID_WriteRegister(
        TX_ASK_REG,
        0x40
    );

    RFID_WriteRegister(
        MODE_REG,
        0x3D
    );

    RFID_AntennaOn();

    version =
        RFID_ReadRegister(VERSION_REG);

    if(version == 0x00 ||
       version == 0xFF)
    {
        return FALSE;
    }

    return TRUE;
}

static u8 RFID_Transceive(
    u8 *sendData,
    u8 sendLength,
    u8 *receiveData,
    u8 *receiveLength)
{
    u8 irq;
    u8 fifoLength;
    u8 i;

    RFID_WriteRegister(
        COMMAND_REG,
        IDLE_CMD
    );

    RFID_WriteRegister(
        COM_IRQ_REG,
        0x7F
    );

    RFID_WriteRegister(
        FIFO_LEVEL_REG,
        0x80
    );

    for(i = 0;
        i < sendLength;
        i++)
    {
        RFID_WriteRegister(
            FIFO_DATA_REG,
            sendData[i]
        );
    }

    RFID_WriteRegister(
        COMMAND_REG,
        TRANSCEIVE_CMD
    );

    RFID_WriteRegister(
        BIT_FRAMING_REG,
        RFID_ReadRegister(
            BIT_FRAMING_REG
        ) | 0x80
    );

    for(i = 0;
        i < 40;
        i++)
    {
        irq =
            RFID_ReadRegister(
                COM_IRQ_REG
            );

        if(irq & 0x30)
        {
            break;
        }

        _delay_ms(1);
    }

    RFID_WriteRegister(
        BIT_FRAMING_REG,
        RFID_ReadRegister(
            BIT_FRAMING_REG
        ) & ~0x80
    );

    if(i == 40)
    {
        return FALSE;
    }

    if(RFID_ReadRegister(ERROR_REG)
       & 0x1B)
    {
        return FALSE;
    }

    fifoLength =
        RFID_ReadRegister(
            FIFO_LEVEL_REG
        );

    if(fifoLength > 18)
    {
        return FALSE;
    }

    *receiveLength = fifoLength;

    for(i = 0;
        i < fifoLength;
        i++)
    {
        receiveData[i] =
            RFID_ReadRegister(
                FIFO_DATA_REG
            );
    }

    return TRUE;
}

u8 RFID_IsCardPresent(void)
{
    u8 command;
    u8 response[16];
    u8 length;

    command = REQA;

    RFID_WriteRegister(
        BIT_FRAMING_REG,
        0x07
    );

    return RFID_Transceive(
        &command,
        1,
        response,
        &length
    );
}

u8 RFID_ReadUID(u8 *uid)
{
    u8 command[2];

    u8 response[16];

    u8 length;

    u8 i;

    command[0] = ANTICOLL;

    command[1] = 0x20;

    RFID_WriteRegister(
        BIT_FRAMING_REG,
        0x00
    );

    if(!RFID_Transceive(
        command,
        2,
        response,
        &length))
    {
        return FALSE;
    }

    if(length < 5)
    {
        return FALSE;
    }

    for(i = 0; i < 4; i++)
    {
        uid[i] = response[i];
    }

    return TRUE;
}

void RFID_StopCrypto(void)
{
    RFID_WriteRegister(
        COMMAND_REG,
        IDLE_CMD
    );
}