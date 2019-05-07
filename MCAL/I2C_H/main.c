//#include "delay.h"
//#include "MPU.h"
#include "GPIO.h"
#include "I2C_Manager.h"

static uint8_t TransmissionDone = 0;
static uint8_t ReceptionDone = 0;

uint8_t ReceivedArr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t Data[1] = { 0x75 };
uint8_t DATAA[5] = { 'H', 'E', 'L', 'L', 'O' };
uint8_t SlaveAddress = 1;

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
    uint8_t x = 0;

    GPIO_Init();
    GPIO_SetAlternFuntion(5, 3);
    GPIO_SetAlternFuntion(6, 3);
    // MPU_Init();
    I2C_Init();

    while (1)
    {

        I2C_Manager();
        I2C_RequestWrite(SlaveAddress, Data, 1);
        //I2C_RequestRead(SlaveAddress,  ReceivedArr, 1);
        /* switch(x)
         {
         case 0:
         {
         //I2C_RequestWrite(SlaveAddress, Data,7);

         x = 1;
         }
         break;

         case 1:
         {
         if(ReceptionDone == 1)
         {
         BlinkLed(15,4)
         ReceptionDone = 0;
         x = 4;
         }
         else
         {
         x = 1;
         }
         /* if(TransmissionDone == 1)
         {
         BlinkLed(8,0);

         TransmissionDone = 0;
         x = 2;
         }
         else
         {
         x = 1;
         }*/
        /* }
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

         }*/
    }
}

