

/**
 * main.c
 */
#include "tempSensor.h"

int main(void)
{
    GPIO_Init();
    Adc_Init(&Adc_Config);
    tempSensor_Initialization ();
    float sensorReading;
    uint16_t i = 0;
    tempSensor_EnableStateRequest(0) ;
    while(1)
    {
        tempSensor_MainFunction_Handling ();
         if(tSensorUpdateFlag[0]==1)
         {
             sensorReading = tempSensor_ReturnState(0) ;
         }

         else
         {


         }

        for(i = 0 ; i < 100 ; i++);
    }

    return 0;

}
