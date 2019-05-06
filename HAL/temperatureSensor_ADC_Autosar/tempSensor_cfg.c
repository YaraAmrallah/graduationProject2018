/*
 * tempSensor_cfg.c
 *
 *  Created on: Feb 18, 2019
 *      Author: yaraa
 */

#include "tempSensor_cfg.h"
#include "tempSensor.h"

const tempSensor_ConfigType tSensorConfigParam [TSENSOR_GROUPS_NUMBER] = {{0,10}};

uint8_t tSensorUpdateFlag[TSENSOR_GROUPS_NUMBER]={0};
