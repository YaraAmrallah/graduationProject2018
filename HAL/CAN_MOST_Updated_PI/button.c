/*
 * button.c
 *
 *  Created on: Jan 27, 2019
 *      Author: yaraa
 */

/* -------------------------------------------------- Manager's Description ----------------------------------------------------------------*/

/*
 * The button manager's purpose is to differentiate between the possible button's states:
 * - buttonNotPressed: the button is active low, connected to the ground.
 * - justPressed: the button is pressed for a short amount of time, the user has to take action based on a single click.
 * - longPress: the user has to press continuously to take a continuous/non-continuous action.
 * - buttonStuck: the button may be stuck by the user or by any mechanical damage.
 * - shortCircuit: the resistance of the
 *
 *
 * */

/* -------------------------------------------------- Headers to include ----------------------------------------------------------------*/

#include "ADC.h"
#include "button.h"
#include "button_cfg.h"
#include "M4MemMap.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"


/*-------------------------------------------------------- Global Variables ------------------------------------------------------------------*/

Adc_ValueGroupType dataArray[BUTTON_GROUPS_NUMBER][MINIMUM_NUMBER_OF_SAMPLES];
static uint8_t countArray[BUTTON_GROUPS_NUMBER] = {0};
static buttonState currentStatusArray[BUTTON_GROUPS_NUMBER] = {buttonNotPressed};
static uint8_t stuckCountArray[BUTTON_GROUPS_NUMBER] = {0};
static Adc_ValueGroupType ADC_array[BUTTON_GROUPS_NUMBER] = {0};
static buttonState lastStatusArray[BUTTON_GROUPS_NUMBER] = {buttonNotPressed};
static uint8_t flag[BUTTON_GROUPS_NUMBER] = {0};
static buttonStatus statusArray[BUTTON_GROUPS_NUMBER] = {ButtonIdle};
static uint8_t buttonRequest[BUTTON_GROUPS_NUMBER] = {0};


/*-------------------------------------------------------- Functions -------------------------------------------------------------------*/

/*--------------------------- Function that initializes each button group and performs user's inputs checks ----------------------------*/

button_CheckType Button_Init ()
{
    uint8_t loopIndex;
    button_CheckType retVar = button_NOK;
    Std_ReturnType checkBuffer = E_NOT_OK;
    uint8_t initFlag = 0;

    for (loopIndex = 0; loopIndex < BUTTON_GROUPS_NUMBER; loopIndex ++)
    {
        if((buttonConfigParam[loopIndex].buttonActive == activeHigh) || (buttonConfigParam[loopIndex].buttonActive == activeLow))
        {
            if((buttonConfigParam[loopIndex].buttonHighStateType == justPressed) || (buttonConfigParam[loopIndex].buttonHighStateType == longPress))
            {
                checkBuffer = Adc_SetupResultBuffer(buttonConfigParam[loopIndex].button_ID, &ADC_array[loopIndex]);

                if(checkBuffer == E_OK)
                {
                    initFlag++ ;
                }
                else
                {
                    /* do nothing */
                }
            }
        }
        else
        {
            /* do nothing */
        }
    }

    if (initFlag == BUTTON_GROUPS_NUMBER)
    {
        retVar = button_OK;
    }
    else
    {
        /* do nothing: Error in initialization */
    }

    return retVar;
}

/*------------------------------ A function that requests the button status ------------------------------------*/

void Button_EnableStateRequest(uint8_t buttonIndex)
{
    buttonRequest[buttonIndex] = 1;
}

/*---------------------------- A function that disables the button status request -------------------------------*/

void Button_DisableStateRequest(uint8_t buttonIndex)
{
    buttonRequest[buttonIndex] = 0;
    flag[buttonIndex] = 0;
}
/*------------------------------ A function that requests data from the ADC manager ----------------------------*/

