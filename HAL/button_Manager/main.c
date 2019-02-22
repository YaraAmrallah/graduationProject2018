

/**
 * main.c
 */

#include "button.h"

int main(void)
{
    GPIO_Init();
    ADC_Init();
    buttonInitialization ();
    buttonState returnedState;
    uint16_t i = 0;

    while(1)
    {
        buttonRequest(0);
        returnedState = returnButtonState(0);

        if (returnedState == buttonPressed)
        {
            GPIO_Write(2,0xff);
        }
        else if (returnedState == buttonReleased)
        {
            GPIO_Write(3,0xff);
        }
        else if (returnedState == buttonStuck)
        {
            GPIO_Write(4,0xff);
        }

        else
        {
            GPIO_Write(2,0x00);
            GPIO_Write(3,0x00);
            GPIO_Write(4,0x00);
        }

        for(i = 0 ; i < 20000 ; i++);
    }
	return 0;
}
