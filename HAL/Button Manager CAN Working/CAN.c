/*
 * CAN.c
 *
 *  Created on: Feb 28, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include <stdint.h>
#include "CAN.h"
#include "GPIO.h"
#include "M4MemMap.h"

typedef volatile uint32_t* const CAN_RegAddType;

#define CAN_UNITS 2U

#define CAN_0BaseAddress 0x40040000
#define CAN_1BaseAddress 0x40041000

static const uint32_t CAN_BaseAddressLut[CAN_UNITS] =
{
 CAN_0BaseAddress,
 CAN_1BaseAddress
};

/* Defining Clock Gating Control register 0 */
#define RCGC0_CAN                           *((CAN_RegAddType)(0x400FE100))

/* Defining Control Registers */
#define CAN_CTL(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x000))
#define CAN_STS(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x004))
#define CAN_ERR(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x008))
#define CAN_BIT(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x00C))
#define CAN_INT(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x010))
#define CAN_TST(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x014))
#define CAN_BRPE(CAN_ID)                 *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x018))

/* Defining Interface Registers */
#define CAN_IFnCRQ(CAN_ID,MESSAGE_OBJECT_TYPE)               *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x020) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnCMSK(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x024) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnMSK1(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x028) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnMSK2(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x02C) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnARB1(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x030) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnARB2(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x034) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnMCTL(CAN_ID,MESSAGE_OBJECT_TYPE)              *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x038) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnDA1(CAN_ID,MESSAGE_OBJECT_TYPE)               *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x03C) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnDA2(CAN_ID,MESSAGE_OBJECT_TYPE)               *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x040) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnDB1(CAN_ID,MESSAGE_OBJECT_TYPE)               *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x044) + (0x060*MESSAGE_OBJECT_TYPE)))
#define CAN_IFnDB2(CAN_ID,MESSAGE_OBJECT_TYPE)               *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x048) + (0x060*MESSAGE_OBJECT_TYPE)))

/* Defining CAN Message Object Registers */
#define CAN_TXRQn(CAN_ID,MESSAGE_BLOCK)                      *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x100) + (0x004*MESSAGE_BLOCK)))
#define CAN_NWDATAn(CAN_ID,MESSAGE_BLOCK)                    *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x120) + (0x004*MESSAGE_BLOCK)))
#define CANMSG1INT                                           *((CAN_RegAddType)(CAN_0BaseAddress + 0x140))
#define CANMSG1VAL                                           *((CAN_RegAddType)(CAN_0BaseAddress + 0x160))
// #define CAN_NEWDATACHECK(MSG_OBJECT)                          (CANNWDA1 >> (MSG_OBJECT - 1) && 0x01)

#define CAN_IFnDATA(CAN_ID,MESSAGE_OBJECT_TYPE,WORD_NUMBER)  *((CAN_RegAddType)((CAN_BaseAddressLut[CAN_ID] + 0x03C +(0x004*WORD_NUMBER)) + (0x060*MESSAGE_OBJECT_TYPE)))

/*--------------------------------- Global Variables ------------------------*/
uint8_t initFlag[CAN_GROUPS_NUMBER] = {0};
uint8_t TxFlag[ TOTAL_MESSAGE_OBJECT_NUMBER] = {0};

/*------------------------- Reserved Message Objects ------------------------*/

const CAN_MessageObject_cfgType CAN_ReservedObject_configParameters[8] =
{
 /* Additional Message objects servers */
 {CAN0,transfer,31,stdId,0xAD,valid,disable,stdId,0x0000,disable,disable,disable,enable,8},
 {CAN0,receive,32,stdId,0xFD2,valid,enable,stdId,0xA56,disable,disable,enable,enable,8},

 /* Additional Message objects servers */
 {CAN1,transfer,31,stdId,0xAD,valid,disable,stdId,0x0000,disable,disable,disable,enable,8},
 {CAN1,receive,32,stdId,0xFD2,valid,enable,stdId,0xA56,disable,disable,enable,enable,8}
};


/* -------------------------------- Functions -------------------------------*/

