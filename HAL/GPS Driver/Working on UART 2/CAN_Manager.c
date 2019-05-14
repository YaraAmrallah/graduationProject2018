/*
 * CANA_MANAGER.c
 *
 *  Created on: Apr 25, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#include "CAN_Manager.h"
#include "UART.h"

/*------------------------------------------------- Global Variables ----------------------------------------------*/

CAN_StateType CAN_StatesArray[CAN_MODULES_GROUPS_NUMBER] = {idle};

uint8_t  requestFlag[CAN_MODULES_GROUPS_NUMBER] = {0};
uint8_t  updateDataArray[CAN_MODULES_GROUPS_NUMBER] = {0};
uint8_t  newDataRecFlag[CAN_MODULES_GROUPS_NUMBER] = {0};

uint8_t* moduleDataArray[CAN_MODULES_GROUPS_NUMBER] = {0};
uint8_t  doneFlag[CAN_MODULES_GROUPS_NUMBER] = {0};

uint8_t dataLength[CAN_MODULES_GROUPS_NUMBER] = {0};
uint8_t incrementValue[CAN_MODULES_GROUPS_NUMBER] = {0};

uint8_t pendingFlag[CAN_MODULES_GROUPS_NUMBER] = {0};
/*---------------------------------------------------- Functions --------------------------------------------------*/


void CAN_MainFunction_Handling ()
{


    uint8_t loopIndex;
    uint8_t i = 0;

    for(loopIndex = 0; loopIndex < CAN_MODULES_GROUPS_NUMBER; loopIndex++)
    {
        const CAN_Manager_cfgType* mConfigPtr = &CAN_Manager_configParameters[loopIndex];
        const CAN_MessageObject_cfgType* configPtr = &CAN_MessageObject_configParameters[mConfigPtr->objectID];

        switch (CAN_StatesArray[loopIndex])
        {

        case idle:
            if(configPtr->messageObjectType == transfer)
            {
                if ((updateDataArray[loopIndex] == 1) && (requestFlag[loopIndex] == 1))
                {

                    CAN_StatesArray[loopIndex] = txRequest;
                    doneFlag[loopIndex] = 0;
                }
                else if((updateDataArray[loopIndex] == 1) && (requestFlag[loopIndex] == 0))
                {
                    CAN_StatesArray[loopIndex] = updateRequest;
                    doneFlag[loopIndex] = 0;
                }
                else
                {
                    /* do nothing */
                }
            }
            else if(configPtr->messageObjectType == receive)
            {
                if (requestFlag[loopIndex] == 1)
                {
                    CAN_StatesArray[loopIndex] = rxRequest;
                    doneFlag[loopIndex] = 0;
                }
                else
                {
                    /* do nothing */
                }

                if(newDataFlag(configPtr->unitId, configPtr->messageObjectNumber) == 1)
                {
                    newDataRecFlag[loopIndex] = 1;
                }
                else
                {
                    /* do nothing */
                }
            }

            break;

        case updateRequest:

            CAN_WriteData (configPtr->messageObjectNumber,moduleDataArray[loopIndex]);
            updateDataArray[loopIndex] = 0;
            CAN_StatesArray[loopIndex] = txRequest;
            break;

        case txRequest:

            if ((updateDataArray[loopIndex] == 1) && (requestFlag[loopIndex] == 1))
            {
                if(dataLength[loopIndex] > configPtr->dataLengthCode)
                {
                    if(pendingFlag[loopIndex] == 0)
                    {
                        if(txRequestFlag(configPtr->unitId, configPtr->messageObjectNumber) == 0)
                        {

                            dataLength[loopIndex] = dataLength[loopIndex] - configPtr->dataLengthCode;


                            CAN_Write_TX (mConfigPtr->objectID, (moduleDataArray[loopIndex] + incrementValue[loopIndex]));



                            incrementValue[loopIndex] += configPtr->dataLengthCode;
                            //GPIO_Write(3,0xff);
                            //  GPIO_Write(3,0x00);

                        }
                        else
                        {
                            pendingFlag[loopIndex] = 1;
                        }
                    }
                    else
                    {
                        if(txRequestFlag(configPtr->unitId, configPtr->messageObjectNumber) == 0)
                        {
                            pendingFlag[loopIndex] = 0;
                        }
                        else
                        {
                            /* do noting */
                        }

                    }
                }
                else
                {

                    /*----------------------------------------------------------------*/
                    if(pendingFlag[loopIndex] == 0)
                    {

                        dataLength[loopIndex] = dataLength[loopIndex] - configPtr->dataLengthCode;
                        CAN_Write_TX (mConfigPtr->objectID, moduleDataArray[loopIndex] + incrementValue[loopIndex]);


                        if(txRequestFlag(configPtr->unitId, configPtr->messageObjectNumber) == 0)
                        {
                            pendingFlag[loopIndex] = 0;



                            doneFlag[loopIndex] = 1;
                            requestFlag[loopIndex] = 0;
                            updateDataArray[loopIndex] = 0;
                            incrementValue[loopIndex]=0;


                            if (mConfigPtr->callBackPtrEnable == enable)
                            {
                                //GPIO_Write(3, 0xFF);
                                mConfigPtr->donePointer();
                            }
                            else
                            {
                                /* do nothing */
                            }

                            CAN_StatesArray[loopIndex] = idle;

                        }
                        else
                        {
                            /* do nothing */
                            pendingFlag[loopIndex] = 1;
                        }
                    }
                    else
                    {
                        if(txRequestFlag(configPtr->unitId, configPtr->messageObjectNumber) == 0)
                        {
                            pendingFlag[loopIndex] = 0;



                            doneFlag[loopIndex] = 1;
                            requestFlag[loopIndex] = 0;
                            updateDataArray[loopIndex] = 0;
                            incrementValue[loopIndex]=0;







                            if (mConfigPtr->callBackPtrEnable == enable)
                            {
                                mConfigPtr->donePointer();
                            }
                            else
                            {
                                /* do nothing */
                            }

                            CAN_StatesArray[loopIndex] = idle;

                        }
                        else
                        {
                            /* do nothing */
                            pendingFlag[loopIndex] = 1;
                        }

                    }

                }

            }
            else if ((updateDataArray[loopIndex] == 0) && (requestFlag[loopIndex] == 1))
            {
                CAN_TX (mConfigPtr->objectID);
                if(txRequestFlag(configPtr->unitId, configPtr->messageObjectNumber) == 0)
                {
                    doneFlag[loopIndex] = 1;
                    if (mConfigPtr->callBackPtrEnable == enable)
                    {
                        mConfigPtr->donePointer();
                    }
                    else
                    {
                        /* do nothing */
                    }
                    CAN_StatesArray[loopIndex] = idle;

                }
                else
                {
                    /* do nothing */
                }

            }


            break;

        case rxRequest:


            if(dataLength[loopIndex] > configPtr->dataLengthCode)
            {


                if(newDataFlag(configPtr->unitId, configPtr->messageObjectNumber) == 1)
                {

                    dataLength[loopIndex] = dataLength[loopIndex] - configPtr->dataLengthCode;

                    CAN_RX (mConfigPtr->objectID, (moduleDataArray[loopIndex] + incrementValue[loopIndex]));

                    incrementValue[loopIndex] += configPtr->dataLengthCode;


                }
                else
                {

                }
            }
            else
            {
                if(newDataFlag(configPtr->unitId, configPtr->messageObjectNumber) == 1)
                {

                    dataLength[loopIndex] = dataLength[loopIndex] - configPtr->dataLengthCode;
                    CAN_RX (mConfigPtr->objectID, moduleDataArray[loopIndex] + incrementValue[loopIndex]);


                    doneFlag[loopIndex] = 1;
                    requestFlag[loopIndex]=0;
                    incrementValue[loopIndex]=0;

                    if (mConfigPtr->callBackPtrEnable == enable)
                    {
                        mConfigPtr->donePointer();
                    }
                    else
                    {
                        /* do nothing */
                    }
                    CAN_StatesArray[loopIndex] = idle;

                }
                else
                {
                    /* do nothing */
                }

            }

            /*----------------------------------------------------------------------------------------------------------*/


        }
    }

}

