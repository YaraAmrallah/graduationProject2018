#include "I2C.h"

uint8_t I2C_InitFlag = 0;


I2C_CheckType I2C_Init(void)
{
	
		I2C_CheckType RetVal;
		const I2C_ConfigType* ConfigPtr;	
	    uint8_t LoopIndex;

  for(LoopIndex=0;LoopIndex < I2C_PERIPHERAL_NUMBER; LoopIndex++)
 	{

    ConfigPtr = &I2C_ConfigParam[LoopIndex];

   if(((ConfigPtr->I2C_FrequencyOf_SCL<=MAX_SCL_FREQUENCY_SM && !ConfigPtr->I2C_SpeedMode)    ||
           (ConfigPtr->I2C_FrequencyOf_SCL<=MAX_SCL_FREQUENCY_FM && ConfigPtr->I2C_SpeedMode)) )
		 {			  

           SYSCTL_RCGCGPIO_R |= 0x02; /* enable clock to GPIOA */
           /* PORTB 7, 6 for I2C1 */
           GPIO_PORTB_AFSEL_R |=1<<3;/* PORTB 7, 6 for I2C1 */
            GPIO_PORTB_AFSEL_R |=1<<2;
		   GPIO_PORTB_PCTL_R |= (3u) << 4*(3);
           GPIO_PORTB_PCTL_R |= (3u) << 4*(2);
           GPIO_PORTB_DEN_R |= (1<<(3)) + (1<<2);
           GPIO_PORTB_ODR_R |= 1<<(3) ;/* PORTB 7 as open drain */
                //GPIO_SetAlternFuntion(ConfigPtr->I2C_GPIO_Structure_ID ,0xC0);

			I2CRCGC |= (((uint32_t)1) <<(ConfigPtr->I2C_Peripheral_ID));	
			//I2C1_MCR_R|= (ConfigPtr-> I2C_TYPE);
			I2CMCR(ConfigPtr->I2C_Peripheral_ID)  |= (ConfigPtr-> I2C_TYPE);
			I2CMTPR(ConfigPtr->I2C_Peripheral_ID) |= (ConfigPtr->I2C_PeripheralFrequency);
			I2CMTPR(ConfigPtr->I2C_Peripheral_ID) |= ((ConfigPtr->I2C_SpeedMode)<<7);	
			
			RetVal = I2C_OK;	
			I2C_InitFlag=1;
	}
	  else
                    {
                            RetVal = I2C_NOK;
                            I2C_InitFlag = 0;
                    }

 } 
	
		return RetVal;
		
	}


uint8_t I2C_TX(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data,uint8_t datacount)
{
  I2C_CheckType RETVAL;
  I2C_CheckType DriverReturnStatus = I2C_NOK;
    if(datacount == 1)//single send
        {
			static uint8_t r=0;
			switch(r)
			{
					case 0:
					{
				 //   volatile char error=0;
					I2C_SlaveADD_send(I2CID,Slave_address,I2C_Transmit);
					I2C_WRITE_DATA(I2CID,*(Data));
					I2C_MasterCTL(I2CID, SINGLE_OP);
					
					r=1;
					//while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
					//error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/
				   // while(I2CMCS(I2CID) & 0x40); /*Check on the bus busy flag*/
					//error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

				  //  if (error) return error;
			        }
				  case 1:
				  {
				   DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_SINGLESEND);
				   if(DriverReturnStatus==I2C_SINGLEOK)
					 {
						 RETVAL==I2C_OK;
						 r=2;
				     }
				     else
					 {
						 RETVAL==I2C_NOK;
					     r=1;
					 }
				  }break;
			}
		
		
		}
		else
        {
           // volatile char error=0;
		   static uint8_t e=0;
           static int i,k=0;
           switch(e)
		   {
			   
			   case 0:
			   {
					I2C_SlaveADD_send(I2CID,Slave_address,I2C_Transmit);
					I2C_WRITE_DATA(I2CID,*(Data));
					I2C_MasterCTL(I2CID,BURST_SEND_START);
					e=1;
			   }
			   case 1:
			   {
				   if(i<datacount)
				   {
					   k=i+1;

					   I2C_WRITE_DATA(I2CID,*(Data+k));
					   if(k != datacount-1)
                        {
					    	I2C_MasterCTL(I2CID,BURST_SEND_CONT);	
							DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BURSTSEND_STAT);
						   if(DriverReturnStatus==I2C_BURSTOK)
							 {
						
								 e=1; i++;
							 }
							 else
							 {
								
								/**/
								
							 }
						}
						else
						{
							e=2;
						}
					   
				   }
				   else 
				   { RETVAL==I2C_NOK;}
						  }break;
			   
			    case 2:
			   {
						   I2C_MasterCTL(I2CID,BURST_DONE);
						   DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BURSTSEND_STAT);
						   if(DriverReturnStatus==I2C_BURSTOK)
						   {
							   RETVAL==I2C_OK;
							   i=0;
							   e=0;
							   k=0;			     	   }
					      else
						  {
							  RETVAL==I2C_NOK;
							  e=3;
							  }
				   
			   }break;
			   
		   }
           
          //  while(I2CMCS(I2CID) & 1);    /*Checks if the I2C Master still busy*/
            //error = I2CMCS(I2CID) & 0xE; /*returns 1 if any of these bits are equal 1 (Error || DataAcknowledge || ADress Acknowledge)*/

        }
		return RETVAL;
}

 void  I2C_GenerateStart(uint8_t Peripheral_ID)
   {
	  const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
	  
	  I2CMCS(ConfigPtr->I2C_Peripheral_ID) = I2C_START_GEN;
	 
   }


 /*
 I2C_CheckType I2C_StartStatus(uint8_t Peripheral_ID){
	 
	  const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];

      I2C_CheckType RetVal;
    
    if( ((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	) )// I2C_BUS_BUSY ,The bit changes based on the START and STOP conditions
       {
            RetVal = I2C_OK;
       }
    else
       {
            RetVal = I2C_NOK;
       }
    return RetVal;
	 
 }*/
 
 
 void I2C_SendSlaveAddress (uint8_t SlaveAddress,uint8_t WriteOrRead, uint8_t Peripheral_ID)
	{
        const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
        I2CMSA(ConfigPtr->I2C_Peripheral_ID) = ((SlaveAddress<<1)|WriteOrRead);
	}

	void I2C_Clear_ADDR(uint8_t Peripheral_ID)
{
        const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
        I2CMSA(ConfigPtr->I2C_Peripheral_ID) = 0;
}
/*
I2C_CheckType I2C_SendSlaveAddressStatus(uint8_t Peripheral_ID)
	{
		I2C_CheckType RetVal;
	
		const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
    
	while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
   
   if (((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ADDR_ACK)) == (((1<<I2C_ADDR_ACK))&& (I2CMCS(ConfigPtr->I2C_Peripheral_ID)&&(1<<I2C_ERROR)==0))))//ADDRess Ack
		{
			RetVal = I2C_OK;
		}
    else
		{
			RetVal = I2C_NOK;
		}
    return RetVal;
	}*/


