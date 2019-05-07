#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#include "I2C_CFG.h"
typedef enum
{
    I2C_NOK = 0,
    I2C_OK = 1,
    I2C_SINGLENOK = 2,
    I2C_SINGLEOK = 3,
    I2C_BURSTOK = 4,
    I2C_BURSTNOK = 5,
    I2C_READDATAOK = 6,
    I2C_BURST_READOK = 7,
    I2C_SREAD_NOK = 8,
    I2C_SREAD_OK = 9,
    I2C_NOBUSY = 10,
    I2C_BUSY = 11,
    I2C_MASTERBUSY = 12,
    I2C_MASTERNOBUSY = 13
} I2C_CheckType;
typedef enum
{
    SINGLE_OP = 0x00000007,
    BURST_SEND_START = 0x00000003,
    BURST_SEND_CONT = 0x00000001,
    BURST_DONE = 0x00000005,
    BURST_RECEIVE_START = 0x0000000b,
    BURST_RECEIVE_CONT = 0x00000009,
    BURST_RECEIVE_ERROR_STOP = 0x00000004
} Ops;
typedef enum
{
    I2C_SINGLESEND = 0,
    I2C_BURSTSEND_STAT = 1,
    MASTER_BUSY = 2,
    I2C_BUSBUSY = 3,
    I2C_MBUSY = 5
} I2C_Checkstatus;

typedef struct
{
    uint8_t PortID;
    uint8_t SDA_Pin;
    uint8_t SCK_Pin;
    uint32_t Clock;
    uint8_t Enable;
} I2C_Mode;
typedef enum
{

    I2C_Transmit = 0, I2C_Receive = 1

} I2C_Receive_transmit;
typedef enum
{
    I2C_Busy = 0, //WHEN SET ALL OTHER STATUS BITS ARE NOT VALID
    I2C_ERROR = 1,
    I2C_ADDR_ACK = 2,
    I2C_DATA_ACK = 3,
    I2C_ARBIR_LST = 4,
    I2C_IDEL = 5,
    I2C_BUS_BUSY = 6, //CHANGE BASED ON START TOP BIT
    I2C_CLK_TO = 7
} I2C_STATUS;

typedef enum
{

    I2C_START_GEN = 0x00000007, I2C_STOP_GEN = 0x00000005
} I2C_Control;
typedef enum
{
    I2C_RUN_EN = 0, I2C_START = 1, I2C_STOP = 2, I2C_ACK = 3, I2C_HS_EN = 4

} I2C_position;
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
    I2C_RIS_NSET = 0, I2C_RIS_SET = 1
} MASK;
typedef enum
{
    DISABLE_GENERAL_CALL = 0, ENABLE_GENERAL_CALL = 1
} GeneralCall;

typedef void (*I2C_CallBackFunctionType)(void);

typedef enum
{

    PERIPHERAL_CLOCK_16MHz_Standard = 0x07, PERIPHERAL_CLOCK_16MHz_Fast = 0x01

} PeripheralFrequency; //for standardmode

typedef struct
{
    uint8_t I2C_GPIO_Structure_ID;
    /*used to choose from I2Cx,  x=1,2,3*/
    Peripheral_ID I2C_Peripheral_ID;

    uint32_t I2C_FrequencyOf_SCL;

    PeripheralFrequency I2C_PeripheralFrequency;
    /**/
    Type I2C_TYPE;

    SpeedMode I2C_SpeedMode;
    /**/
    GeneralCall I2C_GeneralCall;
    /**/
    I2C_CallBackFunctionType I2C_TransmissionDoneCallBack;

    I2C_CallBackFunctionType I2C_ReceptionDoneCallBack;

} I2C_ConfigType;

extern const I2C_ConfigType I2C_ConfigParam[I2C_PERIPHERAL_NUMBER];
extern uint8_t I2C_InitFlag;
/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            				Functions Prototypes				              *
 ******************************************************************************
 */

I2C_CheckType I2C_Init(void);
extern uint8_t I2C_TX(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data,
                      uint8_t datacount);

extern void I2C_GenerateStart(uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_StartStatus(uint8_t Peripheral_ID);

extern void I2C_SendSlaveAddress(uint8_t SlaveAddress, uint8_t WriteOrRead,
                                 uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_SendSlaveAddressStatus(uint8_t Peripheral_ID);
extern void I2C_Clear_ADDR(uint8_t Peripheral_ID);

extern void I2C_SendData(uint8_t* DataPtr, uint8_t Peripheral_ID);
//extern I2C_CheckType I2CSendDataStatus(uint8_t Peripheral_ID);

extern void I2C_GetData(uint8_t *Data, uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_GetDataStatus(uint8_t Peripheral_ID);
extern I2C_CheckType I2C_CHECKSTATUS(uint8_t Peripheral_ID,
                                     I2C_Checkstatus Status);
extern void I2C_GenerateStop(uint8_t Peripheral_ID);

extern void I2C_TurnOffAcknowledge(uint8_t Peripheral_ID);
extern void I2C_TurnOnAcknowledge(uint8_t Peripheral_ID);

#endif