// data frame length el mafroud tetzabat fel transmit, flag fel main y decrement by 8 le7ad ma yewsal l zero, wa2taha yewa2af el transmission,
// w momken neshouf el FIFO buffer, w wa2t ma n5alas ne3mel el done flag aw el call back function, yeb2a awel 7aga a check 3l length



void AsyncTxData_Request(uint8_t moduleId,uint8_t * DataPtr)
{
    if (dataLength[moduleId] == 0)
    {
        requestFlag[moduleId] = 1;
        updateDataArray[moduleId] = 1;
        moduleDataArray[moduleId] = DataPtr;
        dataLength[moduleId] = CAN_Manager_configParameters[moduleId].dataFrameLength;

    }
    else
    {
        /* do nothing */
    }

}

void AsyncUpdateData_Request(uint8_t moduleId,uint8_t * DataPtr)
{
    moduleDataArray[moduleId] = DataPtr;
    updateDataArray[moduleId] = 1;
}

void AsyncTx_Request(uint8_t moduleId)
{
    requestFlag[moduleId] = 1;
}


void AsyncRxData_Request(uint8_t moduleId,uint8_t * DataPtr)
{
    requestFlag[moduleId] = 1;
    moduleDataArray[moduleId] = DataPtr;
    dataLength[moduleId] = CAN_Manager_configParameters[moduleId].dataFrameLength;

}
