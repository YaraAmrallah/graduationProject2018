/*
 * CAN_M_Cfg.c
 *
 *  Created on: Apr 25, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include "CAN_Manager.h"

void nullFunction(void);
void GPS_DoneSending(void);


void START_Notify(void);
void END_Notify(void);

const CAN_Manager_cfgType CAN_Manager_configParameters[CAN_MODULES_GROUPS_NUMBER] =
{
{0,enable,GPS_DoneSending,56,nullFunction},
{1,enable,nullFunction,8,START_Notify},
{2,enable,nullFunction,8,END_Notify}

};
