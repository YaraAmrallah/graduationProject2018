#include "CANManager.h"
#include "CANManager_Cfg.h"
#include "CAN.h"
#include "CAN_Cfg.h"
#include "Utilities.h"
#include <stdint.h>

#define MessageID        32
#define ByteID           8


#define Shift_8              8
#define Shift_16             16
#define Shift_24             24
#define Shift_32             32
#define Shift_40             40
#define Shift_48             48
#define Shift_56             56


static uint8_t DataArray[MessageID][ByteID];
static uint8_t Status [MessageID];

CANMgr_CheckType CAN_Write(uint8_t SignalID, uint64_t Data)
{
    CANMgr_CheckType RetVar;
    uint8_t* DataArrayPtr;
    DataArrayPtr = &DataArray[SIG_ConfigParam[SignalID].ObjectID][0];
    if(SignalID < Signal_Groups_Number)
    {
        if(SIG_ConfigParam[SignalID].SignalDir == 0)
        {
            Data = Data <<(SIG_ConfigParam[SignalID].StartBit);

            *DataArrayPtr |= Data & 0xFF;
            *(DataArrayPtr + 1) |= ((Data & 0xFF00) >> Shift_8);
            *(DataArrayPtr + 2) |= ((Data & 0xFF0000) >> Shift_16);
            *(DataArrayPtr + 3) |= ((Data & 0xFF000000) >> Shift_24);
            *(DataArrayPtr + 4) |= ((Data & 0xFF00000000) >> Shift_32);
            *(DataArrayPtr + 5) |= ((Data & 0xFF0000000000) >> Shift_40);
            *(DataArrayPtr + 6) |= ((Data & 0xFF000000000000) >> Shift_48);
            *(DataArrayPtr + 7) |= ((Data & 0xFF00000000000000) >> Shift_56);
        }
        else
        {
            /* Do nothing */
        }
        RetVar = CANMgr_OK;
    }
    else
    {
        RetVar = CANMgr_NOK;
    }
    return RetVar;
}

CANMgr_CheckType CAN_Buffer(uint8_t ObjectID)
{
    CANMgr_CheckType RetVar;
    uint8_t* DataArrayPtr;
    uint8_t flag;
    DataArrayPtr = &DataArray[ObjectID][0];
    if(ObjectID < MessageID)
    {
        flag = CAN_Status(CANChannel, Sort(ObjectID));
        if((flag == TxDone) || (flag == InitialState))
        {
            CAN_TX(CANChannel, DataArrayPtr, ObjectID);
            Status[ObjectID] = TxDone;
            RetVar = CANMgr_OK;
        }
        else
        {
            RetVar = CANMgr_BUSY;
        }
    }
    else
    {
        RetVar = CANMgr_NOK;
    }
    return RetVar;
}

CANMgr_CheckType CAN_Read(uint8_t SignalID, uint64_t* Data)
{
    CANMgr_CheckType RetVar;
    uint8_t* DataArrayPtr;
    uint64_t TempData;
    uint8_t LoopIndex;
    uint8_t LoopIndex1;
    if(SignalID < Signal_Groups_Number)
    {
        TempData = 0;
        DataArrayPtr = &DataArray[SIG_ConfigParam[SignalID].ObjectID][0];
        TempData |= *DataArrayPtr;
        for(LoopIndex = 1; LoopIndex < 8; LoopIndex++)
        {
            for(LoopIndex1 = 0; LoopIndex1 <= LoopIndex ; LoopIndex1++)
            {
                TempData |= (*(DataArrayPtr + LoopIndex) << Shift_8);
            }
        }
        *Data = ((TempData << (63 - SIG_ConfigParam[SignalID].EndBit)) >> ((63 - (SIG_ConfigParam[SignalID].EndBit - SIG_ConfigParam[SignalID].StartBit))));
        RetVar = CANMgr_OK;
    }
    else
    {
        RetVar = CANMgr_NOK;
    }
    return RetVar;
}

void CAN_checkstatus(void)
{
    uint8_t LoopIndex;
    for(LoopIndex = 0; LoopIndex < Signal_Groups_Number ; LoopIndex ++)
    {
        if (Status[SIG_ConfigParam[LoopIndex].ObjectID] == TxDone)
        {
            SIG_ConfigParam[LoopIndex].TxDoneCbkPtr();
        }
    }
}