CAN_CheckType CAN_Init()
{
    uint8_t LoopIndex;
    CAN_CheckType retVar = CAN_E_NOK;
    const CAN_cfgType * CfgPtr;
    uint8_t errorFlag = 0;

    if (CAN_GROUPS_NUMBER <= CAN_UNITS)
    {
        for(LoopIndex = 0; LoopIndex < CAN_GROUPS_NUMBER ; LoopIndex ++)
        {
            CfgPtr = &CAN_configParameters[LoopIndex];

            /* Check for proper pins, modules and baud rate configurations */

            if ((CfgPtr->receiverPin < PA0_8) && (CfgPtr->transmitterPin < PA1_8) && (CfgPtr->transmitterPin >= PF3_3) && (CfgPtr->unitId == CAN0))
            {
                if ((CfgPtr->baudRate >= MINIMUM_BAUD_RATE) && (CfgPtr->baudRate <= MAXIMUM_BAUD_RATE))
                {
                    initFlag[LoopIndex] = 1;
                }
                else
                {
                    errorFlag++;
                }
            }
            else if ((CfgPtr->receiverPin == PA0_8) && (CfgPtr->transmitterPin == PA1_8) && (CfgPtr->unitId == CAN1))
            {
                if ((CfgPtr->baudRate >= MINIMUM_BAUD_RATE) && (CfgPtr->baudRate <= MAXIMUM_BAUD_RATE))
                {
                    initFlag[LoopIndex] = 1;
                }
                else
                {
                    errorFlag++;
                }
            }
            else
            {
                errorFlag++;
            }

            /* Module's Initialization */

            if(initFlag[LoopIndex] == 1)
            {
                /* Enable the peripheral Clock */
                RCGCCAN_REG |= 1 << (CfgPtr->unitId);
                //RCGC0_CAN |= 1 << (24 + CfgPtr->unitId);

                /*Entering the initialization state does not change the configuration of the CAN controller,
                 * the message objects, or the error counters. However, some configuration registers
                 * are only accessible while in the initialization state.*/

                /* Module's Initialization - 1 --> INIT bit*/
                CAN_CTL(CfgPtr->unitId) |= 1 << 0;

                if(LOOPBACK == ENABLE)
                {
                    CAN_CTL(CfgPtr->unitId) |= 1 << 7;
                    CAN_TST(CfgPtr->unitId) |= 1 << 4;
                }
                else
                {
                    /* do nothing */
                }

                /* Set CAN Bit timing  6 --> CCE bit */
                /* - Enable Configuration Change */
                CAN_CTL(CfgPtr->unitId) |= 1 << 6;
                /* - CAN_BIT register configuration */
                CAN_SetBauderate(LoopIndex);

                /*Enable or Disable auto retransmission  */
                CAN_CTL(CfgPtr->unitId) |= (AUTO_RETRANSMISSION << 5);

                /* End of module's initialization - 0 --> INIT bit*/
                CAN_CTL(CfgPtr->unitId) &= ~(1 << 0);

                /* Message objects configuration */
                errorFlag = CAN_MessageObjectsConfig () + errorFlag;
                //errorFlag = CAN_ReservedObjectsConfig() + errorFlag;
            }
            else
            {
                /* do nothing */
            }
        }
    }
    else
    {
        errorFlag++;
    }

    if (errorFlag == 0)
    {
        retVar = CAN_E_OK  ;
    }
    else
    {
        /* do nothing */
    }

    return retVar;

}

/*---------------------------------------- Static function to configure message objects ------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------*/

