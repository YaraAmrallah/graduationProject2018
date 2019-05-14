/*
 * CAN_cfg.c
 *
 *  Created on: Feb 28, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include "CAN.h"
#include "CAN_cfg.h"

const CAN_cfgType CAN_configParameters[CAN_GROUPS_NUMBER] =
{{CAN0,PB4_8,PB5_8,1000,220,80,200}};

const CAN_MessageObject_cfgType CAN_MessageObject_configParameters[TOTAL_MESSAGE_OBJECT_NUMBER] =
{{CAN0,receive,5,stdId,0xAF,valid,disable,stdId,0x0000,disable,disable,disable,enable,8},
 {CAN0,transfer,2,stdId,0xAD,valid,disable,stdId,0xFA,disable,disable,disable,disable,8},
 {CAN0,receive,7,stdId,0xAD,valid,disable,stdId,0x0000,disable,disable,disable,enable,8},
 {CAN0,transfer,12,stdId,0xFD2,valid,enable,stdId,0xA56,disable,disable,enable,enable,8}/*,
 {CAN0,transfer,10,stdId,0x123,valid,enable,stdId,0x0AA,disable,disable,enable,enable,8}*/};
