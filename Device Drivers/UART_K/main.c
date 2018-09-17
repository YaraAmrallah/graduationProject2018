#include <stdint.h>
#include "GPIO.h"
#include "UART.h"

/**
 * main.c
 */
void delay100ms(uint64_t time);
void BlinkLed(uint8_t period,uint8_t groupId);
int main(void)
{   uint8_t x,y;
    UART_RetType RetVar3;
    GPIO_CheckType RetVar4,RetVar5;
    GPIO_Init();
    RetVar4 = GPIO_SetAlternFuntion(4, 1);
    RetVar5 = GPIO_SetAlternFuntion(5, 1);
    RetVar3 = UART_Init();
    if(RetVar5 == GPIO_OK &&RetVar4==GPIO_OK &&RetVar3==UART_OK){
        BlinkLed(1,0);
    }
    while(1){
        GPIO_Read(1,&x);
        UART_ReceiveByte(0,&y);
        if(!x){//UART_SendByte(0,'c');
        UART_SendString(0,"Welcome to UART Device Driver! ");

        BlinkLed(1,3);
        }
        if(y=='r'){BlinkLed(1,0);y='p';}

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
    if(groupId<4){
    GPIO_Write(groupId,0xff);
    delay100ms(period);
    GPIO_Write(groupId,0x00);
    delay100ms(period);

    }
}
