#include "CAN.h"
#include "CAN_Cfg.h"
#include "Utilities.h"
#include <stdint.h>

CAN_CheckType CAN_Init(void)
{
    uint8_t LoopIndex;
    uint8_t ErrorFlag = 0;
    CAN_CheckType RetVar;
    for(LoopIndex = 0; (LoopIndex < CAN_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
    {
        if(CAN_ConfigParam[LoopIndex].CAN_Id < CAN_NUMBER)
        {
            /* Enabling CAN_ Clock Gating Control */
            /* If CAN_Id equals to 0 it initializes CAN_0 and if it equals to 1 it initializes CAN_1 */
            RCGC0_CAN |= (CAN_0_EN + CAN_ConfigParam[LoopIndex].CAN_Id);

            /* Setting the INIT bit in CAN_CTL */
            CAN_CTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_CTL_INIT;

            #if Mode_Type == Test_Mode
            /* Setting TEST bit to enable CAN_TST register */
            CAN_CTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_CTL_TEST;
            /* Selecting which mode is used: Basic, Silent or Loopback Mode */
            CAN_TST(CAN_ConfigParam[LoopIndex].CAN_Id) |= Mode_Selector;
            #endif

            if(CAN_ConfigParam[LoopIndex].InterruptEnable == 1)
            {
                /* Setting IE bit to enable CAN_INT register */
                CAN_CTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_CTL_IE ;
                CAN_IF1MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= 0x800;
                //CAN_IF2MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= 0x400;
            }
            /* */

            /* Setting the CCE bit in CAN_CTL */
            CAN_CTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_CTL_CCE;
            /* Setting the bits in CAN_BIT*/
            /*Entering the value of Phase 2 in TSEG2 in the register CAN_BIT*/
            CAN_BIT(CAN_ConfigParam[LoopIndex].CAN_Id) |= ((CAN_ConfigParam[LoopIndex].TSEG2) << CAN_BIT_TSEG2_MASK) & CAN_BIT_TSEG2_EN;
            /*Entering the value of Phase 1 and Propagation in TSEG1 in the register CAN_BIT*/
            CAN_BIT(CAN_ConfigParam[LoopIndex].CAN_Id) |= ((CAN_ConfigParam[LoopIndex].TSEG1) << CAN_BIT_TSEG1_MASK) & CAN_BIT_TSEG1_EN;
            /*Entering the value of SJW in the register CAN_BIT*/
            CAN_BIT(CAN_ConfigParam[LoopIndex].CAN_Id) |= ((CAN_ConfigParam[LoopIndex].SJW) << CAN_BIT_SJW_MASK) & CAN_BIT_SJW_EN;
            /*Entering the value of Baud rate pre-scalar in the register CAN_BIT*/
            CAN_BIT(CAN_ConfigParam[LoopIndex].CAN_Id) |= (CAN_ConfigParam[LoopIndex].BRP) & CAN_BIT_BRP_EN ;
            /*Entering the value of Baud rate pre-scalar extension in the register CAN_BRPE*/
            CAN_BRPE(CAN_ConfigParam[LoopIndex].CAN_Id) |= (CAN_ConfigParam[LoopIndex].BRP) & CAN_BRPE_EN;
            /* Clearing the INIT bit in CAN_CTL */
            CAN_CTL(CAN_ConfigParam[LoopIndex].CAN_Id) &= ~(CAN_CTL_INIT);
            /*Initializing Tx and RX*/
            CAN_TxInit(MSG_ConfigParam[LoopIndex].ModuleType);
            CAN_RxInit(MSG_ConfigParam[LoopIndex].ModuleType);

            RetVar = CAN_OK;
        }
        else
        {
            RetVar = CAN_NOK;
            ErrorFlag = 1;
        }
    }
    return RetVar;
}

CAN_CheckType CAN_TX(uint8_t ChannelID, uint8_t *DataPtr, uint8_t Module)
{
    CAN_CheckType RetVar;
    if(CAN_ConfigParam[ChannelID].CAN_Id < CAN_NUMBER)
    {
        if(MSG_ConfigParam[Module].DLC >= 0x07)
        {
            /* Putting the input data into the data registers */
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= *DataPtr;
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 1)) << 8;
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 2);
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 3)) << 8;
            CAN_IF1DB1(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 4);
            CAN_IF1DB1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 5)) << 8;
            CAN_IF1DB2(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 6);
            CAN_IF1DB2(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 7)) << 8;
        }
        else if(MSG_ConfigParam[Module].DLC >= 0x05)
        {
            /* Putting the input data into the data registers */
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= *DataPtr;
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 1)) << 8;
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 2);
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 3)) << 8;
            CAN_IF1DB1(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 4);
            CAN_IF1DB1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 5)) << 8;
        }
        else if(MSG_ConfigParam[Module].DLC >= 0x03)
        {
            /* Putting the input data into the data registers */
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= *DataPtr;
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 1)) << 8;
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= *(DataPtr + 2);
            CAN_IF1DA2(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 3)) << 8;
        }
        else if(MSG_ConfigParam[Module].DLC >= 0x01)
        {
            /* Putting the input data into the data registers */
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= *DataPtr;
            CAN_IF1DA1(CAN_ConfigParam[ChannelID].CAN_Id) |= (*(DataPtr + 1)) << 8;
        }
        else
        {
            /* Do nothing */
        }

        /* Putting the message priority in MNUM bits in CAN_IF1CRQ */
        /* Range of message priority: 0x01-0x20 */
        CAN_IF1CRQ(CAN_ConfigParam[ChannelID].CAN_Id) = Sort(MSG_ConfigParam[Module].ID);

        TxStart = 1;
        RetVar = CAN_OK;
    }
    else
    {
        RetVar = CAN_NOK;

    }
    return RetVar;
}

