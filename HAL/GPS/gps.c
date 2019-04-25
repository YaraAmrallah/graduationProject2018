/*
 *  The Design is made in a way that the DMA continously Receive data and put it in an Circular
 *  Buffer And the Parsing function extracts the required data in another Circular Buffer
 *  that sustain up to 8 sentences.
 *
 * GPS_ActiveAddress is the address of the initial Byte to which DMA starts to put data in.
 */


#include<gps.h>

#define GPRMC_INDICATOR_LENGTH      6
#define GPS_VALID_FLAG_OFFSET       2
#define MAX_INDICATOR_OFFSET        20
#define MAX_COMMA_COUNTS            3
#define CIRC_BUFF_LENGTH            8
#define RAWDATA_CIRC_BUFF_LENGTH    2


/*------------------------------------------- Circular Buffer -------------------------------------------*/
uint8_t GPS_CircularBufferCount = 0;
uint8_t GPS_RawDataCircBufferCount = 0;
uint8_t *GPS_ActiveAddress;

#ifdef DEBUG_MODE

void BlinkLed(uint8_t period,uint8_t groupId);

#endif

/*------------------------------------------- GPS Data -------------------------------------------*/

static const uint8_t GPSDataFormat[] = "$GPRMC";
static const uint8_t GPSDataValidIndicator = 'A';

//Store the contents of DMA Data
uint8_t GPSRawData[TOTAL_ARR_LENGTH];

//To put GPRMC sentence only
uint8_t GPS_DataSentence[CIRC_BUFF_LENGTH][GPRMC_DATA_LENGTH];


/*------------------------------------------- State Variables -------------------------------------------*/
static uint8_t GPS_Initialized = 0;
static uint8_t GPS_DataReceived = 0;

static GPS_State GPSState;



GPS_RetType GPS_Init()
{
    GPS_RetType RetVar;
    GPS_Initialized = 1;
    RetVar = GPS_OK;
    return RetVar;
}


/*
 * This Function Provides A full Control on the state machine of the GPS according to the software behavior
 * ie: The flags conditions.
 */
void GPS_Main_Function_Handling()
{

    /*
     * Flags Handling
     */
    if(GPS_Initialized == 1)
    {
        GPSState = GPS_INITIALIZED;
        GPS_Initialized = 0;
    }

    if(GPS_DataReceived == 1)
    {
        GPSState = GPS_DONE_RECEIVING;
        GPS_DataReceived = 0;

    }

    /*
     * Circular BUffer Handling
     */
    if(GPS_RawDataCircBufferCount < RAWDATA_CIRC_BUFF_LENGTH)
    {
        if((GPS_RawDataCircBufferCount % RAWDATA_CIRC_BUFF_LENGTH) == 0)
        {
            GPS_ActiveAddress = GPSRawData;
        }
        else if((GPS_RawDataCircBufferCount % RAWDATA_CIRC_BUFF_LENGTH) == 1)
        {
            GPS_ActiveAddress = GPSRawData + TRANSFER_SIZE;
        }
    }
    else
    {
        GPS_RawDataCircBufferCount = 0;
    }

    /*
     * States Handling
     */
    switch(GPSState)
       {
                   case GPS_INITIALIZED:
                       GPSState = GPS_READY;
                       GPS_ReqData(UsedUartModuleNum);
                       break;

                   case GPS_READY:
                       //GPS_ReqData(UsedUartModuleNum);
                       //Waiting For Data Through DMA interrupt over UART.
                       break;

                   case GPS_DONE_RECEIVING:
                       GPS_Parse(GPS_ActiveAddress,GPS_DataSentence);
                       GPS_ReqData(UsedUartModuleNum);
                       GPSState = GPS_DONE_PARSING;
                       break;

                   case GPS_DONE_PARSING:
                       /*
                        * ToDo
                        * Send
                        * GPS_DataSentence[GPS_CircularBufferCount][GPRMC_DATA_LENGTH];
                        *  through CAN Bus
                        */
                       GPSState = GPS_READY;
                       break;


                   default:
                       /*Do nothing
                        */

       }
}

/**A function to Request GPS Data
 * Works only when Parsing is Done **/
GPS_RetType GPS_ReqData(uint8_t UART_ID)
{
    GPS_RetType RetVar;
    //Check if parsing is finished
    if ((GPSState == GPS_READY) || (GPSState == GPS_DONE_RECEIVING))
    {
        //Start Receiving the next block of bytes specified with RXLength.
        UART_StartReceiving(UART_ID, GPS_ActiveAddress, TRANSFER_SIZE);
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
GPS_RetType GPS_Parse(uint8_t InputArray[],uint8_t GPRMCData[][GPRMC_DATA_LENGTH])
{
    GPS_RetType RetVar;
    uint8_t Counter2;
    uint8_t DataFound = 1;
    uint8_t CommaCounter;
    uint8_t ValidFlag = 0;
    uint16_t Counter1;
    if (GPSState == GPS_DONE_RECEIVING)
    {
        for (Counter1 = 0; ((Counter1 < TRANSFER_SIZE) && (ValidFlag == 0));
                Counter1++)
        {
            /*
             * memCmp -> Returns zero if the required  data is found
             *        -> Otherwise returns non zero integer.
             */
            DataFound = memCmp(InputArray + Counter1, GPSDataFormat,
                               GPRMC_INDICATOR_LENGTH);

            if (DataFound == 0)
            {
                CommaCounter = 0;

                for (Counter2 = 0;
                        (Counter2 < MAX_INDICATOR_OFFSET) && (CommaCounter < MAX_COMMA_COUNTS);
                        Counter2++)
                {
                    if (InputArray[Counter1 + Counter2] == ',')
                    {
                        CommaCounter++;
                    }
                    /*
                     * There are 2 Commas before the first occurrence of The Character that
                     * indicates sentence ValidFlagity
                     */
                    if (CommaCounter == GPS_VALID_FLAG_OFFSET)
                    {
                        //CHECK IF THE next character indicates GPS ValidFlagity.
                        if (InputArray[Counter1 + Counter2 + 1] == GPSDataValidIndicator)
                        {
                            ValidFlag = 1;
                            /*
                             * Counter1 Now is set at the second character in the required sentence
                             * So, It needs to be set to the first character again to Copy the sentence
                             * Then,
                             * Break the first Loop indicating the Required Data is found.
                             */
                            Counter1--;
                            break;
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
                }
            }
            else
            {
                RetVar = GPS_NOK;
            }
        }

        //If the Data is found & ValidFlag
        if(ValidFlag == 1)
        {   //Copy the Data Contents to another array.
            for (Counter2 = 0; Counter2 <= GPRMC_DATA_LENGTH; Counter2++)
            {
                GPRMCData[GPS_CircularBufferCount][Counter2] = InputArray[Counter1 + Counter2];
            }
            GPS_CircularBufferCount++;

            #ifdef DEBUG_MODE
            BlinkLed(20, 6);
            #endif

            if(GPS_CircularBufferCount == CIRC_BUFF_LENGTH)
            {
                GPS_CircularBufferCount = 0;
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

    return RetVar;
}

//Call back function to be used on the uart ISR.
void GPS_ReceptionDone()
{
    GPS_DataReceived = 1;
    GPS_RawDataCircBufferCount ++;
#ifdef DEBUG_MODE
    BlinkLed(20, 0);
#endif

}
