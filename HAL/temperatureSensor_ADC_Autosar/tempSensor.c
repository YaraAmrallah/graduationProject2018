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

static float dataArray[TSENSOR_GROUPS_NUMBER][NUMBER_OF_SAMPLES];
static uint8_t countArray[TSENSOR_GROUPS_NUMBER] = {0};
static Adc_ValueGroupType ADC_array[TSENSOR_GROUPS_NUMBER] = {0};
static uint8_t flag[TSENSOR_GROUPS_NUMBER] = {0};
static uint8_t tempSensorRequest[TSENSOR_GROUPS_NUMBER] = {0};
static tSensorStatus statusArray[TSENSOR_GROUPS_NUMBER] = {tSensorIdle};
static float tSensorReading[TSENSOR_GROUPS_NUMBER] = {0.0};

/*-------------------------------------------------------- Functions -------------------------------------------------------------------*/

/*--------------------------- Function that initializes each sensor group and performs user's inputs checks ----------------------------*/

tSensor_CheckType tempSensor_Initialization ()
{
    uint8_t loopIndex;
    tSensor_CheckType retVar = tSensor_NOK;
    Std_ReturnType checkBuffer = E_NOT_OK;
    uint8_t initialiazationflag = 0;

    for (loopIndex = 0; loopIndex < TSENSOR_GROUPS_NUMBER ; loopIndex ++)
    {

        if((tSensorConfigParam[loopIndex].tSensor_ID) < (NUMBER_OF_CHANNEL_GROUPS))
        {
            checkBuffer = Adc_SetupResultBuffer(tSensorConfigParam[loopIndex].tSensor_ID, &ADC_array[loopIndex]);
            if(checkBuffer == E_OK)
            {
                initialiazationflag++;
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

        if (initialiazationflag == TSENSOR_GROUPS_NUMBER)
        {
            retVar = tSensor_OK;
        }
        else
        {
            /* do nothing */
        }
    }

    return retVar;
}


/*-------------------------- A function that requests the temperature sensor status ------------------------------*/

void tempSensor_EnableStateRequest(uint8_t tSensorIndex)
{
    tempSensorRequest[tSensorIndex] = 1;
}

/*--------------------- A function that disables the temperature sensor status request --------------------------*/

void tempSensor_DisableStateRequest(uint8_t tSensorIndex)
{
    tempSensorRequest[tSensorIndex] = 0;
    flag[tSensorIndex] = 0;
}

/*------------------------------ A function that requests data from the ADC manager ----------------------------*/

void tempSensor_MainFunction_Handling ()
{
    Adc_StatusType checkStatus;
    Std_ReturnType checkRead = E_NOT_OK;
    Adc_ValueGroupType receivedData;
    uint8_t tSensorIndex = 0;
    float mappingValue = (float)VDDA/(float)ADC_SAMPLING_VALUE;

    for (tSensorIndex = 0 ; tSensorIndex < TSENSOR_GROUPS_NUMBER ; tSensorIndex++)
    {
        switch(statusArray[tSensorIndex])
        {
        case tSensorIdle:

            if(tempSensorRequest[tSensorIndex] == 1)
            {
               // Adc_StartGroupConversion(tSensorIndex);
                tSensorUpdateFlag[tSensorIndex]=0 ;
                statusArray[tSensorIndex] = tSensorBusy;
            }
            else
            {
                /* do nothing */
            }

            break ;

        case tSensorBusy:

            Adc_StartGroupConversion(tSensorIndex);
            checkStatus = Adc_GetGroupStatus(tSensorConfigParam[tSensorIndex].tSensor_ID);
            if(checkStatus == ADC_STREAM_COMPLETED)
            {
                checkRead = Adc_ReadGroup(tSensorConfigParam[tSensorIndex].tSensor_ID, &receivedData);

                if(checkRead == E_OK)
                {
                    dataArray[tSensorIndex][countArray[tSensorIndex]] = (float)(receivedData)*(mappingValue)/(tSensorConfigParam[tSensorIndex].sensorSensitivity*0.001);

                    if(countArray[tSensorIndex] == (NUMBER_OF_SAMPLES-1))
                    {
                        /* reset the counter */
                        countArray[tSensorIndex] = 0;
                    }
                    else
                    {
                        countArray[tSensorIndex] = countArray[tSensorIndex] + 1;
                    }

                    if (flag[tSensorIndex] < (NUMBER_OF_SAMPLES-1))
                    {
                        flag[tSensorIndex] = flag[tSensorIndex] + 1;
                    }
                    else
                    {
                        statusArray[tSensorIndex] =  tSensorReady;
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

            break;

        case tSensorReady:

            tempSensor_StateCalculation(tSensorIndex);
            statusArray[tSensorIndex] = tSensorIdle;
            tSensorUpdateFlag[tSensorIndex]=1 ;

            break;

        }

    }

}

/*---------------------------------- Function that returns the sensor's reading --------------------------------------------------*/

static void tempSensor_StateCalculation (uint8_t tSensorIndex)
{
    uint8_t loopIndex;

    float averagingValue = (float) NUMBER_OF_SAMPLES;

            for(loopIndex = 0; loopIndex < NUMBER_OF_SAMPLES; loopIndex++)
            {
                tSensorReading[tSensorIndex] = tSensorReading[tSensorIndex] + dataArray[tSensorIndex][loopIndex];
            }

            tSensorReading[tSensorIndex] = tSensorReading[tSensorIndex]/averagingValue;
}

/* ------------------------------------------ Return the sensor reading to the user ------------------------------------------*/

float tempSensor_ReturnState (uint8_t tSensorIndex)
{
    float returnSensorReding = tSensorReading[tSensorIndex];
    return returnSensorReding;
}