CAN_CheckType CAN_RX(uint8_t ChannelID, uint16_t* DataPtr, uint8_t Module)
{
    CAN_CheckType RetVar;
    if(CAN_ConfigParam[ChannelID].CAN_Id < CAN_NUMBER)
    {
        /* Accessing the CAN_IF2CMSK register */
        /* Clearing the WRNRD bit in CAN_IF2CMSK to transfer the message object data to CAN_IF2 registers */
        CAN_IF2CMSK(CAN_ConfigParam[ChannelID].CAN_Id) &= ~(CAN_IFnCMSK_WRNRD);
        /* Setting the MASK bit in CAN_IF2CMSK to specify whether to transfer the IDMASK, DIR and MXTD */
        CAN_IF2CMSK(CAN_ConfigParam[ChannelID].CAN_Id) |= CAN_IFnCMSK_MASK;
        /* Setting the ARB bit in CAN_IF2CMSK to specify whether to transfer the ID, DIR, XTD and MSGVAL */
        CAN_IF2CMSK(CAN_ConfigParam[ChannelID].CAN_Id) |= CAN_IFnCMSK_ARB;
        /* Setting the CONTROL bit in CAN_IF2CMSK to specify whether to transfer the control bits */
        CAN_IF2CMSK(CAN_ConfigParam[ChannelID].CAN_Id) |= CAN_IFnCMSK_CONTROL;
        /* Setting the DATAA and DATAB bits in CAN_IF2CMSK to specify which bits to transfer */
        CAN_IF2CMSK(CAN_ConfigParam[ChannelID].CAN_Id) |= CAN_IFnCMSK_DATA_EN;

        /* Putting the message priority in MNUM bits in CAN_IF1CRQ */
        /* Range of message priority: 0x01-0x20 */
        CAN_IF2CRQ(CAN_ConfigParam[ChannelID].CAN_Id) = Sort(MSG_ConfigParam[Module].ID);

        /* Fetching the received data into DataPtr */
        *DataPtr = CAN_IF2DB2(CAN_ConfigParam[ChannelID].CAN_Id);
        *(DataPtr + 1) = CAN_IF2DB1(CAN_ConfigParam[ChannelID].CAN_Id);
        *(DataPtr + 2) = CAN_IF2DA2(CAN_ConfigParam[ChannelID].CAN_Id);
        *(DataPtr + 3) = CAN_IF2DA1(CAN_ConfigParam[ChannelID].CAN_Id);

        //CAN_IF1MCTL(CAN_ConfigParam[ChannelID].CAN_Id) |= 0x800;
        //CAN_IF2MCTL(CAN_ConfigParam[ChannelID].CAN_Id) |= 0x400;

        RetVar = CAN_OK;
    }
    else
    {
        RetVar = CAN_NOK;
    }
    return RetVar;
}