static void CAN_SetBauderate(uint8_t configID)
{
    uint16_t timeQuantaLength = 0; /* tq in nano second */
    uint8_t timeQuantaNumber = 4;
    uint16_t BRP = 0;
    uint8_t tProp = 0;
    uint16_t tTSeg1 = 0;
    uint16_t tTSeg2 = 0;
    uint16_t tSJW = 0;
    uint8_t stopFlag = 0;
    uint8_t LoopIndex;
    float tempVar1 = 0;
    float tempVar2 = 0;
    uint16_t tempVar3 = 0;
    const CAN_cfgType * CfgPtr;

    CfgPtr = &CAN_configParameters[configID];

    for ( LoopIndex = timeQuantaNumber; ((LoopIndex <= 25 ) && (stopFlag==0)); LoopIndex++)
    {
        tempVar1 = (float)1000000/((CfgPtr->baudRate) *LoopIndex)  ; /* to calculate tq by using n integer num */
        tempVar2 = (float)(tempVar1 * SYSTEM_FREQUENCY)/1000 ;

        if ((tempVar1 == (uint16_t)tempVar1) && (tempVar2 == (uint16_t)tempVar2) && (tempVar2 < 1023))
        {
            timeQuantaLength = (uint16_t)tempVar1;
            BRP = (uint16_t)tempVar2 - 1;

            stopFlag = 1 ;
        }
        else
        {
            /* do nothing */
        }
    }

    timeQuantaNumber = LoopIndex - 1;

    tempVar1 = (float)(CfgPtr->busDriverDelay + CfgPtr->receiverCircuitDelay + CfgPtr->busLineDelay)/timeQuantaLength;

    if (tempVar1 == (uint16_t)tempVar1)
    {
        tProp = (uint8_t) tempVar1;
        tempVar3 = timeQuantaNumber - tProp - 1;
    }
    else
    {
        tProp = (uint8_t)tempVar1 + 1;
        tempVar3 = timeQuantaNumber - tProp - 1;
    }

    if (tempVar3 <= 1)
    {
        tTSeg1 = tProp - 1;
        tTSeg2 = 0;
        tSJW = 0;
    }
    else if ((tempVar3 % 2) == 0)
    {
        tTSeg1 =  tProp + (tempVar3/2) - 1;
        tTSeg2 = (tempVar3/2) - 1;
        tSJW = (tempVar3/2) - 1;
    }
    else
    {
        tTSeg1 = tProp + (tempVar3/2) - 1;
        tTSeg2 = (tempVar3/2) + 1 - 1;
        tSJW  = (tempVar3/2) - 1;
    }

    CAN_BIT(CfgPtr->unitId) = 0x0000;

    if (BRP < 64)
    {
        CAN_BIT(CfgPtr->unitId) |= BRP;
        CAN_BIT(CfgPtr->unitId) |= (tSJW << 6);
        CAN_BIT(CfgPtr->unitId) |= (tTSeg1  << 8);
        CAN_BIT(CfgPtr->unitId) |= (tTSeg2  << 12);
    }
    else
    {
        tempVar3 = (0x3f & BRP);
        CAN_BIT(CfgPtr->unitId) |= tempVar3;
        CAN_BRPE(CfgPtr->unitId) |= (BRP >> 6);
        CAN_BIT(CfgPtr->unitId) |= (tSJW << 6);
        CAN_BIT(CfgPtr->unitId) |= (tTSeg1 << 8);
        CAN_BIT(CfgPtr->unitId) |= (tTSeg2 << 12);
    }
}

/*---------------------------------------- Static function to configure message objects ------------------------------------------*/

