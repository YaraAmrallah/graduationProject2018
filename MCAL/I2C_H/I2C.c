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

           SYSCTL_RCGCGPIO_R |= 0x01; /* enable clock to GPIOA */
           /* PORTA 7, 6 for I2C1 */
           GPIO_PORTA_AFSEL_R |= 0xC0; /* PORTA 7, 6 for I2C1 */
           GPIO_PORTA_PCTL_R &= ~0xFF000000; /* PORTA 7, 6 for I2C1 */
           GPIO_PORTA_PCTL_R |= 0x33000000;
           GPIO_PORTA_DEN_R |= 0xC0; /* PORTA 7, 6 as digital pins */
           GPIO_PORTA_ODR_R |= 0x80; /* PORTA 7 as open drain */
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
        I2CMSA(ConfigPtr->I2C_Peripheral_ID) = (SlaveAddress|WriteOrRead);
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


	I2C_CheckType I2C_CHECKSTATUS(uint8_t Peripheral_ID,I2C_Checkstatus Status)
	{


          I2C_CheckType RetVal;
	      const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[Peripheral_ID];
	      I2C_Checkstatus x = Status;
 switch(x)
 {

   case I2C_Start:
   {

	     if( ((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	) )// I2C_BUS_BUSY ,The bit changes based on the START and STOP conditions
	       {
	            RetVal =I2C_StartOK;
	       }
	     else
	       {
	            RetVal = I2C_StartNOK;
	       }
	    

   }break;

 case I2C_SendSlaveAdd:
   {
			   while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
			   
			   if (((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ADDR_ACK)) == (((1<<I2C_ADDR_ACK))&& (I2CMCS(ConfigPtr->I2C_Peripheral_ID)&&(1<<I2C_ERROR)==0))))//ADDRess Ack
					{
						RetVal = I2C_SendSlaveAddressOK;
					}
			    else
					{
						RetVal = I2C_SendSlaveAddressNOK;
		            }
	    

    }break;

     case I2C_SendDATAA:
   {
	       while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
	       
		   if(((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_DATA_ACK)))&& ((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ERROR)))==0)//DATA ACK
				{
					RetVal = I2C_SendDATAOK;
				}
	        else
				{
					RetVal = I2C_SendDATANOK;
			}
	    

    }break;

     case I2C_GetDataa:
   {
	      while(((I2CMCS(ConfigPtr->I2C_Peripheral_ID)) & (1<<I2C_BUS_BUSY)) == (1<<I2C_BUS_BUSY	));//WAIT UNTIL BUS IS NOT BUSY
 
    if( ((I2CMCS(ConfigPtr->I2C_Peripheral_ID) & (1<<I2C_ERROR)))==0)
		  {
			RetVal = I2C_GetDataOK;
		  }
    else
		  {
			RetVal = I2C_GetDataNOK;
		  }
	    

    }break;


    
 }
  return RetVal;
}
void I2C_GenerateStop(uint8_t Peripheral_ID)
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

