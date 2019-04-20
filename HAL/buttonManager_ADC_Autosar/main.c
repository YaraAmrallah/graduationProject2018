

/**
 * main.c
 */

#include "button.h"

int main(void)
{
    uint8_t loopIndex;
    uint8_t x=0;
    GPIO_Init();
    Adc_Init(&Adc_Config);
    Button_Init ();
    Button_EnableStateRequest (0);
    Button_EnableStateRequest (1);
    Button_EnableStateRequest (2);
    Button_EnableStateRequest (3);
    buttonState returnedState[BUTTON_GROUPS_NUMBER];
    uint32_t i = 0;

    while(1)
    {
        Button_MainFunction_Handling ();
        for (loopIndex = 0; loopIndex < BUTTON_GROUPS_NUMBER; loopIndex ++)
        {

            if (buttonUpdateFlag[loopIndex] == 1)
            {
                returnedState[loopIndex] = Button_ReturnState(loopIndex);

                if (returnedState[loopIndex] == buttonPressed)
                {

                    GPIO_Write(4,0xff);
                }
                else if (returnedState[loopIndex] == buttonReleased)
                {
                    x=1;
                    GPIO_Write(5,0xff);
                }
                else if (returnedState[loopIndex] == buttonStuck)
                {
                    x=1;
                    GPIO_Write(6,0xff);
                }

                else
                {

                    GPIO_Write(4,0x00);
                    GPIO_Write(5,0x00);
                    GPIO_Write(6,0x00);

                }

            }

        }


        for(i = 0 ; i < 2000 ; i++);
    }
    return 0;
}
