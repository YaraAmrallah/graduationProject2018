/*
 * CAN_M_Cfg.c
 *
 *  Created on: Apr 25, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include "CAN_Manager.h"

void GPS_DoneSending();


const CAN_Manager_cfgType CAN_Manager_configParameters[CAN_MODULES_GROUPS_NUMBER] =
{{1,enable,GPS_DoneSending,56}};
