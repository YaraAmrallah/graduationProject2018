#ifndef CANMANAGER_H_
#define CANMANAGER_H_

#include <stdint.h>
#include "CANManager_Cfg.h"

typedef struct
{
    uint8_t ObjectID;
    uint8_t StartBit;
    uint8_t EndBit;
    uint8_t SignalDir;
    void (*TxDoneCbkPtr)(void);
} SignalParameters;

typedef enum {CANMgr_NOK, CANMgr_OK,CANMgr_BUSY} CANMgr_CheckType;

extern const SignalParameters SIG_ConfigParam[Signal_Groups_Number];

CANMgr_CheckType CAN_Write(uint8_t SignalID, uint64_t Data);
CANMgr_CheckType CAN_Buffer(uint8_t ObjectID);
CANMgr_CheckType CAN_Read(uint8_t SignalID, uint64_t* Data);
void CAN_checkstatus(void);

#endif