Status_CheckType CAN_Status(uint8_t ChannelID, uint8_t MessagePriority)
{
    Status_CheckType RetVar;
    /* The CAN_ controller is in bus-off state */
    if(((CAN_STS(CAN_ConfigParam[ChannelID].CAN_Id) & CAN_STS_BOFF_EN) >> CAN_STS_BOFF_MASK) == 1)
    {
        RetVar = BusOff;
    }
    /* The CAN_ controller is not in bus-off state */
    else
    {
        /* Transmission */
        if(MSG_ConfigParam[ChannelID].RTR == 1)
        {
            /* The transmitted message is in progress */
            if (TxStart == 0)
            {
                RetVar = InitialState;
            }
            else
            {
                if(((CAN_STS(CAN_ConfigParam[ChannelID].CAN_Id) & CAN_STS_TXOK_EN) >> CAN_STS_TXOK_MASK) == 0)
                {
                    RetVar = TxInprogress;
                }
                /* The transmitted message is done */
                else
                {
                    RetVar = TxDone;
                    /* Clearing the TXOK bit */
                    CAN_STS(CAN_ConfigParam[ChannelID].CAN_Id) &= ~(CAN_STS_TXOK_EN);
                }
            }
        }
        /* Reception */
        else
        {
            /* The received message is in progress */
            if(((CAN_STS(CAN_ConfigParam[ChannelID].CAN_Id) & CAN_STS_RXOK_EN) >> CAN_STS_RXOK_MASK) == 0)
            {
                RetVar = RxInprogress;
            }
            /* The received message is done */
            else
            {
                RetVar = RxDone;
                /* Clearing the RXOK bit */
                CAN_STS(CAN_ConfigParam[ChannelID].CAN_Id) &= ~(CAN_STS_RXOK_EN);
            }
        }
    }
    return RetVar;
}

void CAN_StopCurrentObject(uint8_t ChannelID)
{
    CAN_CTL(ChannelID) |= CAN_CTL_DAR;
}


CAN_CheckType CAN_TxInit(uint8_t Module)
{
    uint8_t LoopIndex;
    uint8_t ErrorFlag = 0;
    CAN_CheckType RetVar;
    for(LoopIndex = 0; (LoopIndex < CAN_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
    {
        if((CAN_ConfigParam[LoopIndex].CAN_Id < CAN_NUMBER) && ((CAN_IF1CRQ(CAN_ConfigParam[LoopIndex].CAN_Id) & CAN_IFnCRQ_Busy) == 0))
        {
            /* Accessing the CAN_IF1CMSK register */
            /* Setting the WRNRD bit in CAN_IF1CMSK to specify a write to the register */
            CAN_IF1CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_WRNRD;
            /* Setting the ARB bit in CAN_IF1CMSK to specify whether to transfer the ID, DIR, XTD and MSGVAL */
            CAN_IF1CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_ARB;
            /* Setting the CONTROL bit in CAN_IF1CMSK to specify whether to transfer the control bits */
            CAN_IF1CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_CONTROL;
            /* Setting the DATAA and DATAB bits in CAN_IF1CMSK to specify which bits to transfer */
            CAN_IF1CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_DATA_EN;

            /* Accessing the CAN_IF1ARB2 register */
            /* Choosing type of identifier whether is it standard or extended by clearing or setting the XTD bit */
            CAN_IF1ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= MSG_ConfigParam[Module].IDE << CAN_IFnARB2_IDE_MASK  ;
            /* Setting the DIR bit to indicate transmit */
            CAN_IF1ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnARB2_DIR;
            /* Setting the MSGVAL bit to indicate that the message object is valid */
            CAN_IF1ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnARB2_MSGVAL;
            if(MSG_ConfigParam[Module].IDE == 0)
            {
                /* Input the MSGID in ID bits to put the identifier */
                CAN_IF1ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB2_ID_STANDARD_EN) << CAN_IFnARB2_ID_STANDARD_MASK;
            }
            else
            {
                /* Input the MSGID in ID bits to put the identifier */
                CAN_IF1ARB1(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB1_ID_EXTENDED_EN);
                CAN_IF1ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB2_ID_EXTENDED_EN) >> CAN_IFnARB2_ID_EXTENDED_MASK;
            }

            /* Accessing the CAN_IF1MCTL register */
            /* Setting the DLC bits to determine the length of data */
            CAN_IF1MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= MSG_ConfigParam[Module].DLC;
            /* Setting the TXRQST bit in CAN_IF1MCTL to make the data available for transmission */
            CAN_IF1MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnMCTL_TXRQST;

            /* CAN_ is successfully initialized and ready */
            RetVar = CAN_OK;
        }
        else if((CAN_ConfigParam[LoopIndex].CAN_Id < CAN_NUMBER) && ((CAN_IF2CRQ(CAN_ConfigParam[LoopIndex].CAN_Id) & CAN_IFnCRQ_Busy) == CAN_IFnCRQ_Busy))
        {
            /* CAN_ is successfully initialized but it is busy */
            RetVar = CAN_Busy;
        }
        else
        {
            /* CAN_ is not initialized */
            RetVar = CAN_NOK;
            ErrorFlag = 1;
        }
    }
    return RetVar;
}