static uint8_t CAN_MessageObjectsConfig()
{
    uint8_t errorFlag = 0;
    uint8_t loopIndex;
    uint16_t tempID;
    const CAN_MessageObject_cfgType * CfgPtr;

    for (loopIndex = 0; loopIndex < TOTAL_MESSAGE_OBJECT_NUMBER; loopIndex++)
    {
        CfgPtr = &CAN_MessageObject_configParameters[loopIndex];

        /* Check for user's input */
        if((CfgPtr->unitId== CAN0) || (CfgPtr->unitId == CAN1))
        {
            if ((CfgPtr->messageObjectType == transfer) || (CfgPtr->messageObjectType == receive))
            {
                /* momken neb2a ngama3hom fel a5er b hexadecimal no */
                /* Transfer the data in the CANIFn registers to the CAN message object.*/
                /* Clear the register from previous configurations */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xFF);

                /* Enable WRNRD bit to transfer data from interface registers to the message object to be specified by MNUM */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 7;

                /* Transfer Mask, arbitration, and control bits */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 5; /* ARB */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 4; /* CONTROL */
                // CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1 << 2);

                /* WHICH DATA BYTES TO ACCESS */ /* REVIEW THIS */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 1; /* DATAA */
                CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 0; /* DATAB */

                /* Enable/Disable Mask */
                if (CfgPtr->enableMSK == enable)
                {
                    /* Access Mask Bits */
                    CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 6; /* MASK */
                    /* Use Acceptance Mask */
                    CAN_IFnMCTL (CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 12; /* UMASK */

                    /* Use the proper mask identifier size and assign them to the registers */
                    if (CAN_MessageObject_configParameters[loopIndex].maskIdSize == stdId)
                    {
                        /* Reset the register */
                        CAN_IFnMSK1(CfgPtr->unitId,CfgPtr->messageObjectType) |= 0xFFFF;
                        /* Clear bits 12:2 */
                        CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) &= (0xFFFF0003);
                        /* Update the 12:2 bits with the mask ID */
                        CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) |= (CfgPtr->maskID) << 2;
                    }
                    else if (CfgPtr->maskIdSize == extendedId)
                    {
                        /* Take the LSBs of the Mask */
                        tempID = (CfgPtr->maskID & 0x0000FFFF);
                        /* Reset the register */
                        CAN_IFnMSK1(CfgPtr->unitId,CfgPtr->messageObjectType) &= 0x0000;
                        /* Update the register with the 16 LSBs */
                        CAN_IFnMSK1(CfgPtr->unitId,CfgPtr->messageObjectType) |= tempID;
                        /* Take the MSBs of the mask and shift them right */
                        tempID = (CfgPtr->maskID & 0xFFFF0000) >> 16;
                        /* Reset the register */
                        CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xFFFF);
                        /* Update the register with the 16 MSBs of the mask */
                        CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) |= tempID;
                        /* Use the Mask extended ID for acceptance filtering */
                        CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) |= CfgPtr->enableMaskExtID << 15; /* MXTD */
                    }

                    else
                    {
                        errorFlag++;
                    }

                    /* Use/Don't use mask direction and mask extended identifier bits in acceptance filtering */
                    CAN_IFnMSK2(CfgPtr->unitId,CfgPtr->messageObjectType) |= CfgPtr->enableMaskMessageDirection << 14; /* MDIR */

                }
                else if (CfgPtr->enableMSK == disable)
                {
                    /* Do not use the Acceptance Mask */
                    CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1 << 12); /* UMASK */
                }
                else
                {
                    errorFlag++;
                }

                /* Set message object identifier */
                if(CAN_MessageObject_configParameters[loopIndex].IdSize == stdId)
                {
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) = 0x0000;
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) = (CfgPtr->messageObjectID) << 2;
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1 << 14);
                }
                else if (CAN_MessageObject_configParameters[loopIndex].IdSize == extendedId)
                {
                    tempID = (CAN_MessageObject_configParameters[loopIndex].messageObjectID & 0x0000FFFF);
                    CAN_IFnARB1(CfgPtr->unitId,CfgPtr->messageObjectType) = tempID;
                    tempID = (CfgPtr->messageObjectID & 0xFFFF0000) >> 16;
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) = tempID;
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 14;
                }
                else
                {
                    errorFlag++;
                }

                /* Set the direction of the message object */
                if (CfgPtr->messageObjectType == transfer)
                {
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 13; /* DIR */
                }
                else
                {
                    CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1 << 13);
                }


                /* Set the validity of the message object */
                CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1 << 15);
                CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) |= CAN_MessageObject_configParameters[loopIndex].messageValidity << 15; /* MSGVAL */

                /* Remote enable/Disable */
                if(CfgPtr->messageObjectType == transfer)
                {
                    CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1<< 9);
                    CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) |= CfgPtr->remoteEnable << 9; /*RMTEN */
                }
                else
                {
                    /* do nothing */
                }

                /* End of buffer enable/Disable */
                CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(1<< 7);
                CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) |= CfgPtr->endOfBuffer << 7; /* EOB */

                /* Specify the number of bytes in the data frame */
                if(CfgPtr->dataLengthCode <= 8)
                {
                    CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xF);
                    CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) |= CfgPtr->dataLengthCode;
                }
                else
                {
                    errorFlag++;
                }

                /* Set Message Object Number */
                if ((CfgPtr->messageObjectNumber >= 1) && (CfgPtr->messageObjectNumber <= 30))
                {
                    CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;
                }
                else
                {
                    errorFlag++;
                }
            }
        }
        else
        {
            errorFlag++;
        }
    }

    return errorFlag ;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/


