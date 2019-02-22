/*
 * tempSensor.c
 *
 *  Created on: Feb 18, 2019
 *      Author: yaraa
 */

#include "ADC.h"
#include "tempSensor.h"
#include "tempSensor_cfg.h"
#include "M4MemMap.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"

/*-------------------------------------------------------- Structures ------------------------------------------------------------------*/
/* Structure that must be defined as global to the button functions in the code */

float dataArray[TSENSOR_GROUPS_NUMBER][NUMBER_OF_SAMPLES];
uint8_t countArray[TSENSOR_GROUPS_NUMBER] = {0};
uint16_t ADC_array[TSENSOR_GROUPS_NUMBER] = {0};

/*-------------------------------------------------------- Functions -------------------------------------------------------------------*/

/*--------------------------- Function that initializes each sensor group and performs user's inputs checks ----------------------------*/

tSensor_CheckType tempSensorInitialization ()
{
    uint8_t loopIndex;
    tSensor_CheckType retVar = tSensor_NOK;
    Adc_ReturnType checkBuffer = ADC_NOK;
    Adc_ReturnType checkConversion = ADC_NOK;

    for (loopIndex = 0; loopIndex < TSENSOR_GROUPS_NUMBER ; loopIndex ++)
    {

        if((tSensorConfigParam[loopIndex].tSensor_ID) < (NUMBER_OF_ADC_GROUPS))
        {
            retVar = tSensor_OK;
        }
        else
        {
            /* do nothing */
        }

        checkBuffer = Adc_SetupResultBuffer(tSensorConfigParam[loopIndex].tSensor_ID, &ADC_array[loopIndex]);
        checkConversion = Adc_StartGroupConversion(tSensorConfigParam[loopIndex].tSensor_ID);

        if((checkBuffer == ADC_OK) && (checkConversion == ADC_OK) && (retVar == tSensor_OK ))
        {
            /* do nothing */
        }
        else
        {
            retVar = tSensor_NOK;
        }
    }

    return retVar;
}

/*------------------------------ A function that requests data from the ADC manager ----------------------------*/

tSensor_CheckType tempSensorRequest (uint8_t tSensorIndex)
{
    tSensor_CheckType retVar = tSensor_NOK;
    Adc_StatusType checkStatus;
    Adc_ReturnType checkRead = ADC_NOK;
    Adc_ValueGroupType receivedData;
    float mappingValue = (float)VDDA/4095;

    if (tSensorIndex < (TSENSOR_GROUPS_NUMBER))
    {
            checkStatus = Adc_GetGroupStatus(tSensorConfigParam[tSensorIndex].tSensor_ID);

            if(checkStatus == ADC_STREAM_COMPLETED)
            {
                checkRead = Adc_ReadGroup(tSensorConfigParam[tSensorIndex].tSensor_ID, &receivedData);

                if(checkRead == ADC_OK)
                {
                    retVar = tSensor_OK;
                    /* N.B.: Precision is multiplied by 100 */
                    dataArray[tSensorIndex][countArray[tSensorIndex]] = (float)(receivedData)*(mappingValue)/(tSensorConfigParam[tSensorIndex].sensorSensitivity*0.001);
                    countArray[tSensorIndex] = countArray[tSensorIndex] + 1;
                }
                else
                {
                    /* do nothing */
                }
            }
            else
            {
                /* do nothing */
            }
    }

    else
    {
        /* do nothing */
    }

    return retVar;
}

/*---------------------------------- Function that returns the sensor's reading --------------------------------------------------*/

float returnTempSensorReading (uint8_t tSensorIndex)
{
    uint8_t loopIndex;
    float tSensorReading = 0.0;
    float averagingValue = (float) NUMBER_OF_SAMPLES;

    if(countArray[tSensorIndex] == (NUMBER_OF_SAMPLES))
    {
        /* reset the counter */
        countArray[tSensorIndex] = 0;

            for(loopIndex = 0; loopIndex < NUMBER_OF_SAMPLES; loopIndex++)
            {
                tSensorReading = tSensorReading + dataArray[tSensorIndex][loopIndex];
            }

            tSensorReading = tSensorReading/averagingValue;
    }
    else
    {
        /* do nothing */
    }

    return tSensorReading;
}



