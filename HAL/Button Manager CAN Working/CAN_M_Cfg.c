/*
 * CAN_M_Cfg.c
 *
 *  Created on: Apr 25, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include "CAN_Manager.h"

void nullFunction(void);

void CAN_ReceptionCallBackFunctionL(void);
void CAN_DataReceptionCBL(void);
void CAN_ReceptionCallBackFunctionR(void);
void CAN_DataReceptionCBR(void);
void CAN_SEND_LSERVO_Done(void);
void START_Notify(void);
void CAN_SEND_RSERVO_Done(void);
void END_Notify(void);
void CAN_TEMP_Done(void);
const CAN_Manager_cfgType CAN_Manager_configParameters[CAN_MODULES_GROUPS_NUMBER] =
{{0,enable,CAN_DataReceptionCBL,8,CAN_ReceptionCallBackFunctionL},
{1,enable,CAN_DataReceptionCBR,8,CAN_ReceptionCallBackFunctionR},
{2,enable,CAN_SEND_LSERVO_Done,8,nullFunction},
{3,enable,CAN_SEND_RSERVO_Done,8,nullFunction},
{4,enable,nullFunction,8,START_Notify},
{5,enable,nullFunction,8,END_Notify},
{6,enable,CAN_TEMP_Done,8,nullFunction},

};
