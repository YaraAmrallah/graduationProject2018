#include <stdint.h>
#include "GPIO.h"
#include "UART.h"

/**
 * main.c
 */


void delay100ms(uint64_t time);
void BlinkLed(uint8_t period,uint8_t groupId);
int main(void)
{   UART_RetType RetVar3;
    GPIO_CheckType RetVar4,RetVar5;
    uint8_t* name = "Khaled";
    uint8_t* Data = "BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM";
    uint8_t num = 0;
    GPIO_Init();
    RetVar4 = GPIO_SetAlternFuntion(4,1);
    RetVar5 = GPIO_SetAlternFuntion(5,1);
    RetVar3 = UART_Init();
    if(RetVar5 == GPIO_OK &&RetVar4==GPIO_OK &&RetVar3==UART_OK){
      // BlinkLed(1,0);
    }

    UART_StartReceiving(UART1,Data,20000);
    UART_StartTransmission(Data,20000,UART0);
    while(1){

    UART_ManageOngoingOperations(UART0);
   // UART_GetNumofTxBytes(UART0,&num);
    UART_GetNumofRxBytes(UART1, &num);
    }

	return 0;
}


void delay100ms(uint64_t time){
    unsigned long i;
    while(time>0){
        i=133333;
        while(i>0){
            i--;
        }
        time--;
    }
}



void BlinkLed(uint8_t period,uint8_t groupId){
    if(groupId<GPIO_GROUPS_NUMBER){
    GPIO_Write(groupId,0xff);
    delay100ms(period);
    GPIO_Write(groupId,0x00);
    delay100ms(period);

    }
}

void UART0_ISR(){
    BlinkLed(1,6);
}

void trans(){

}

void receive(){

}
