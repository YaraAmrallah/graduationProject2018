#include "I2C.h"
#include "Macros.h"
uint8_t I2C_InitFlag = 0;
I2C_CheckType I2C_Init(void)
{

    I2C_CheckType RetVal;
    const I2C_ConfigType* ConfigPtr;
    uint8_t LoopIndex;

    for (LoopIndex = 0; LoopIndex < I2C_PERIPHERAL_NUMBER; LoopIndex++)
    {

        ConfigPtr = &I2C_ConfigParam[LoopIndex];

        if (((ConfigPtr->I2C_FrequencyOf_SCL <= MAX_SCL_FREQUENCY_SM
                && !ConfigPtr->I2C_SpeedMode)
                || (ConfigPtr->I2C_FrequencyOf_SCL <= MAX_SCL_FREQUENCY_FM
                        && ConfigPtr->I2C_SpeedMode)))
        {
            I2CRCGC |= (((uint32_t) 1) << (ConfigPtr->I2C_Peripheral_ID));
            //I2C1_MCR_R|= (ConfigPtr-> I2C_TYPE);
            I2CMCR(ConfigPtr->I2C_Peripheral_ID) |= (ConfigPtr->I2C_TYPE);
            I2CMTPR(ConfigPtr->I2C_Peripheral_ID) |=
                    (ConfigPtr->I2C_PeripheralFrequency);
            I2CMTPR(ConfigPtr->I2C_Peripheral_ID) |= ((ConfigPtr->I2C_SpeedMode)
                    << 7);

            RetVal = I2C_OK;
            I2C_InitFlag = 1;
        }
        else
        {
            RetVal = I2C_NOK;
            I2C_InitFlag = 0;
        }

    }

    return RetVal;

}

I2C_CheckType I2C_CHECKSTATUS(uint8_t I2CID, I2C_Checkstatus Status)
{

    I2C_CheckType RetVal;
    //const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
    I2C_Checkstatus x = Status;
    switch (x)
    {

    case I2C_SINGLESEND:
    {
        if ((I2CMCS(I2CID) & 1)==1) /*Checks if the I2C Master still busy*/
        {
            RetVal = I2C_SINGLENOK;
        }
        else
        {
            if ((I2CMCS(I2CID) & 0xE)==1) /* if any of these bits are equal 0 (Error || DataAcknowledge || ADress Acknowledge)*/
            {
                RetVal = I2C_SINGLENOK;

            }
            else
            {
                if ((I2CMCS(I2CID) & 0x40)==1) /*Check on the bus busy flag*/
                {
                    RetVal = I2C_SINGLENOK;

                }
                else
                {
                    if ((I2CMCS(I2CID) & 0xE)==1)
                    {
                        RetVal = I2C_SINGLENOK;
                    }
                    else
                    {
                        RetVal = I2C_SINGLEOK;
                    }
                }
            }

        }
    }
        break;

    case I2C_BURSTSEND_STAT:
    {
        if ((I2CMCS(I2CID) & 1)==1)
        {
            RetVal = I2C_BURSTNOK;
        }
        else
        {
            if ((I2CMCS(I2CID) & 0xE)==1)
            {
                RetVal = I2C_BURSTNOK;
            }
            else
            {
                RetVal = I2C_BURSTOK;
            }
        }

    }
        break;
    case MASTER_BUSY:
    {
        if ((I2CMCS(I2CID) & 1)==1)
        {
            RetVal = I2C_SREAD_NOK;
        }
        else
        {
            if ((I2CMCS(I2CID) & 0xE)==1)
            {
                RetVal = I2C_SREAD_NOK;
            }
            else
            {
                RetVal = I2C_SREAD_OK;
            }
        }
    }
        break;

    case I2C_BUSBUSY:

        if ((I2CMCS(I2CID) & 0x40)==1)
        {
            RetVal = I2C_BUSY;
        }
        else
        {
            RetVal = I2C_NOBUSY;
        }
        break;

    case I2C_MBUSY:

        if( (I2CMCS(I2CID) & 1)==1)
        {
            RetVal = I2C_MASTERBUSY;
        }
        else
        {
            RetVal = I2C_MASTERNOBUSY;
        }
        break;

    }
    return RetVal;
}

