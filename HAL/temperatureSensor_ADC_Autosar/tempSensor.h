/*
 * tempSensor.h
 *
 *  Created on: Feb 18, 2019
 *      Author: yaraa
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include <stdint.h>
#include <stdlib.h>
#include "tempSensor_cfg.h"
#include "GPIO.h"
#include "ADC.h"

typedef enum {tSensor_OK = 0, tSensor_NOK = 1} tSensor_CheckType;
typedef enum {tSensorIdle = 0, tSensorBusy, tSensorReady} tSensorStatus;

#define ADC_SAMPLING_VALUE 4095

typedef struct
{
    /* Sensor ID among the ADC groups */

    uint8_t tSensor_ID;

    /* Temperature Sensor Sensitivity in mVs */

    int sensorSensitivity;


} tempSensor_ConfigType;

extern const tempSensor_ConfigType tSensorConfigParam [TSENSOR_GROUPS_NUMBER];
extern uint8_t tSensorUpdateFlag[TSENSOR_GROUPS_NUMBER];

tSensor_CheckType tempSensor_Initialization ();
void tempSensor_EnableStateRequest(uint8_t tSensorIndex);
void tempSensor_DiableStateRequest(uint8_t tSensorIndex) ;
void tempSensor_MainFunction_Handling ();
static void tempSensor_StateCalculation (uint8_t tSensorIndex);
float tempSensor_ReturnState (uint8_t buttonIndex);


#endif /* TEMPSENSOR_H_ */
