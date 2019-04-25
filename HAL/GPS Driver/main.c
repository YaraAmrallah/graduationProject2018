#include <stdint.h>
#include "UART.h"
#include "DMA.h"
#include "GPIO.h"
#include "UART.h"
#include "M4MemMap.h"
#include "gps.h"


/*ToDo
 * GPS extern and # define - > to be deleted*/


uint8_t Data[1024];
uint8_t DMA[1024];
uint32_t i=0, k =0;

extern uint8_t GPSRawData[TOTAL_ARR_LENGTH];
extern uint8_t GPS_DataSentence[GPRMC_DATA_LENGTH];
extern uint8_t UARTDriverStates[USED_UART_MODULES];


void BlinkLed(uint8_t period,uint8_t groupId);

#define DMAERRCLR          (*(volatile  uint32_t* const)(0x400FF000 + 0x04C))

void delay1ms(uint64_t time);

int main(void)
{

    EN_INT(6);
    EN_INT(46);
    GPIO_Init();
    GPIO_SetAlternFuntion(4,1);
    GPIO_SetAlternFuntion(5,1);
    GPIO_SetAlternFuntion(7,1);
    GPIO_SetAlternFuntion(8,1);
    DMA_Init();
    UART_Init();
    GPS_Init();

    // waiting for the GPS to warm up.
    delay1ms(100);

    //DMA_StartChannel(0,0x4000D000,(uint32_t)(DMA + 1023), 1024);

    //UART_StartReceiving(UART1,Data,1024);
    //UART_StartReceiving(UART0,Data,1024);
    //UART_StartTransmission(UART0,Data,1024);
    //BlinkLed(20,0);



    while(1)
    {


        GPS_Main_Function_Handling();
        /*
            for(i=0;i<3;i++)
            {
                GPS_ReqData(UART1);
            //delay1ms(30);
            }*/

           /* if(k == GPRMCDataLENGTH || k == 0)
            {
            UARTDriverStates[UART0] = 1;
            UART_StartTransmission(UART0,GPS_DataSentence,GPRMCDataLENGTH);
            }
            DIS_INT(6);
            DIS_INT(46);
            for(k=0;k<GPRMCDataLENGTH;k++){

                UART_ManageOngoingOperations(UART0);

                //UART_GetNumofTxBytes(UART0, &i);

            }
            EN_INT(46);
            EN_INT(6);*/

       /* if((DMAERRCLR & 1) == 1)
        {
            BlinkLed(20,0);
            //clear error
            interruptErrorHandling();
        }*/

      //  UART_ManageOngoingOperations(UART1);
    }
	return 0;
}

//UART CALLBACKS

void trans0(){

}

void receive0(){

}

void trans1(){

}

void receive1(){

}

void delay1ms(uint64_t time)
{
    unsigned long i;
    while(time>0){
        i=16000;
        while(i>0){
            i--;
        }
        time--;
    }
}

void BlinkLed(uint8_t period,uint8_t groupId)
{
    if(groupId<GPIO_GROUPS_NUMBER){
    GPIO_Write(groupId,0xff);
    delay1ms(period);
    GPIO_Write(groupId,0x00);
    delay1ms(period);

    }
}

void testISR()
{
    BlinkLed(200,0);

}