void  CAN_Write_TX (uint8_t MessageObject_configID, uint8_t* DataPtr)
{
    const   CAN_MessageObject_cfgType* CfgPtr;
    CfgPtr = &CAN_MessageObject_configParameters[MessageObject_configID];
    uint8_t loopIndex = 0;
    uint16_t wordNumber = 0;
    uint16_t byteShift = 0;

    if ((initFlag[CfgPtr->unitId] == 1) && (CfgPtr->messageObjectType == transfer))
    {
        /* Reset the CAN_IFnCMSK register to clear ARB and CONTROL bits */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xFF);

        /* Set only WRNRD, DATAA and DATAB to transfer data */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 7); /* WRNRD */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 0); /* DATAB */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 1); /* DATAB */

        /* Clear all data bytes */
        CAN_IFnDB1(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDB2(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDA1(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDA2(CfgPtr->unitId,transfer) = 0x0000;

        for (loopIndex = 0; loopIndex < (CfgPtr->dataLengthCode); loopIndex++)
        {
            wordNumber = (uint16_t)loopIndex/2;
            byteShift = loopIndex % 2 ;
            // Even byte is stored in the low byte
            // byte 0 is the first byte to be transmitted
            // the MSB of each byte is transmitted first.
            CAN_IFnDATA(CfgPtr->unitId,CfgPtr->messageObjectType,wordNumber) |= (*(DataPtr + loopIndex)) << (8*byteShift);
        }

        /* Set the new data (NEWDAT) bit and the transmit request bit AT THE SAME TIME */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 2;

        /* AFTER loading data send it to the message object memory, It is now ready to be transmitted */
        CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;
    }
    else
    {
        /* do nothing */
    }
    while(CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) & 0x00008000);



}


void CAN_WriteData (uint8_t MessageObject_configID, uint8_t* DataPtr)
{
    const   CAN_MessageObject_cfgType* CfgPtr;
    CfgPtr = &CAN_MessageObject_configParameters[MessageObject_configID];
    uint8_t loopIndex = 0;
    uint16_t wordNumber = 0;
    uint16_t byteShift = 0;

    if ((initFlag[CfgPtr->unitId] == 1) && (CfgPtr->messageObjectType == transfer))
    {
        while(CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) & 0x00008000);
        /* Reset the CAN_IFnCMSK register to clear ARB and CONTROL bits */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xFF);

        /* Set only WRNRD, DATAA and DATAB to transfer data */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 7); /* WRNRD */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 0); /* DATAB */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 1); /* DATAB */

        /* Clear all data bytes */
        CAN_IFnDB1(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDB2(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDA1(CfgPtr->unitId,transfer) = 0x0000;
        CAN_IFnDA2(CfgPtr->unitId,transfer) = 0x0000;

        for (loopIndex = 0; loopIndex < (CfgPtr->dataLengthCode); loopIndex++)
        {
            wordNumber = (uint16_t)loopIndex/2;
            byteShift = loopIndex % 2 ;
            // Even byte is stored in the low byte
            // byte 0 is the first byte to be transmitted
            // the MSB of each byte is transmitted first.
            CAN_IFnDATA(CfgPtr->unitId,CfgPtr->messageObjectType,wordNumber) |= (*(DataPtr + loopIndex)) << (8*byteShift);
        }

        /* AFTER loading data send it to the message object memory, It is now ready to be transmitted */
        CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;
    }
    else
    {
        /* do nothing */
    }
}


void test ()
{
    uint8_t i =0;
    const   CAN_MessageObject_cfgType * CfgPtr2 ;
    for(i=1 ; i<4 ; i++)
    {
        CfgPtr2= &CAN_MessageObject_configParameters[i];
        CAN_IFnCMSK(CfgPtr2->unitId,CfgPtr2->messageObjectType) &= ~(0xFF);
        CAN_IFnCMSK(CfgPtr2->unitId,1) &=  ~(1<<7);
        CAN_IFnCMSK(CfgPtr2->unitId,1) |=  (1<<6);
        CAN_IFnCMSK(CfgPtr2->unitId,1) |=  (1<<5);
        CAN_IFnCMSK(CfgPtr2->unitId,1) |=  (1<<4);
        CAN_IFnCMSK(CfgPtr2->unitId,1) |=  (1<<1);
        CAN_IFnCMSK(CfgPtr2->unitId,1) |=  (1<<0);
        CAN_IFnCRQ(CfgPtr2->unitId,1) = CfgPtr2->messageObjectNumber;

    }


}

