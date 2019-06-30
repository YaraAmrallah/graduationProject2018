/*
 * CANA_MANAGER.h
 *
 *  Created on: Apr 25, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#ifndef CAN_MANAGER_H_
#define CAN_MANAGER_H_

#include "CAN.h"
#include "CAN_M_Cfg.h"

typedef enum {idle = 0, updateRequest, txRequest, rxRequest}CAN_StateType;
typedef void (*CAN_CallBackPtr)(void);


typedef struct
{
    /* CAN object index */

    uint8_t objectID;

    /* Enable callback function */

    CAN_EnableType callBackPtrEnable;

    /* CAN callback function */

    CAN_CallBackPtr donePointer;

    /* Data frame length */

    uint8_t dataFrameLength;

    /* Receiver call back function */

    CAN_CallBackPtr dataReceivedPointer;

} CAN_Manager_cfgType;


extern const CAN_Manager_cfgType CAN_Manager_configParameters[CAN_MODULES_GROUPS_NUMBER];
//extern uint8_t  doneFlag[CAN_MODULES_GROUPS_NUMBER];

/*--------------------------------------------------------- Function Prototypes --------------------------------------*/
void CAN_MainFunction_Handling ();

void AsyncTxData_Request(uint8_t moduleId,uint8_t * DataPtr);
void AsyncUpdateData_Request(uint8_t moduleId,uint8_t * DataPtr);
void AsyncTx_Request(uint8_t moduleId);
void AsyncRxData_Request(uint8_t moduleId,uint8_t * DataPtr);


#endif /* CAN_MANAGER_H_ */
