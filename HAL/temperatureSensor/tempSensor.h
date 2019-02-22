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

typedef struct
{
    /* Sensor ID among the ADC groups */

    uint8_t tSensor_ID;

    /* Temperature Sensor Sensitivity in mVs */

    int sensorSensitivity;


} tempSensor_ConfigType;

extern const tempSensor_ConfigType tSensorConfigParam [TSENSOR_GROUPS_NUMBER];

tSensor_CheckType tempSensorInitialization ();
tSensor_CheckType tempSensorRequest (uint8_t tSensorIndex);
float returnTempSensorReading (uint8_t tSensorIndex);


#endif /* TEMPSENSOR_H_ */
