#include <stdint.h>
#include "DMA.h"
#include "GPIO.h"
#include "UART.h"
#include "M4MemMap.h"
#include "gps.h"
#include "CAN_Manager.h"


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
    GPIO_SetAlternFuntion(9,8);
    GPIO_SetAlternFuntion(10,8);
    DMA_Init();
    UART_Init();
    GPS_Init();
    CAN_Init();

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
        CAN_MainFunction_Handling ();

    }
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
