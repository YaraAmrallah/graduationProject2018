#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <Reg.h>
#include "I2C_CFG.h"

typedef enum
{
    I2C_OK = 0,
    I2C_NOK = 1
} I2C_CheckType;

typedef enum
{

    I2C_ERROR = 0x00000002,
    I2C_BUSY = 0x00000001,
    I2C_BUSBUSY = 0x00000040,
    I2C_ADRACK = 0x00000004,
    I2C_DATAACK =0x00000008,
    I2C_ARBITLST = 0x00000010


} Flags;
typedef enum
{

    I2C_Transmit = 0, I2C_Receive = 1

} I2C_Receive_transmit;


typedef enum
{
    I2C_0 = 0, I2C_1 = 1, I2C_2 = 2, I2C_3 = 3
} Peripheral_ID;
typedef enum
{
    PERIPHERAL_DISABLE = 0, PERIPHERAL_ENABLE = 1
} PeripheralEnable;
typedef enum
{
    MASTER = 0x10, SLAVE = 0x20
} Type;

typedef enum
{
    STANDARD_MODE = 0, FAST_MODE = 1
} SpeedMode;

typedef enum
{
    DISABLE_GENERAL_CALL = 0, ENABLE_GENERAL_CALL = 1
} GeneralCall;

typedef void (*I2C_CallBackFunctionType)(void);

typedef enum
{

    PERIPHERAL_CLOCK_16MHz_Standard = 0x07, PERIPHERAL_CLOCK_16MHz_Fast = 0x01

} PRESCALE;

typedef struct
{
            Peripheral_ID ChannelId; // 0, 1, 2, or 3
            Type ModuleType; // Master -> 0 - Slave  -> 1
            PRESCALE Prescalar; // Solving for TPR: SCL_PERIOD = 2×(1 + TPR)×(SCL_LP + SCL_HP)×CLK_PRD
            SpeedMode I2C_SpeedMode;
            uint32_t I2C_FrequencyOf_SCL;
}I2C_CfgType; // The set of parameters needed for any I2C Module configuration

extern const I2C_CfgType I2C_ConfigParam [I2C_GROUPS_NUMBER]; // Declaring an array of the I2C_CfgType type
/*
 **********************************************
 Functions Prototypes
 **********************************************
 */
I2C_CheckType I2C_Init(void); //To initialize I2C modules
I2C_CheckType I2C_SetSlaveAdd(uint8_t SlaveAddress, uint8_t ReadWrite,
                                   uint8_t GroupID); //To place Slave Address in MCS register + Value of R/S bit
I2C_CheckType I2C_SendData(uint8_t* Data, uint8_t GroupID); //To place the Data in case of Transmission
I2C_CheckType I2C_ReadData(uint8_t* Data, uint8_t GroupID); //To read the Data in case of Reception
I2C_CheckType I2C_SetOperation(uint8_t Condition, uint8_t GroupID); //Configuring the suitable "Codes" for the I2C Module operation
I2C_CheckType I2C_CheckBusStat(uint8_t GroupID); //Checking whether the bus is acquired by another Master/Slave or not
I2C_CheckType I2C_CheckBUSY(uint8_t GroupID); //Checking whether the I2C controller is sending/receiving or not
I2C_CheckType I2C_CheckErrBit(uint8_t GroupID); //Checking for all kinds of error
I2C_CheckType I2C_CheckArbitBit(uint8_t GroupID); //Checking for Arbitration error
I2C_CheckType I2C_Addcheck(uint8_t GroupID); //Checking for add ack
I2C_CheckType I2C_Datacheck(uint8_t GroupID); //Checking for data ack

#endif