void Button_MainFunction_Handling ()
{
    Adc_StatusType checkStatus;
    Std_ReturnType checkRead = E_NOT_OK;
    uint16_t VDDA_value = VDDA_ADC_VALUE;
    uint8_t buttonIndex = 0;

    for (buttonIndex = 0; buttonIndex < BUTTON_GROUPS_NUMBER; buttonIndex++)
    {
        switch(statusArray[buttonIndex])
        {

        case ButtonIdle:

            if(buttonRequest[buttonIndex] == 1)
            {
                // Adc_StartGroupConversion(buttonIndex);
                buttonUpdateFlag[buttonIndex]= 0;
                statusArray[buttonIndex] = ButtonBusy;
            }
            else
            {
                /* do nothing */
            }

            break ;


        case ButtonBusy:

            Adc_StartGroupConversion(buttonIndex);
            checkStatus = Adc_GetGroupStatus(buttonConfigParam[buttonIndex].button_ID);

            if(checkStatus == ADC_STREAM_COMPLETED)
            {
                checkRead = Adc_ReadGroup(buttonConfigParam[buttonIndex].button_ID, &dataArray[buttonIndex][countArray[buttonIndex]]);

                if(checkRead == E_OK)
                {
                    /* N.B.: Precision is multiplied by 100 */
                    dataArray[buttonIndex][countArray[buttonIndex]] = (dataArray[buttonIndex][countArray[buttonIndex]])*(VDDA_value)/ADC_SAMPLING_VALUE;

                    /* Stuck handling */

                    if(buttonConfigParam[buttonIndex].buttonActive == activeHigh)
                    {
                        if ((dataArray[buttonIndex][countArray[buttonIndex]] <= (VDDA_value)) && (dataArray[buttonIndex][countArray[buttonIndex]] > (0.6*VDDA_value)))
                        {
                            stuckCountArray[buttonIndex] = stuckCountArray[buttonIndex] + 1;
                        }
                        else
                        {
                            stuckCountArray[buttonIndex] = 0;
                        }
                    }
                    else
                    {
                        if ((dataArray[buttonIndex][countArray[buttonIndex]] < (0.109*VDDA_value)) && (dataArray[buttonIndex][countArray[buttonIndex]] > 0))
                        {
                            stuckCountArray[buttonIndex] = stuckCountArray[buttonIndex] + 1;
                        }
                        else
                        {
                            stuckCountArray[buttonIndex] = 0;
                        }
                    }

                    /* --------------------------------------------------------------------------------------------------------------------*/
                    /*Add samples in a cyclic queue*/
                    if(countArray[buttonIndex] == (MINIMUM_NUMBER_OF_SAMPLES-1))
                    {
                        /* reset the counter */
                        countArray[buttonIndex] = 0;
                    }
                    else
                    {
                        countArray[buttonIndex] = countArray[buttonIndex] + 1;
                    }

                    if (flag[buttonIndex] < (MINIMUM_NUMBER_OF_SAMPLES-1))
                    {
                        flag[buttonIndex] = flag[buttonIndex] + 1;
                    }
                    else
                    {
                        statusArray[buttonIndex] =  ButtonReady;
                    }
                }
                else
                {
                    /* do nothing, ADC haven't returned any value */
                }
            }
            else
            {
                /* do nothing, ADC haven't returned any value */
            }

            break ;


        case ButtonReady:

            Button_StateCalculation(buttonIndex);
            statusArray[buttonIndex] =  ButtonIdle;
            buttonUpdateFlag[buttonIndex]= 1;
            buttonConfigParam[buttonIndex].stateReadyCallBack();

            break;
        }
    }
}

/*---------------------------------- Function that returns the button State --------------------------------------------------*/

