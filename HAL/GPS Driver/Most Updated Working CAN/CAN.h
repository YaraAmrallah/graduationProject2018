/*
 * CAN.h
 *
 *  Created on: Feb 28, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#ifndef CAN_H_
#define CAN_H_

#include "CAN_cfg.h"
#include <stdint.h>
#include "CAN_NwIdentifiers.h"
#include "GPIO.h"
#include"M4MemMap.h"

#define ENABLE 0
#define DISABLE 1
#define CAN_MESSAGE_OBJECT_NUMBER(X) X
#define LOOPBACK DISABLE

typedef enum {CAN0 = 0,CAN1} CANUnitId;
typedef enum {PF0_3 = 0, PB4_8, PE4_8, PA0_8} CANRx;
typedef enum {PF3_3 = 4, PB5_8, PE5_8, PA1_8} CANTx;
typedef enum {CAN_E_OK = 0,CAN_E_NOK} CAN_CheckType;

typedef enum {stdId = 0, extendedId} CAN_IdentifierSize;
typedef enum {invalid = 0, valid = 1} CAN_MessageObjectValidity;
typedef enum {disable = 0, enable} CAN_EnableType;
typedef enum {transfer = 0, receive} CAN_MessageObjectType;

typedef struct
{
    /* CAN unit Number */

    CANUnitId unitId;

    /* CAN Receiver Pin Number */

    CANRx receiverPin;

    /* CAN Transmitter Pin Number */

    CANTx transmitterPin;

    /* Bit Rate in Kbps */

    uint16_t baudRate;

    /* Propagation delay parameters */

    uint16_t  busDriverDelay;

    /* Receiver Circuit Delay */

    uint16_t  receiverCircuitDelay;

    /* Bus line delay */

    uint16_t busLineDelay;

} CAN_cfgType;

extern const CAN_cfgType CAN_configParameters[CAN_GROUPS_NUMBER];


typedef struct
{
    /* CAN unit Number */

    CANUnitId unitId;

    /* Message object type */

    CAN_MessageObjectType messageObjectType;

    /* Message object Number */

    uint8_t messageObjectNumber;

    /* Identifier size */

    CAN_IdentifierSize IdSize;

    /* Message object ID */

    uint32_t messageObjectID;

    /* Message object validity */

    CAN_MessageObjectValidity messageValidity;

    /* Enable/disable acceptance identifier mask */

    CAN_EnableType enableMSK;

    /* Mask identifier size */

    CAN_IdentifierSize maskIdSize;

    /* Mask ID */

    uint32_t maskID;

    /* Enable/disable acceptance message direction */

    CAN_EnableType enableMaskMessageDirection;

    /* Enable/disable acceptance mask extended identifier */

    CAN_EnableType enableMaskExtID;

    /* Remote enable */

    CAN_EnableType remoteEnable;

    /* End of buffer, enabled for a single message object */

    CAN_EnableType endOfBuffer;

    /* Number of bytes in a data frame, specifies a number between 0 and 8 */

    uint8_t dataLengthCode;


} CAN_MessageObject_cfgType;


extern const CAN_MessageObject_cfgType CAN_MessageObject_configParameters[TOTAL_MESSAGE_OBJECT_NUMBER];


CAN_CheckType CAN_Init();
static void CAN_SetBauderate (uint8_t configID);
static uint8_t CAN_MessageObjectsConfig ();
void CAN_Write_TX (uint8_t MessageObject_configID, uint8_t *DataPtr);
void CAN_TX (uint8_t MessageObject_configID);
void CAN_RX (uint8_t MessageObject_configID, uint8_t *DataPtr);
void test();
void test1(uint8_t *ptr);
void CAN_WriteData (uint8_t MessageObject_configID, uint8_t* DataPtr);
void CAN_TX_RemoteFrame (uint8_t MessageObject_configID);
void CAN_Request_RemoteTx(uint8_t MessageObject_configID);
void CAN_Receive_RemoteTx();
static uint8_t CAN_ReservedObjectsConfig();
uint8_t newDataFlag(CANUnitId CAN_UnitID, uint8_t messageObjectNumber);
uint8_t txRequestFlag(CANUnitId CAN_UnitID, uint8_t messageObjectNumber);
#endif /* CAN_H_ */

