#include <stdint.h>
#include "DMA.h"
#include "GPIO.h"
#include "UART.h"
#include "M4MemMap.h"
#include "gps.h"
#include "CAN_Manager.h"




void BlinkLed(uint8_t period,uint8_t groupId);
void nullFunction(void);
void START_Notify(void);

uint8_t StartFlag = 0;
uint8_t EndFlag = 0;

void delay1ms(uint64_t time);

uint8_t dummyBuffer[8]={0};

int main(void)
{
    EN_INT(33);
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

void START_Notify(void)
{
    BlinkLed(20,0);
    StartFlag = 1;
    EndFlag = 0;
    AsyncRxData_Request(1, dummyBuffer);
}

void nullFunction(void)
{
     '\0';
}



void END_Notify(void)
{
    BlinkLed(255,0);
    EndFlag = 1;
    StartFlag = 0;
    AsyncRxData_Request(2, dummyBuffer);
}
