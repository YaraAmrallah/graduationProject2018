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

    static uint8_t o = 1;
    //taking the group struct
    const I2C_ConfigType* ConfigPtr = &I2C_ConfigParam[0];
    static uint8_t i,k=0;
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
                 I2C_SlaveADD_send(0,I2C_SlaveAddress,I2C_WRITE);
                 ManagerState = WRITE_DATA;
            }
            else if(I2C_Read == 1 && (I2C_ByteCount>0))
            {
                 I2C_SlaveADD_send(0,I2C_SlaveAddress,I2C_READ);
                  ManagerState = READ_DATA;
            }
            else
            {
                ManagerState = I2C_IDLE;
            }
        }
        break;

        case WRITE_DATA:
        {

            if(I2C_ByteCount == 1)
            {
                I2C_WRITE_DATA(0,*(I2C_DataPointer));
                ManagerState = SINGLE_SEND_START;
            }
            else if(I2C_ByteCount > 1)
            {
                ManagerState = BURST_SEND;

            }
            else{/*MISRA*/};
        }
        break;

        case SINGLE_SEND_START:
        {
           I2C_MasterCTL(0, SINGLE_OP);
           ManagerState = STATUS_SEND_START;
        }
        break;


        case STATUS_SEND_START:
        {
              DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_SINGLESEND);
              if(DriverReturnStatus == I2C_SINGLEOK)
            {
                ManagerState = I2C_IDLE;
                I2C_Write = 0;
                I2C_DataPointer = 0;
                I2C_ByteCount = 0;
                (*(ConfigPtr->I2C_TransmissionDoneCallBack))();
            }
            else
            {
                ManagerState = STATUS_SEND_START;
            }

        }
        break;

        case BURST_SEND:
        {
            I2C_WRITE_DATA(0,*(I2C_DataPointer));
            I2C_MasterCTL(0,BURST_SEND_START); 
            ManagerState=BURST_SENDSTAT;

        } break;

        case BURST_SENDSTAT:
        {
                  DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BURSTSEND_STAT);
                        static uint8_t z=1;

                        if(DriverReturnStatus == I2C_BURSTOK)
                        {

                            if(i<I2C_ByteCount-1)
                            {
                                k=i+1;

                                I2C_WRITE_DATA(0,*(I2C_DataPointer+k));

                            //
                                switch (z)
                                {
                                    case 1:
                                    {
                                      if(k == (I2C_ByteCount-1))
                                        {
                                           z=3;
                                        }
                                        else
                                        {
                                            I2C_MasterCTL(0,BURST_SEND_CONT);
                                            z=2;
                                        }
                                    }
                                     break;

                                    case 2:
                                    {
                                     DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BURSTSEND_STAT);
                                    if(DriverReturnStatus == I2C_BURSTOK)
                                    {
                                        z = 1; i++;
                                     }
                                    else
                                    {
                                        z = 2;
                                    }
                                    } break;

                                    case 3:
                                    {
                                        I2C_MasterCTL(0,BURST_DONE);
                                        z=4;

                                    }
                                     break;

                                    case 4:
                                    {
                                        DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BURSTSEND_STAT);
                                        if(DriverReturnStatus == I2C_BURSTOK)
                                       {
                                        ManagerState = I2C_IDLE;
                                        I2C_Write = 0;
                                        I2C_DataPointer = 0;
                                        I2C_ByteCount = 0;
                                        k=0;i=0;z=1;
                                        (*(ConfigPtr->I2C_TransmissionDoneCallBack))();
                                        }
                                        else
                                        {
                                            z = 4;
                                        }

                                    }break;
                                }
                            }

                        }
                       else
                       {ManagerState = BURST_SENDSTAT;}
                 }break;


     /*I2C clear bit state here  is used in multiple reads*/
        case READ_DATA:
        {
          if(I2C_ByteCount == 1)
          {
                 I2C_MasterCTL(0,SINGLE_OP);
                DriverReturnStatus = I2C_CHECKSTATUS(0,MASTER_BUSY);
                if(DriverReturnStatus == I2C_SREAD_OK)
                {
                    ManagerState = SINGLE_READ;
                }
                else
                {
                    ManagerState = READ_DATA;
                }
          }

          else
          {
               I2C_MasterCTL(0,BURST_RECEIVE_START);
                DriverReturnStatus = I2C_CHECKSTATUS(0,MASTER_BUSY);
              if(DriverReturnStatus == I2C_SREAD_OK)
                {
                    ManagerState = BURST_READ;
                }
                else
                {
                    ManagerState = READ_DATA;
                }


          }
        }
        break;

        case SINGLE_READ:
        {

             I2C_READ_DATA(0,I2C_DataPointer);

             ManagerState = I2C_READ_DATA_STATUS
			 
			 ;
        }
        break;





        case I2C_READ_DATA_STATUS:
        {
          
              DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BUSBUSY);

            if(DriverReturnStatus == I2C_BUSY)
            {
                ManagerState = I2C_READ_DATA_STATUS;
            }
            else
            {
                (*(ConfigPtr->I2C_ReceptionDoneCallBack))();
                ManagerState = I2C_IDLE;
            }
        }
        break;



        case BURST_READ:

        {
              I2C_READ_DATA(0,I2C_DataPointer);
              ManagerState = I2C_BURST_READSTAT;

         } break;



        case I2C_BURST_READSTAT:
        {
            switch(o)
            {
                case 1:
                {
                    if(I2C_ByteCount > 1)
                    {
                         I2C_MasterCTL(0,BURST_RECEIVE_CONT);
                         DriverReturnStatus = I2C_CHECKSTATUS(0,MASTER_BUSY);

                        if(DriverReturnStatus == I2C_SREAD_OK)
                        {
                            ManagerState = I2C_BURST_READSTAT;
                            I2C_ByteCount--;
                            I2C_DataPointer++;
                            I2C_READ_DATA(0,I2C_DataPointer);
                            o=1;
                        }
               
                    }
                    else if(I2C_ByteCount == 1)
                    {
                        I2C_MasterCTL(0,BURST_DONE); /* -Data-NACK-P */
                        o=2;
                    }

                }break;

                case 2:
                {
                   DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_MBUSY);

                    if(DriverReturnStatus == I2C_MASTERBUSY)
                    {
                        o=2;

                        }
                    else
                    {
                        I2C_READ_DATA(0,I2C_DataPointer);
                        o=3;
                    }
               } break;

               case 3:
               {
                       DriverReturnStatus = I2C_CHECKSTATUS(0,I2C_BUSBUSY);
                        if(DriverReturnStatus == I2C_BUSBUSY)
                        {
                            o=3;
                        }
                        else{
                        ManagerState = I2C_IDLE;
                        I2C_Read = 0;
                        I2C_DataPointer = 0;
                        I2C_ByteCount = 0;
                        (*(ConfigPtr->I2C_ReceptionDoneCallBack))();
                        }
               }break;
                default: break;
            }
        }break;
    }
 }

       
    


