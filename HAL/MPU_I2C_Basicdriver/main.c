/**
 * main.c
 */
#include "stdint.h"
#include "GPIO.h"
#include "MPU_Manager.h"

#define FLAG_SAFEMODE                           '0'
#define FLAG_ACCIDENT                           '1'

void BlinkLed(uint8_t period, uint8_t groupId);
void delay100ms(uint64_t time);

extern Accident_CheckType Indicator;

typedef enum
{
    MPU_DONE = 0, MPU_UDONE
} MPU_Statetype;
static MPU_Statetype MPU_Flag;
void MPU_CallBack(void)
{
    MPU_Flag = MPU_DONE;
}

int main(void)
{

    while (1)
    {
        if (MPU_Flag == MPU_DONE)
        {
            MPU_Flag = MPU_UDONE;
            Request_MPU();
        }
        MPU_Cyclic();

        if (Indicator == ACCIDENT)
        {
            BlinkLed(15, 4);
        }

    }

}
void delay100ms(uint64_t time)
{
    unsigned long i;
    while (time > 0)
    {
        i = 133333;
        while (i > 0)
        {
            i--;
        }
        time--;
    }
}

void BlinkLed(uint8_t period, uint8_t groupId)
{
    if (groupId < 6)
    {
        GPIO_Write(groupId, 0xff);
        delay100ms(period);
        GPIO_Write(groupId, 0x00);
        delay100ms(period);

    }
}
