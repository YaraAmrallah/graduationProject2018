#include "I2C.h"
#include "I2C_Cfg.h"
/* Bit fields for Transaction Conditions*/

/*
 **********************************************
         Global Functions
 **********************************************
 */
// Initializing I2C Modules
I2C_CheckType I2C_Init(void)
{

    I2C_CheckType RetVal;
    const I2C_CfgType* CfgPtr;
    uint8_t LoopIndex;

    for (LoopIndex = 0; LoopIndex < I2C_GROUPS_NUMBER; LoopIndex++)
    {

        CfgPtr = &I2C_ConfigParam[LoopIndex];

        if (((CfgPtr->I2C_FrequencyOf_SCL <= MAX_SCL_FREQUENCY_SM
                && !CfgPtr->I2C_SpeedMode)
                || (CfgPtr->I2C_FrequencyOf_SCL <= MAX_SCL_FREQUENCY_FM
                        && CfgPtr->I2C_SpeedMode)))
        {
            I2CRCGC |= (((uint32_t) 1) << (CfgPtr->ChannelId));
            I2CMCR(CfgPtr->ChannelId) |= (CfgPtr->ModuleType);
            I2CMTPR(CfgPtr->ChannelId) |= (CfgPtr->Prescalar);
            I2CMTPR(CfgPtr->ChannelId) |= ((CfgPtr->I2C_SpeedMode) << 7);

            RetVal = I2C_OK;

        }
        else
        {
            RetVal = I2C_NOK;

        }

    }

    return RetVal;

}

I2C_CheckType I2C_SetSlaveAdd(uint8_t SlaveAddress, uint8_t ReadWrite,
                              uint8_t GroupID) //To place Slave Address in MCS register + Value of R/S bit
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType *CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];
    I2CMSA(CfgPtr->ChannelId) &= ~(I2C_Receive);
    I2CMSA(CfgPtr->ChannelId) = (SlaveAddress << 1) | (ReadWrite && 0x01);
    RetVar = I2C_OK;
    return RetVar;
}

I2C_CheckType I2C_SendData(uint8_t* Data, uint8_t GroupID) // the Data in case of Transmission
{

    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType *CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];
    I2CMDR(CfgPtr->ChannelId) = *Data;
    RetVar = I2C_OK;
    return RetVar;

}
I2C_CheckType I2C_ReadData(uint8_t* Data, uint8_t GroupID) //To read the Data in case of Reception
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType *CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    *Data = I2CMDR(CfgPtr->ChannelId);
    RetVar = I2C_OK;
    return RetVar;
}

I2C_CheckType I2C_SetOperation(uint8_t Condition, uint8_t GroupID) //Configuring the suitable  operation
{

    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType *CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];
    I2CMCS(CfgPtr->ChannelId) = Condition;
    RetVar = I2C_OK;

    return RetVar;
}

/*********************************************************************************
 *                       Checking on Flags                                       *
 *                                                                               *
 *                                                                               *
 ***********************************************************************************/
I2C_CheckType I2C_CheckBusStat(uint8_t GroupID) //Checking whether the bus is acquired by another Master/Slave or not
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType * CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    if (((I2CMCS(CfgPtr->ChannelId)) & I2C_BUSBUSY)==1)
    {
        RetVar = I2C_NOK;
    }
    else
    {
        RetVar = I2C_OK;
    }
    return RetVar;

}
I2C_CheckType I2C_CheckBUSY(uint8_t GroupID) //Checking whether the I2C controller is sending/receiving or not
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType * CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    if ((I2CMCS(CfgPtr->ChannelId) & I2C_BUSY)==0)
    {

        RetVar = I2C_OK;
    }

    else
    {
        RetVar = I2C_NOK;
    }
    return RetVar;

}

I2C_CheckType I2C_CheckErrBit(uint8_t GroupID) //Checking for all kinds of error
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType * CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    if ( (I2CMCS(CfgPtr->ChannelId) & I2C_ERROR)==1)
    {
        RetVar = I2C_NOK;
    }
    else
    {
        RetVar = I2C_OK;
    }
    return RetVar;

}

I2C_CheckType I2C_Addcheck(uint8_t GroupID) //Checking for Address ack
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType * CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    if ( (I2CMCS(CfgPtr->ChannelId) & I2C_ADRACK)==1)
    {
        RetVar = I2C_NOK;
    }
    else
    {
        RetVar = I2C_OK;
    }
    return RetVar;

}

I2C_CheckType I2C_Datacheck(uint8_t GroupID) //Checking for Data ack
{
    I2C_CheckType RetVar = I2C_NOK;
    const I2C_CfgType * CfgPtr;
    CfgPtr = &I2C_ConfigParam[GroupID];

    if ( (I2CMCS(CfgPtr->ChannelId) & I2C_DATAACK)==1)
    {
        RetVar = I2C_NOK;
    }
    else
    {
        RetVar = I2C_OK;
    }
    return RetVar;

}
