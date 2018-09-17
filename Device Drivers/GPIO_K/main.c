/*
 * main.c
 */
#include <stdint.h>
#include "GPIO.h"
void delay100ms(uint64_t time);
void BlinkLed(uint8_t period,uint8_t groupId);
int main(void) {
   //uint8_t x;
    GPIO_IntStatus y;
    GPIO_IntMaskStatus mask = MASK_ENABLED;
    GPIO_IntEventType Type = EVENT_FALL_EDGE;
    GPIO_Init();
    GPIO_SetInterruptEvent(1,Type,mask);

    /*******Interrupt test*********/
    while(1){

        //polling on the flag fails when ISR or portF is not mentioned in the startup file.
      /*GPIO_GetInterruptStatus(1,&y);
       if(y == INT_PENDING){
            BlinkLed(0,0);
            GPIO_ClrInterruptFlag(1);
        }else if(y == INT_INACTIVE){
            BlinkLed(3,3);
        }
       GPIO_ClrInterruptFlag(1);*/
    }


/*******read write test*********/
    /*while(1){
    GPIO_Read(1,&x);
    if(!x){
        GPIO_Write(3,0xff);
        delay100ms(1);
    }else{
        GPIO_Write(3,0x00);
        delay100ms(1);
    }
    }*/
	//return 0;
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

void ISR_PortF(void){
    GPIO_ClrInterruptFlag(1);
    BlinkLed(1,0);
}