void I2C_SendData(uint8_t* DataPTR,uint8_t Peripheral_ID)
	{
	
	const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
	I2CMDR(ConfigPtr->I2C_Peripheral_ID)=(*DataPTR);

	 }

/*
I2C_CheckType I2CSendDataStatus(uint8_t Peripheral_ID)
	{
        I2C_CheckType RetVal;
        const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
       
	   while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
       
	   if(((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_DATA_ACK)))&& ((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ERROR)))==0)//DATA ACK
			{
				RetVal = I2C_OK;
			}
        else
			{
				RetVal = I2C_NOK;
			}
	
        return RetVal;
	}*/

 
 void I2C_GetData(uint8_t *Data, uint8_t Peripheral_ID)
	{
      const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
      *Data = I2CMDR(ConfigPtr->I2C_Peripheral_ID);

	}	
/*
I2C_CheckType I2C_GetDataStatus(uint8_t Peripheral_ID)
	{
        
    I2C_CheckType RetVal;
    
	      const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
	 
    while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
 
    if(((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_DATA_ACK)))&& ((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ERROR)))==0)
		  {
			RetVal = I2C_OK;
		  }
    else
		  {
			RetVal = I2C_NOK;
		  }
    return RetVal;
	}

*/


	I2C_CheckType I2C_CHECKSTATUS(uint8_t I2CID,I2C_Checkstatus Status)
	{


          I2C_CheckType RetVal;
	      //const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
	      I2C_Checkstatus x = Status;
 switch(x)
 {

   case I2C_SINGLESEND:
   {
         if(I2CMCS(I2CID) & 1) /*Checks if the I2C Master still busy*/
		 {
			RetVal = I2C_SINGLENOK; 
		 }
		 else
		 {
			 if(I2CMCS(I2CID) & 0xE) /* if any of these bits are equal 0 (Error || DataAcknowledge || ADress Acknowledge)*/
			 {
			     RetVal = I2C_SINGLENOK;

				  } 
				  else
				  {
				      if(I2CMCS(I2CID) & 0x40) /*Check on the bus busy flag*/
				            {
				          RetVal = I2C_SINGLENOK;

				  }
				      else
				         {
				          if(I2CMCS(I2CID) & 0xE)
				            {
				               RetVal = I2C_SINGLENOK;
				             }
				                    else{RetVal = I2C_SINGLEOK; }
				             }
		     }
				 

		 }
   }break;

 case I2C_BURSTSEND_STAT:
   {
     if(I2CMCS(I2CID) & 1)
	 {
         RetVal =  I2C_BURSTNOK;
	 }
	 else
	 {
		 if(I2CMCS(I2CID) & 0xE)
		   {
			RetVal = I2C_BURSTNOK;
			}
		else
		{
			RetVal = I2C_BURSTOK;
		} 
	 }
	    

    }break;
 case MASTER_BUSY:
   {
     if(I2CMCS(I2CID) & 1)
	 {
         RetVal =  I2C_SREAD_NOK;
	 }
	 else
	 {
		 if((I2CMCS(I2CID) & 0xE))
		   {
			RetVal = I2C_SREAD_NOK;
			}
		else
		{
			RetVal = I2C_SREAD_OK;
		} 
	 }
	  }break;

   case I2C_BUSBUSY:

     if(I2CMCS(I2CID) & 0x40)
	 {
         RetVal =  I2C_BUSY;
	 }
	 else
	 {
		RetVal = I2C_NOBUSY;  
	 }
	  break;

case I2C_MBUSY:

     if(I2CMCS(I2CID) & 1)
	 {
         RetVal =  I2C_MASTERBUSY;
	 }
	 else
	 {
		RetVal = I2C_MASTERNOBUSY;  
	 }
	  break;
    
 }
  return RetVal;
}
/* void I2C_GenerateStop(uint8_t Peripheral_ID)
	{
       const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
        I2CMCS(ConfigPtr->I2C_Peripheral_ID) = I2C_STOP_GEN;
	}	


void I2C_TurnOffAcknowledge(uint8_t Peripheral_ID)
	{
        const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
       I2CMCS(ConfigPtr->I2C_Peripheral_ID) &= ~(1<<I2C_ACK);
	}

 void I2C_TurnOnAcknowledge(uint8_t Peripheral_ID)
	{
        const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
         I2CMCS(ConfigPtr->I2C_Peripheral_ID) |= (1<<I2C_ACK);
	}

 */