static void Button_StateCalculation (uint8_t buttonIndex)
{
    buttonPressType pressType = buttonConfigParam[buttonIndex].buttonHighStateType;
    uint8_t loopIndex;
    uint8_t highFlag = 0;
    uint8_t lowFlag = 0;
    uint8_t SC_flag = 0;
    uint16_t VDDA_value = VDDA_ADC_VALUE;

    currentStatusArray[buttonIndex] = lastStatusArray[buttonIndex];

    if(buttonConfigParam[buttonIndex].buttonActive == activeHigh)
    {
        for(loopIndex = 0; loopIndex < MINIMUM_NUMBER_OF_SAMPLES; loopIndex++)
        {
            if ((dataArray[buttonIndex][loopIndex] <= (VDDA_value)) && (dataArray[buttonIndex][loopIndex] > (0.6*VDDA_value)))
            {
                highFlag++;
            }
            else if (dataArray[buttonIndex][loopIndex] < (0.4*VDDA_value))
            {
                lowFlag++;
            }
            else
            {
                /* do nothing */
            }
        }
    }
    else
    {

        for(loopIndex = 0; loopIndex < MINIMUM_NUMBER_OF_SAMPLES; loopIndex++)
        {
            if ((dataArray[buttonIndex][loopIndex] < (0.109*VDDA_value)) && (dataArray[buttonIndex][loopIndex] >= 0))
            {
                highFlag++;
            }
            /*else if (dataArray[buttonIndex][loopIndex] == 0)
            {
                SC_flag++ ;
            }*/
            else if ((dataArray[buttonIndex][loopIndex] <= VDDA_value) && (dataArray[buttonIndex][loopIndex] > (0.6*VDDA_value)))
            {
                lowFlag++;
            }
            else
            {
                /* do nothing */
            }
        }
    }

    /* decision */
    switch (pressType)
    {

    case justPressed:

        if (highFlag == (MINIMUM_NUMBER_OF_SAMPLES) && (stuckCountArray[buttonIndex] != (STUCK_SHORT - 1)))
        {
            lastStatusArray[buttonIndex] = buttonPressed;
            currentStatusArray[buttonIndex] = buttonPressed;
            // stuckCountArray[buttonIndex]++;
        }
        else if ((lowFlag <= (MINIMUM_NUMBER_OF_SAMPLES)) && (lowFlag > NOT_PRESSED_PERCENTAGE(MINIMUM_NUMBER_OF_SAMPLES)) && (lastStatusArray[buttonIndex] == buttonPressed))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonReleased;
            // stuckCountArray[buttonIndex] = 0;
        }
        else if (SC_flag == (MINIMUM_NUMBER_OF_SAMPLES))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = shortCircuit;
            // stuckCountArray[buttonIndex] = 0;
        }
        else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (stuckCountArray[buttonIndex] == (STUCK_SHORT - 1)))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonStuck;
            stuckCountArray[buttonIndex] = 0;
        }
        else
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonNotPressed;
            // stuckCountArray[buttonIndex] = 0;
        }


        break;

    case longPress:

        if (highFlag == (MINIMUM_NUMBER_OF_SAMPLES) && (stuckCountArray[buttonIndex] == MINIMUM_NUMBER_OF_SAMPLES) && (stuckCountArray[buttonIndex] < (STUCK_LONG-1)))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonNotPressed;
            // stuckCountArray[buttonIndex]++;
        }
        else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (stuckCountArray[buttonIndex] >= (2*MINIMUM_NUMBER_OF_SAMPLES)) && (stuckCountArray[buttonIndex] < (STUCK_LONG - 1)))
        {
            lastStatusArray[buttonIndex] = buttonPressed;
            currentStatusArray[buttonIndex] = buttonPressed;
            // stuckCountArray[buttonIndex]++;
        }
        else if ((lowFlag <= (MINIMUM_NUMBER_OF_SAMPLES)) && (lowFlag > NOT_PRESSED_PERCENTAGE(MINIMUM_NUMBER_OF_SAMPLES)) && (lastStatusArray[buttonIndex] == buttonPressed))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonReleased;
            // stuckCountArray[buttonIndex] = 0;
        }
        else if (SC_flag == (MINIMUM_NUMBER_OF_SAMPLES))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = shortCircuit;
            // stuckCountArray[buttonIndex] = 0;
        }
        else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (stuckCountArray[buttonIndex] == (STUCK_LONG - 1)))
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonStuck;
            stuckCountArray[buttonIndex] = 0;
        }
        else
        {
            lastStatusArray[buttonIndex] = buttonNotPressed;
            currentStatusArray[buttonIndex] = buttonNotPressed;
            // stuckCountArray[buttonIndex] = 0;
        }

        break;
    }
}

/* ------------------------------------------ Return the button state to the user ------------------------------------------*/

buttonState Button_ReturnState (uint8_t buttonIndex)
{
    buttonState returnButtonState = currentStatusArray[buttonIndex];
    return returnButtonState;
}