CAN_CheckType CAN_RxInit(uint8_t Module)
{
    uint8_t LoopIndex;
    uint8_t ErrorFlag = 0;
    CAN_CheckType RetVar;
    for(LoopIndex = 0; (LoopIndex < CAN_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
    {
        if((CAN_ConfigParam[LoopIndex].CAN_Id < CAN_NUMBER) && ((CAN_IF2CRQ(CAN_ConfigParam[LoopIndex].CAN_Id) & CAN_IFnCRQ_Busy) == 0))
        {
            /* Accessing the CAN_IF2CMSK register */
            /* Setting the WRNRD bit in CAN_IF2CMSK to specify a write to the RAM */
            CAN_IF2CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_WRNRD;
            /* Setting the MASK bit in CAN_IF2CMSK to specify whether to transfer the IDMASK, DIR and MXTD */
            CAN_IF2CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_MASK;
            /* Setting the ARB bit in CAN_IF1CMSK to specify whether to transfer the ID, DIR, XTD and MSGVAL */
            CAN_IF2CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_ARB;
            /* Setting the CONTROL bit in CAN_IF1CMSK to specify whether to transfer the control bits */
            CAN_IF2CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_CONTROL;
            /* Setting the DATAA and DATAB bits in CAN_IF2CMSK to specify which bits to transfer */
            CAN_IF2CMSK(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnCMSK_DATA_EN;

            /* Accessing the CAN_IF2ARB2 register */
            /* Choosing identifier type whether standard or extended by clearing or setting XTD bit */
            CAN_IF2ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= MSG_ConfigParam[Module].IDE << CAN_IFnARB2_IDE_MASK;
            /* Clearing the DIR bit to indicate received */
            CAN_IF2ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) &= ~(CAN_IFnARB2_DIR);
            /* Setting the MSGVAL bit to indicate that the message object is valid */
            CAN_IF2ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnARB2_MSGVAL;
            if(MSG_ConfigParam[Module].IDE == 0)
            {
                /* Input the MSGID in ID bits to put the identifier */
                CAN_IF2ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB2_ID_STANDARD_EN ) << CAN_IFnARB2_ID_STANDARD_MASK;
            }
            else
            {
                /* Input the MSGID in ID bits to put the identifier */
                CAN_IF2ARB1(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB1_ID_EXTENDED_EN);
                CAN_IF2ARB2(CAN_ConfigParam[LoopIndex].CAN_Id) |= (MSG_ConfigParam[Module].ID & CAN_IFnARB2_ID_EXTENDED_EN) >> CAN_IFnARB2_ID_EXTENDED_MASK;
            }

            /* Accessing the CAN_IF2MCTL register */
            /* Setting the UMASK bit to enable the mask (MSK, MXTD and MDIR) */
            CAN_IF2MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnMCTL_UMASK;
            /* Setting the EOB bit for a single message object */
            CAN_IF2MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= CAN_IFnMCTL_EOB;
            /* Setting the DLC bits to determine the length of data */
            CAN_IF2MCTL(CAN_ConfigParam[LoopIndex].CAN_Id) |= MSG_ConfigParam[Module].DLC;

            /* CAN_ is successfully initialized and ready */
            RetVar = CAN_OK;
        }
        else if((CAN_ConfigParam[LoopIndex].CAN_Id < CAN_NUMBER) && ((CAN_IF2CRQ(CAN_ConfigParam[LoopIndex].CAN_Id) & CAN_IFnCRQ_Busy) == CAN_IFnCRQ_Busy))
        {
            /* CAN_ is successfully initialized but it is busy */
            RetVar = CAN_Busy;
        }
        else
        {
            /* CAN_ is not initialized */
            RetVar = CAN_NOK;
            ErrorFlag = 1;
        }
    }
    return RetVar;
}
