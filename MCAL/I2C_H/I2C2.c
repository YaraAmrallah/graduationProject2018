#include <stdint.h>
#include "I2C2.h"
#include "I2C2_Cfg.h"
#define I2CS_NUMBER 4u


void I2C_Init2(void)
{
    uint8_t I2CID;
    const I2C_Mode *Mode_ptr;
    for(I2CID=0 ; I2CID<I2CS_NUMBER ; I2CID++)
    {
        Mode_ptr = &I2CMOD_Data[I2CID];
        if(Mode_ptr->Enable == 1)
        {
            RCGC_GPIO |= 1<<(Mode_ptr->PortID);
            GPIODEN_REG(Mode_ptr->PortID) |= (1<<(Mode_ptr->SDA_Pin)) + (1<<(Mode_ptr->SCK_Pin));
            GPIOAFSEL_REG(Mode_ptr->PortID) |= 1<<(Mode_ptr->SDA_Pin);/*SET the AFSEL of the pins*/
            GPIOAFSEL_REG(Mode_ptr->PortID) |= 1<<(Mode_ptr->SCK_Pin);/*SET the AFSEL of the pins*/
            GPIOPCTL_REG(Mode_ptr->PortID) |= I2C_PCTL_VAL << 4*(Mode_ptr->SDA_Pin);
            GPIOPCTL_REG(Mode_ptr->PortID) |= I2C_PCTL_VAL << 4*(Mode_ptr->SCK_Pin);
            GPIOODR_REG(Mode_ptr->PortID) |= 1<<(Mode_ptr->SDA_Pin);

            //RCGC_I2C |= 1<<I2CID;
			I2CRCGC |= (((uint32_t)1) <<I2CID);
            I2CMCR(I2CID) |= MASTER;                 /*Enable as a Master*/
            I2CMTPR(I2CID) |= TPR_Calculator(MAX_SCL_FREQUENCY_SM); /*****************************Clock*****************************/
        }else
        {
         /*Misra*/
        }
    }
}

/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						Basic Functions 			              *
 ******************************************************************************
 */

uint8_t I2C_Send(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data)
{
	/*data is passed as array*/
    int loop;//iteration to loob
    int datacount=0;
    int w=0;//variable to increase the datacount
    for(loop=0;;loop++)/*loop till the if condition is satisfied and it will get you out ,we do this to count the data*/
        {
            datacount=w+1;
            w++;
            if(*(Data+loop+1)=='\0')
            {
                break;
            }
        }

    if(datacount == 1)//single send
        {
            volatile char error=0;
            I2C_SlaveADD_send(I2CID,Slave_address,WRITE);
            I2C_WRITE_DATA(I2CID,*(Data));
            I2C_MasterCTL(I2CID, SINGLE_OP);

            while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
            error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
            while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/
            error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

            if (error) return error;
        
		}
		else
        {
            volatile char error=0;
            int i,k=0;

            I2C_SlaveADD_send(I2CID,Slave_address,WRITE);
            I2C_WRITE_DATA(I2CID,*(Data));
            I2C_MasterCTL(I2CID,BURST_SEND_START);
			
            while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
            error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

            for(i=0;i<datacount-1;i++)
            {
                k=i+1;
                I2C_WRITE_DATA(I2CID,*(Data+k));
                if(k != datacount-1)
                {
                    I2C_MasterCTL(I2CID,BURST_SEND_CONT);
                    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
                    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
                    if (error) return error;
                }else
                {
                    I2C_MasterCTL(I2CID,BURST_DONE);
                    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
                    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
                    if (error) return error;
                }
            }
        }
}


