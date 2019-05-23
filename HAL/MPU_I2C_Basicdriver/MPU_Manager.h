#ifndef MPUMANGER_H_
#define MPUMANGER_H_
#include <stdint.h>
#include "MPU_Manager_Cfg.h"

/***********************************************
 Type Definitions
 ***********************************************/

typedef void (*MPU_CallBackPtrType)(void);

typedef enum
{
    REQ_VALID, REQ_INVALID
} MPUManager_ReadRetType;
typedef enum
{
    READ_REQ, NO_READ_REQ
} ReadRequest_Flag;
typedef enum
{
    IDLE, READ_IN_PROGRESS, DECISION_IN_PROGRESS
} MPUManager_State;
typedef enum
{
    NO_ACCIDENT = 0, ACCIDENT
} Accident_CheckType;

/*
 **********************************************
 External Variables
 **********************************************
 */

extern const MPU_CallBackPtrType CallBack;

/***********************************************
 Prototypes
 ***********************************************/
void MPUManager_Init(void); //Initialization of all Global Variables
MPUManager_ReadRetType Request_MPU(void); //Requesting to read processed data
void MPU_Cyclic(void);

#endif /* MPUMANGER_H_ */
