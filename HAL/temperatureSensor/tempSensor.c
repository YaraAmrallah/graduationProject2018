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
uint8_t flag = 0;

/*-------------------------------------------------------- Functions -------------------------------------------------------------------*/

/*--------------------------- Function that initializes each sensor group and performs user's inputs checks ----------------------------*/

tSensor_CheckType tempSensorInitialization ()
{
    uint8_t loopIndex;
    tSensor_CheckType retVar = tSensor_NOK;
    Adc_ReturnType checkBuffer = ADC_NOK;

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

        if((checkBuffer == ADC_OK) && (retVar == tSensor_OK ))
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

/*------------------------------ A function that starts the ADC conversion ------------------------------------*/

tSensor_CheckType startADC_conversion(uint8_t tSensorIndex)
{
    Adc_ReturnType checkConversion = ADC_NOK;
    tSensor_CheckType retVar = tSensor_NOK;

    checkConversion = Adc_StartGroupConversion(tSensorIndex);

    if(checkConversion == ADC_OK)
    {
        retVar = tSensor_OK;
    }
    else
    {
        /* do nothing */
    }

    return retVar;
}

/*------------------------------ A function that requests data from the ADC manager ----------------------------*/

tSensor_CheckType mainTempSensorRequest (uint8_t tSensorIndex)
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
                    dataArray[tSensorIndex][countArray[tSensorIndex]] = (float)(receivedData)*(mappingValue)/(tSensorConfigParam[tSensorIndex].sensorSensitivity*0.001);

                    if(countArray[tSensorIndex] == (NUMBER_OF_SAMPLES-1))
                    {
                        countArray[tSensorIndex] = 0;
                    }
                    else
                    {
                        countArray[tSensorIndex] = countArray[tSensorIndex] + 1;
                    }

                    if(flag < NUMBER_OF_SAMPLES)
                    {
                        flag++;
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


    if(flag == (NUMBER_OF_SAMPLES))
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
