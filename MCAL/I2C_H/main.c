
//#include "delay.h"
#include "I2C_Manager.h"
//#include "I2C2.h"
static uint8_t TransmissionDone = 0;
static uint8_t ReceptionDone = 0;

void I2C_TxDone(void)
{
    TransmissionDone = 1;
}
void I2C_RxDone(void)
{
    ReceptionDone = 1;
}

int main(void)
{

    uint8_t ReceivedArr[10]={0,0,0,0,0,0,0,0,0,0};
    uint8_t Data[7]={120,200,50,22,30,100,30};
    uint8_t DATAA[5]={'H','E','L','L','O'};
    uint8_t SlaveAddress =1;

    uint8_t x = 0;

    //GPIO_Init();

   I2C_Init();
  // I2C_Init2();
    while(1)
    {
     //I2C_Send(0,SlaveAddress,Data);

       //I2C_Receive(0, SlaveAddress, 18, ReceivedArr);
        //I2C_FASTReceive(0,SlaveAddress, ReceivedArr);
     //   I2C_SendData(Data, 0);

         I2C_Manager();

              switch(x)
              {

               case 0:
                          {
                              //I2C_RequestWrite(SlaveAddress, Data,7);
                              I2C_RequestRead(SlaveAddress,  ReceivedArr, 1);
                              x = 1;
                          }
                          break;

               case 1:
                          {
                              if(ReceptionDone == 1)
                                     {
                                  ReceptionDone = 0;
                                          x = 4;
                                    }
                                 else
                                       {
                                        x = 1;
                                       }
                             /* if(TransmissionDone == 1)
                                   {
                                   TransmissionDone = 0;
                                   x = 2;
                                   }
                               else
                                {
                                   x = 1;
                                }*/
                          }
                          break;

        case 2:
                    {
                        I2C_RequestWrite(SlaveAddress, DATAA,5);
                        x = 3;
                    }
        case 3:
                 {
                  if(TransmissionDone == 1)
                   {
                      TransmissionDone = 0;
                        x = 4;
                      }
                          else
                          {
                       x = 3;
                           }
                      }
                break;
                                  /*
           case 0:
            {
                I2C_RequestWrite(SlaveAddress, Data, 1);
                x = 1;
            }
            break;

            case 1:
            {
                if(TransmissionDone == 1)
                     {
                     TransmissionDone = 0;
                     x = 2;
                     }
                 else
                  {
                     x = 1;
                  }
            }
            break;

           case 2:
            {
                I2C_RequestRead(SlaveAddress, ReceivedArr, 8);

                x = 3;
            }
            break;

            case 3:
            {

              if(ReceptionDone == 1)
             {
                ReceptionDone = 0;
                x = 4;
             }
            else
             {
                x = 3;
             }
            }
            break;


            case 4:
            {
                //GPIO_Write(2,1);
                 x = 5;
            }
            break;

            case 5:
            {
                if(  (ReceivedArr[0] == 0x66) && (ReceivedArr[1] == 0x23) &&  (ReceivedArr[2] == 0x45) &&
                     (ReceivedArr[3] == 0x87) && (ReceivedArr[4] == 0x66) &&  (ReceivedArr[5] == 0x66) &&
                     (ReceivedArr[6] == 0x66))

                 {
                  //   GPIO_Write(4,1);
                     x = 6;
                 }
               }

            break;

            case 6:
            {
            //MISRA
            }
            }
               */
        }
     }
}



