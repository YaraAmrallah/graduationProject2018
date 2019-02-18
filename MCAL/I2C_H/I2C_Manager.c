#include "I2C_Manager.h"
//Flags


//Function write that will be used in the Main
void I2C_RequestWrite(uint8_t SlaveAddress, uint8_t* DataPointer, uint8_t NumOfBytes)
{
    I2C_Write = 1;
    I2C_SlaveAddress = SlaveAddress;
    I2C_DataPointer = DataPointer;
    I2C_ByteCount = NumOfBytes;
}
void I2C_RequestRead(uint8_t SlaveAddress, uint8_t* DataPointer, uint8_t NumOfBytes)
{
    I2C_Read = 1;
    I2C_SlaveAddress = SlaveAddress;
    I2C_DataPointer = DataPointer;
    I2C_ByteCount = NumOfBytes;
}

void I2C_Manager(void)
{
    I2C_CheckType DriverReturnStatus = I2C_NOK;
   //setting the intial states to un intialize

    static I2C_States ManagerState = I2C_UN_INIT;
    static I2C_States OldState     = I2C_UN_INIT;
    //taking the group struct
    const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[0];
    
    switch(ManagerState)
    {
        case I2C_UN_INIT:
        {
            if(I2C_InitFlag == 1)
            {
                //once I2C initalization finishes
                ManagerState = I2C_IDLE;
            }
            else
            {
                //IF Init is intialised zero , it will enter this state
                ManagerState = I2C_UN_INIT;
            }
        }
        break;

        case I2C_IDLE:
        {
            if(I2C_Write == 1)
            {
                ManagerState = I2C_GENERATE_START;
            }
            else if(I2C_Read == 1)
            {
                ManagerState = I2C_GENERATE_START;
            }
            else
            {
                ManagerState = I2C_IDLE;
            }
        }
        break;

        case I2C_GENERATE_START:
        {
            I2C_GenerateStart(0);
            ManagerState = I2C_START_STATUS;
        }
        break;

        case I2C_START_STATUS:
        {
           // DriverReturnStatus = I2C_StartStatus(0);
            DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_Start);
            if(DriverReturnStatus == I2C_StartOK)
            {
                ManagerState = I2C_SEND_SLAVE_ADDRESS;
            }
            else
            {

                ManagerState = I2C_START_STATUS;
            }
        }
        break;

        case I2C_SEND_SLAVE_ADDRESS:
        {
            if(I2C_Write == 1)
            {
                I2C_SendSlaveAddress(I2C_SlaveAddress,I2C_WRITE,0);
            }
            else if(I2C_Read == 1)
            {
                I2C_SendSlaveAddress(I2C_SlaveAddress,I2C_READ,0);
            }
            else{/*MISRA*/};
            
            ManagerState = I2C_SEND_SLAVE_ADDRESS_STATUS;
        }
        break;

        case I2C_SEND_SLAVE_ADDRESS_STATUS:
        {
         //   DriverReturnStatus = I2C_SendSlaveAddressStatus(0);
              DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_SendSlaveAdd);
            if(DriverReturnStatus == I2C_SendSlaveAddressOK)
            {
                if(I2C_Write == 1)
                {
                    ManagerState =I2C_SEND_DATA;
                }
                else if(I2C_Read == 1)
                {
                    OldState = I2C_SEND_SLAVE_ADDRESS_STATUS;
                    ManagerState = I2C_GET_DATA_MANAGER;
                }
                else{/*MISRA*/};
            }
            else
            {
                ManagerState = I2C_SEND_SLAVE_ADDRESS_STATUS;
            }
        }
        break;
     /*I2C clear bit state here  is used in multiple reads*/
        case I2C_CLEAR_ADDR_BIT:
        {
            I2C_Clear_ADDR(0);

            if(I2C_Write == 1)
            {
                ManagerState = I2C_SEND_DATA;
            }
            else if(I2C_Read == 1)
            {
                OldState = I2C_CLEAR_ADDR_BIT;
                ManagerState = I2C_GET_DATA_MANAGER;
            }
            else{/*MISRA*/};

        }
		break;

        case I2C_SEND_DATA:
        {
             I2C_SendData(I2C_DataPointer, 0);
            ManagerState = I2C_SEND_DATA_STATUS;
        }
        break;

        case I2C_SEND_DATA_STATUS:
        {
           // DriverReturnStatus = I2C_PlaceDataStatus(0);
              DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_SendDATAA);

            if(DriverReturnStatus == I2C_SendDATAOK)
            {
                ManagerState = I2C_SEND_DATA_MANAGER;
            }
            else
            {
                ManagerState = I2C_SEND_DATA_STATUS;
            }
        }
        break;

        case I2C_SEND_DATA_MANAGER:
        {
            if(I2C_ByteCount == 1)
            {
                ManagerState = I2C_IDLE;
                I2C_Write = 0;
                I2C_DataPointer = 0;
                I2C_ByteCount = 0;
                (*(ConfigPtr->I2C_TransmissionDoneCallBack))();
            }
            else if(I2C_ByteCount > 1)
            {
                I2C_ByteCount--;
                I2C_DataPointer++;
                ManagerState = I2C_SEND_DATA;
            }
            else{/*MISRA*/};
        }
        break;

        case I2C_GET_DATA_MANAGER:
        {
            switch(OldState)
            {
                case I2C_SEND_SLAVE_ADDRESS_STATUS:
                {
					if(I2C_ByteCount == 1)
                    {
                        I2C_TurnOffAcknowledge(0);
                        ManagerState = I2C_CLEAR_ADDR_BIT;
                    }
                    else if(I2C_ByteCount > 1)
                    {
                       ManagerState = I2C_CLEAR_ADDR_BIT; 
                    }
                    else{/*MISRA*/};
                }
                break;

                case I2C_CLEAR_ADDR_BIT:
                {
					if(I2C_ByteCount == 1)
                    {
                        ManagerState = I2C_GET_SINGLE_BYTE_STATUS;
                    }
                    else if(I2C_ByteCount > 1)
                    {
                        ManagerState = I2C_GET_MULTI_BYTE_STATUS;
                    }
                    else{/*MISRA*/};
                }
                break;

                case I2C_GET_SINGLE_BYTE:
                {
                    ManagerState = I2C_IDLE;
                    OldState = I2C_UN_INIT;
                    I2C_Read = 0;
                    I2C_ByteCount = 0;
                    I2C_DataPointer = 0;
					I2C_TurnOnAcknowledge(0);
                    (*(ConfigPtr->I2C_ReceptionDoneCallBack))();
                }
                break;

                case I2C_GET_MULTI_BYTES:
                {
                    if(I2C_ByteCount == 1)
                    {
                        I2C_TurnOffAcknowledge(0);
                        ManagerState = I2C_GET_SINGLE_BYTE_STATUS;
                    }
                    else if(I2C_ByteCount > 1)
                    {
                        I2C_ByteCount--;
                        I2C_DataPointer++;
						ManagerState = I2C_GET_MULTI_BYTE_STATUS; 
                    }
                }
                break;
				
				default: break;
            }
        }
        break;

        case I2C_GET_SINGLE_BYTE_STATUS:
        {
          //  DriverReturnStatus = I2C_GetDataStatus(0);
               DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_GetDataa);   
            if(DriverReturnStatus == I2C_GetDataOK)
            {
                ManagerState = I2C_GET_SINGLE_BYTE;
            }
            else
            {
                ManagerState = I2C_GET_SINGLE_BYTE_STATUS;
            }
        }
        break;

        case I2C_GET_SINGLE_BYTE:
        {
            I2C_GetData(I2C_DataPointer, 0);
            ManagerState = I2C_GET_DATA_MANAGER;
            OldState = I2C_GET_SINGLE_BYTE;
        }
        break;

        case I2C_GET_MULTI_BYTE_STATUS:
        {
            //DriverReturnStatus = I2C_GetDataStatus(0);
            DriverReturnStatus =I2C_CHECKSTATUS(0,I2C_GetDataa); 
            if(DriverReturnStatus == I2C_GetDataOK)
            {
                ManagerState = I2C_GET_MULTI_BYTES;
            }
            else
            {
                ManagerState = I2C_GET_MULTI_BYTE_STATUS;
            }
        }
        break;

        case I2C_GET_MULTI_BYTES:
        {
            I2C_GetData(I2C_DataPointer, 0);
            ManagerState = I2C_GET_DATA_MANAGER;
            OldState = I2C_GET_MULTI_BYTES;
        }
        break;
    }
}




