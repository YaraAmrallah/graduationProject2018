

/**
 * main.c
 */
#include "tempSensor.h"

int main(void)
{
    GPIO_Init();
    ADC_Init();
       tempSensorInitialization ();
       float sensorReading;
       uint16_t i = 0;

       while(1)
       {
           tempSensorRequest(0);
           sensorReading = returnTempSensorReading(0);

           for(i = 0 ; i < 100 ; i++);
       }
       return 0;

}