uint8_t I2C_Receive(uint8_t I2CID, uint8_t Slave_address, uint8_t Datacount, uint8_t* Data)
{
    volatile  uint8_t error=0;

    if (Datacount <= 0)
    return -1; /* no read was performed */

    I2C_SlaveADD_send(I2CID,Slave_address,READ); /* set slave address and read mode */

    if (Datacount == 1) /* if last byte, don't ack */
    {
        I2C_MasterCTL(I2CID,SINGLE_OP);
    }
    else /* else ack */
    {
        I2C_MasterCTL(I2CID,BURST_RECEIVE_START);
    }

    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
    if (error) return error;

    I2C_READ_DATA(I2CID,Data); /* store the Data received */


    if (--Datacount == 0) /* if single byte read, done */
    {
    while(I2CMCS(I2CID) & 0x40); /* wait until bus is not busy */
    return 0; /* no error */
    }

    /* read the rest of the bytes */
    while (Datacount > 1)
    {
        I2C_MasterCTL(I2CID,BURST_RECEIVE_CONT); /* -Data-ACK- */

        while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
        error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
        if (error) return error;

        Datacount--;

        I2C_READ_DATA(I2CID,Data); /* store the Data received */
    }

    I2C_MasterCTL(I2CID,BURST_DONE); /* -Data-NACK-P */

    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/


    //I2C_READ_DATA(I2CID,Data); /* store the Data received */
    I2C_READ_DATA(I2CID,Data); /* store the Data received */

    while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/

    return 0; /* no error */

}



/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						Special Functions			              *
 ******************************************************************************
 */
uint8_t I2C_Send_EEPROM(uint8_t I2CID, uint8_t Slave_address, uint8_t Memory_Loc, uint8_t* Data)
{
    int loop,i;
    int datacount=0;
    int w=0;
    for(loop=0;;loop++)
    {
         datacount=w+1;
         w++;
         if(*(Data+loop+1)=='\0')
         {
            break;
         }
    }
    volatile char error=0;
    I2C_SlaveADD_send(I2CID,Slave_address,WRITE);
    I2C_WRITE_DATA(I2CID,Memory_Loc);
    I2C_MasterCTL(I2CID, BURST_SEND_START);

    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

    if(datacount == 1)
    {
        volatile char error=0;
        I2C_SlaveADD_send(I2CID,Slave_address,WRITE);
        I2C_WRITE_DATA(I2CID,*(Data+0));
        I2C_MasterCTL(I2CID, BURST_DONE);

        while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
        error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
        while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/
        error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

        if (error) return error;
    }
    for(i=0;i<datacount;i++)
    {
       I2C_WRITE_DATA(I2CID,*(Data+i));
       if(i != datacount-1)
       {
           I2C_MasterCTL(I2CID,BURST_SEND_CONT);
           while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
           error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
           if (error) return error;
       }else
       {
           I2C_MasterCTL(I2CID,BURST_DONE);
           while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
           error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
           if (error) return error;
       }
    }
}


uint8_t TPR_Calculator(uint32_t Clock)
{
    char tpr=0;
    tpr=((16000000)/(20*Clock)) - 1;
    return tpr;
}

uint8_t I2C_FastSend(uint8_t I2CID, uint8_t Slave_address, uint8_t Data)
{
    volatile char error=0;
    I2C_SlaveADD_send(I2CID,Slave_address,WRITE);
    I2C_WRITE_DATA(I2CID,Data);
    I2C_MasterCTL(I2CID, SINGLE_OP);

    while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
    while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/
    error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

    if (error)return error;
}
/*void I2CSend_Array(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data)
{
    int i=0;
        for(i=0;;i++)
        {
            if(*(Data+i) != '\0')
            {
                I2C_Quick_Send(I2CID , Slave_address , *(Data+i));
            }else
            {
                break;
            }
        }
}*/


uint8_t I2C_FASTReceive(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data)
{
        volatile uint8_t error=0;
        I2C_SlaveADD_send(I2CID,Slave_address,READ);
        I2C_MasterCTL(I2CID,SINGLE_OP);

        while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
        error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
        while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/
        error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
        I2C_READ_DATA(I2CID,Data);

    if (error) return error;
    return 0;
}
