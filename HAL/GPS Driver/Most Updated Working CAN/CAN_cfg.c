/*
 * CAN_cfg.c
 *
 *  Created on: Feb 28, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include <CAN_NwIdentifiers.h>
#include "CAN.h"
#include "CAN_cfg.h"

const CAN_cfgType CAN_configParameters[CAN_GROUPS_NUMBER] =
{{CAN0,PB4_8,PB5_8,1000,220,80,200}};

const CAN_MessageObject_cfgType CAN_MessageObject_configParameters[TOTAL_MESSAGE_OBJECT_NUMBER] =
{
 {CAN0,transfer,5,stdId,GPS_FRAME,valid,disable,stdId,0x0000,disable,disable,disable,disable,8},
 {CAN0,receive,1,stdId,START,valid,disable,stdId,0x0000,disable,disable,disable,disable,8},
 {CAN0,receive,5,stdId,END,valid,disable,stdId,0x0000,disable,disable,disable,disable,8}

};
