#include<gps.h>


#define TOTAL_ARR_LENGTH     1024
#define GPRMCDataLENGTH      28
#define GPRMCIndicatorLength 6
/*
 * TO BE REMOOOOOOOOOOVED
 * WARNING ! TESTING ONLY
 * */

void BlinkLed(uint8_t period,uint8_t groupId);
/*
 *
 *
 *
 */
uint8_t GPSRawData[TOTAL_ARR_LENGTH];
//required data after the NMEA (GPRMC) Command
const uint8_t GPSDataFormat[] = "$GPRMC";
//To put GPRMC data in
uint8_t GPS_DataSentence[GPRMCDataLENGTH];

GPS_State GPSState;


GPS_RetType GPS_Init()
{
    GPS_RetType RetVar;
    //initialed by Done to get a new packet of data.
    GPSState = GPS_READY;
    RetVar = GPS_NOK;
    return RetVar;
}

/**A function to Request GPS Data
 * Works only when Parsing is Done **/
GPS_RetType GPS_ReqData(uint8_t UART_ID)
{
    GPS_RetType RetVar;
    //Check if parsing is finished
    if(GPSState == GPS_DONE_PARSING || GPSState == GPS_READY)
    {
  //Start Receiving the next block of bytes specified with RXLength.
    UART_StartReceiving(UART_ID, GPSRawData, TOTAL_ARR_LENGTH);


    }
    else if(GPSState == GPS_DONE_RECEIVING)
    {
        GPS_GetData(GPSRawData,GPS_DataSentence);
        RetVar = GPS_OK;
    }
    else
    {
        RetVar = GPS_NOK;
    }

    return RetVar;
}


/**This function takes out the GPRMC NMEA sentence out of other
 *  GPS sentences and Provides it in an array. **/
GPS_RetType GPS_GetData(uint8_t InputArray[],uint8_t GPRMCData[])
{
    GPS_RetType RetVar;
    uint8_t Counter1, Counter2, DataFound=1;

    if(GPSState == GPS_DONE_RECEIVING)
    {
        for(Counter1=0; (Counter1<TOTAL_ARR_LENGTH) && (DataFound !=0 ); Counter1++)
    {
        DataFound = memCmp(InputArray + Counter1, GPSDataFormat, GPRMCIndicatorLength);
    }
    //If the Data is found
    if(DataFound == 0)
    {   //Copy the Data Contents to another array.
        for(Counter2 = 0; Counter2 <= GPRMCDataLENGTH; Counter2++)
        {
            GPRMCData[Counter2] =  InputArray[Counter1 + Counter2];
        }

    }
    else
    {
        RetVar = GPS_NOK;
    }
    }
    else
    {
        RetVar = GPS_NOK;
    }
    //Clear the flag to receive again.
    GPSState = GPS_DONE_PARSING;
    return RetVar;
}

//Call back function to be used on the uart ISR.
void GPS_ReceptionDone()
{
     GPSState = GPS_DONE_RECEIVING;
     BlinkLed(20, 0);
}