void  CAN_TX (uint8_t MessageObject_configID)
{
    const   CAN_MessageObject_cfgType* CfgPtr;

   CfgPtr = &CAN_MessageObject_configParameters[MessageObject_configID];
   if ((initFlag[CfgPtr->unitId] == 1) && (CfgPtr->messageObjectType == transfer))
       {
           /* Reset the CAN_IFnCMSK register to clear ARB and CONTROL bits */
           CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~(0xFF);

           /* Set only WRNRD, DATAA and DATAB to transfer data */
           CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 7); /* WRNRD */

           /* Set the new data (NEWDAT) bit and the transmit request bit AT THE SAME TIME */
           CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= 1 << 2;

           /* AFTER loading data send it to the message object memory, It is now ready to be transmitted */
           CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;


       }
   else
       {

       }
}

/*------------------------------------------------- A function that receives data from the CAN bus -----------------------------------*/

void  CAN_RX (uint8_t MessageObject_configID, uint8_t *DataPtr)
{
    uint8_t loopIndex = 0;
    const   CAN_MessageObject_cfgType * CfgPtr;
    CfgPtr = &CAN_MessageObject_configParameters[MessageObject_configID];
    uint16_t wordNumber = 0;
    uint16_t byteShift = 0;
    uint16_t tempData;

    if ((initFlag[CfgPtr->unitId] == 1) && ((CfgPtr->messageObjectType) == receive))
    {
        /* Clear all data bytes */
        CAN_IFnDB1(CfgPtr->unitId,receive) = 0x0000;
        CAN_IFnDB2(CfgPtr->unitId,receive) = 0x0000;
        CAN_IFnDA1(CfgPtr->unitId,receive) = 0x0000;
        CAN_IFnDA2(CfgPtr->unitId,receive) = 0x0000;

        /* Clear the CAN_IFnARB2 register */
        CAN_IFnARB2(CfgPtr->unitId,CfgPtr->messageObjectType) = 0x0000;

        /* Set MASK, ARB, CONTROL, DATAA and DATAB bits */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) = (0x73);

        /* Message Object Number to check for new data */
        CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;

        /* Check for new data */
        if (((CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) & 0x8000) >> 15) == 1)
        {
            for (loopIndex=0 ; loopIndex < (CfgPtr->dataLengthCode) ; loopIndex++)
            {
                wordNumber= (uint16_t) loopIndex/2 ;
                byteShift= loopIndex % 2 ;
                tempData = CAN_IFnDATA(CfgPtr->unitId,1,wordNumber);
                *(DataPtr + loopIndex) = (uint8_t)( tempData >>(8*byteShift));
            }

            /***-----------------------------------------------------------------------*/


        }

        /* Set the NEWDAT/TXRQST bit to clear the new data status */
        CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |= (1 << 2);

        if (((CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) & 0x4000) >> 14) == 1)
        {
            CAN_IFnMCTL(CfgPtr->unitId,CfgPtr->messageObjectType) &= ~ (1 << 14);
        }
        else
        {
            /* do nothing */
        }

        // CAN_IFnCMSK(CfgPtr->unitId,CfgPtr->messageObjectType) |=  (1 << 7); /* WRNRD */

        /* Message Object Number to return the values from the interfaces to the message object memory */
        CAN_IFnCRQ(CfgPtr->unitId,CfgPtr->messageObjectType) = CfgPtr->messageObjectNumber;

    }
    else
    {
        /* do nothing */
    }

    /*-----------------------------------------------*/


}



/*-------------------------------------------------- A function that sends a remote frame ---------------------------------------------*/




/*-----------------------------------------------------------------------------------------------------------------------------------*/
uint8_t newDataFlag(CANUnitId CAN_UnitID, uint8_t messageObjectNumber)
{
    if (messageObjectNumber <= 16)
    {
        return ((CAN_NWDATAn(CAN_UnitID,0) >> (messageObjectNumber - 1)) & 0x01);

    }
    else
    {
        return ((CAN_NWDATAn(CAN_UnitID,1) >> (messageObjectNumber - 1 - 16)) & 0x01);
    }

}


uint8_t txRequestFlag(CANUnitId CAN_UnitID, uint8_t messageObjectNumber)
{
    if (messageObjectNumber <= 16)
    {
        return ((CAN_TXRQn(CAN_UnitID,0) >> (messageObjectNumber - 1)) & 0x01);

    }
    else
    {
        return ((CAN_TXRQn(CAN_UnitID,1) >> (messageObjectNumber - 1 - 16)) & 0x01);
    }

}
