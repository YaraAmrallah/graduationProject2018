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
GPS_DataLock GPS_DataLocked = GPS_UNLOCKED;

/*********************CAN Flag**********************************/
uint8_t Can_Sent = 1;

#ifdef DEBUG_MODE

void BlinkLed(uint8_t period,uint8_t groupId);

#endif

/*------------------------------------------- GPS Data -------------------------------------------*/

static const uint8_t GPSDataFormat[] = "$GPRMC";
static const uint8_t GPSDataValidIndicator = 'V';

//Store the contents of DMA Data
uint8_t GPSRawData[TOTAL_ARR_LENGTH];

//To put GPRMC sentence only
uint8_t GPS_DataSentence[CIRC_BUFF_LENGTH][GPRMC_DATA_LENGTH];

uint8_t GPS_Sening_Buff[GPRMC_DATA_LENGTH];
/*------------------------------------------- State Variables -------------------------------------------*/

static GPS_State GPSState = GPS_IDLE;

static GPS_Check_Init GPS_Flag_Init;
static GPS_Received GPS_DataReceived;

GPS_RetType GPS_Init()
{
    GPS_RetType RetVar;
    GPS_Flag_Init = GPS_INIT;
    GPS_DataReceived = GPS_DATA_NOTRECEIVED;
    RetVar = GPS_OK;
    return RetVar;
}


/*
 * This Function Provides A full Control on the state machine of the GPS according to the software behavior
 * ie: The flags conditions.
 */
void GPS_Main_Function_Handling()
{
    GPS_RetType RetVar = GPS_NOK;
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
                   case GPS_IDLE:
                       if(GPS_Flag_Init == GPS_INIT)
                           {
                               GPSState = GPS_INITIALIZED;
                           }
                       else
                           {
                             /* do nothing */
                           }
                   break;


                   case GPS_INITIALIZED:
                       //Request Data through DMA.
                       GPS_ReqData(UsedUartModuleNum);
                       GPSState = GPS_READY;
                       break;


                   case GPS_READY:
                       //Waiting For Data Through DMA interrupt over UART.
                       if((GPS_DataReceived == GPS_DATA_RECEIVED))
                           {
                               GPSState = GPS_DONE_RECEIVING;
                               GPS_DataReceived = GPS_DATA_NOTRECEIVED;

                           }
                       else
                       {
                           /* do nothing */
                       }
                       break;


                   case GPS_DONE_RECEIVING:
                       //Generate Another DMA Request.
                       GPS_ReqData(UsedUartModuleNum);
                       //Parse the Received Data
                       RetVar = GPS_Parse(GPS_ActiveAddress,GPS_DataSentence);
                       if(RetVar  == GPS_OK)
                       {
                       GPSState = GPS_DONE_PARSING;
                       }
                       else
                       {
                           //Generate another Request.
                           GPSState = GPS_INITIALIZED;
                       }
                       break;


                   case GPS_DONE_PARSING:
                       //Lock the buffer from changing Data
                       GPS_DataLocked = GPS_LOCKED;
                       if(Can_Sent == 1)
                       {
                       AsyncTxData_Request(0,GPS_Sening_Buff);
                       Can_Sent = 0;
                       }
                       //Generate another Request.
                       GPSState = GPS_INITIALIZED;
                       break;


                   default:
                       /*
                        * Do nothing
                        */

       }
}

/**A function to Request GPS Data
 * Works only when Parsing is Done **/
GPS_RetType GPS_ReqData(uint8_t UART_ID)
{
    GPS_RetType RetVar;
    //Check if parsing is finished
    if ((GPSState == GPS_INITIALIZED))
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
    GPS_RetType RetVar = GPS_OK;
    uint8_t Counter2;
    uint8_t DataFound = 1;
    uint8_t CommaCounter;
    uint8_t ValidFlag = 0;
    uint16_t Counter1;
    if (GPSState == GPS_DONE_RECEIVING)
    {
        /*
         * ToDo checks
         */
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
                RetVar = GPS_OK;
                for (Counter2 = 0;
                        ((Counter2 < MAX_INDICATOR_OFFSET) && (CommaCounter < MAX_COMMA_COUNTS) && (RetVar == GPS_OK));
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
                            RetVar = GPS_OK;
                            break;
                        }
                        else
                        {
                            RetVar = GPS_NOK;
                        }
                    }
                    else
                    {
                        /* Do nothing */
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
        {
            // Preventing Data to be written on the locked buffer
            /*
             * ToDo
             */
            //Copy the Data Contents to another array.
            for (Counter2 = 0; Counter2 < GPRMC_DATA_LENGTH; Counter2++)
            {
                GPRMCData[GPS_CircularBufferCount][Counter2] = InputArray[Counter1 + Counter2];
            }

            //Store the data on a safe place to prevent overWriting
            for (Counter2 = 0; Counter2 < GPRMC_DATA_LENGTH; Counter2++)
            {
                if(GPS_DataLocked == GPS_UNLOCKED)
                {
                GPS_Sening_Buff[Counter2] =
                        GPRMCData[GPS_CircularBufferCount][Counter2];

                }
            }
            //Increment the Main Counter
            GPS_CircularBufferCount ++;

            #ifdef DEBUG_MODE
            BlinkLed(20, 6);
            #endif
            //Looping over circular buffer.
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
    GPS_DataReceived = GPS_DATA_RECEIVED;
    GPS_RawDataCircBufferCount ++;

#ifdef DEBUG_MODE
    BlinkLed(20, 0);
#endif

}

void GPS_DoneSending()
{
    //unlock the locked buffer

    BlinkLed(20, 3);
    // unlock the buffer after sending
    GPS_DataLocked = GPS_UNLOCKED;
    Can_Sent = 1;
}
