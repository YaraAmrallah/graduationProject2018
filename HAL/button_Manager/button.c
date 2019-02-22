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
 * - When the counter value reaches zero, reloaded, matches the value of the compare registers, the option of the output pin change.
 *
 *
 * */


/*--------------------------------------------------- ADC MAnager functions -------------------------------------------------------------*/
/*
 * ADCManager_FunctionReturn ADCManager_Init();
 * ADCManager_FunctionReturn ADCManager_StartConversion();
 * ADCManager_FunctionReturn ADC_GetData(uint8_t ADC_GroupIdx,uint8_t ADC_DefinitionIdx,uint16_t* ADCData_Buffer);
 *
 */

/* -------------------------------------------------- Headers to include ----------------------------------------------------------------*/

#include "ADC.h"
#include "button.h"
#include "button_cfg.h"
#include "M4MemMap.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"

/*-------------------------------------------- Things that should be added to GPIO.h ---------------------------------------------------*/

/*
 * #define GPIOPUR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x510))
 * #define GPIOPDR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x514))
 */

/*-------------------------------------------------------- Structures ------------------------------------------------------------------*/
/* Structure that must be defined as global to the button functions in the code */

Adc_ValueGroupType dataArray[BUTTON_GROUPS_NUMBER][MINIMUM_NUMBER_OF_SAMPLES];
uint8_t countArray[BUTTON_GROUPS_NUMBER] = {0};
uint8_t secondCountArray[BUTTON_GROUPS_NUMBER] = {0};
uint16_t ADC_array[BUTTON_GROUPS_NUMBER] = {0};
buttonState lastStatusArray[BUTTON_GROUPS_NUMBER] = {buttonNotPressed};

/*-------------------------------------------------------- Functions -------------------------------------------------------------------*/

/*--------------------------- Function that initializes each button group and performs user's inputs checks ----------------------------*/

button_CheckType buttonInitialization ()
{
    uint8_t loopIndex;
    button_CheckType retVar = button_NOK;
    Adc_ReturnType checkBuffer = ADC_NOK;
    Adc_ReturnType checkConversion = ADC_NOK;

    for (loopIndex = 0; loopIndex < BUTTON_GROUPS_NUMBER ; loopIndex ++)
    {
        //Pull up and pull down resistors ?
        if(buttonConfigParam[loopIndex].buttonActive == activeHigh)
        {
            //GPIOPUR_REG(CfgPtr->button_ID) |= (buttonConfigParam[loopIndex].buttonMask & 0x00);
            //GPIOPDR_REG(CfgPtr->button_ID) |= (buttonConfigParam[loopIndex].buttonMask & 0xFF);
            retVar = button_OK;
        }
        else if (buttonConfigParam[loopIndex].buttonActive == activeLow)
        {
            //GPIOPUR_REG(CfgPtr->button_ID) |= (buttonConfigParam[loopIndex].buttonMask & 0xFF);
            //GPIOPDR_REG(CfgPtr->button_ID) |= (buttonConfigParam[loopIndex].buttonMask & 0x00);
            retVar = button_OK;
        }
        else
        {
            retVar = button_NOK;
        }

        if((buttonConfigParam[loopIndex].buttonHighStateType == justPressed ) && (retVar == button_OK))
        {
            /* do nothing */
        }
        else if ((buttonConfigParam[loopIndex].buttonHighStateType == longPress) && (retVar == button_OK))
        {
            /* do nothing */
        }
        else
        {
            retVar = button_NOK;
        }

        checkBuffer = Adc_SetupResultBuffer(buttonConfigParam[loopIndex].button_ID, &ADC_array[loopIndex]);
        checkConversion = Adc_StartGroupConversion(buttonConfigParam[loopIndex].button_ID);
        if((checkBuffer == ADC_OK) && (checkConversion == ADC_OK) && (retVar == button_OK ))
        {
            /* do nothing */
        }
        else
        {
            retVar = button_NOK;
        }
    }

    return retVar;
}

/*------------------------------ A function that requests data from the ADC manager ----------------------------*/

button_CheckType buttonRequest (uint8_t buttonIndex)
{
    button_CheckType retVar = button_NOK;
    //const AdcGroup* ADC_confPtr;
    Adc_StatusType checkStatus;
    Adc_ReturnType checkRead = ADC_NOK;
    uint16_t VDDA_value = VDDA_ADC_VALUE;

    if (buttonIndex < BUTTON_GROUPS_NUMBER)
    {
        //ADC_confPtr = &Adc_GroupParameters[buttonConfigParam[buttonIndex].button_ID];


        if((buttonConfigParam[buttonIndex].buttonHighStateType == justPressed) || (buttonConfigParam[buttonIndex].buttonHighStateType == longPress))
        {
            //checkBuffer = ADC_GetData(buttonConfigParam[buttonIndex].button_ID, *(ADC_confPtr->AdcGroupDefinition),&dataArray[buttonIndex][countArray[buttonIndex]]);
            checkStatus = Adc_GetGroupStatus(buttonConfigParam[buttonIndex].button_ID);


            if(checkStatus == ADC_STREAM_COMPLETED)
            {
                checkRead = Adc_ReadGroup(buttonConfigParam[buttonIndex].button_ID, &dataArray[buttonIndex][countArray[buttonIndex]]);

                if(checkRead == ADC_OK)
                {
                    retVar = button_OK;
                    /* N.B.: Precision is multiplied by 100 */
                    dataArray[buttonIndex][countArray[buttonIndex]] = (dataArray[buttonIndex][countArray[buttonIndex]])*(VDDA_value)/4095;
                    countArray[buttonIndex] = countArray[buttonIndex] + 1;
                }
                else
                {
                    /* do nothing */
                }
            }
            else
            {
                /* do nothing */
            }

        }
        else
        {
            /* do nothing */
        }
    }

    else
    {
        /* do nothing */
    }

    return retVar;
}

/*---------------------------------- Function that returns the button State --------------------------------------------------*/

buttonState returnButtonState (uint8_t buttonIndex)
{
    buttonPressType pressType = buttonConfigParam[buttonIndex].buttonHighStateType;
    buttonState returnButtonState = lastStatusArray[buttonIndex];
    uint8_t loopIndex;
    uint8_t highFlag = 0;
    uint8_t lowFlag = 0;
    uint8_t SC_flag = 0;
    uint16_t VDDA_value = VDDA_ADC_VALUE;

    if(countArray[buttonIndex] == (MINIMUM_NUMBER_OF_SAMPLES))
    {
        /* reset the counter */
        countArray[buttonIndex] = 0;

        if(buttonConfigParam[buttonIndex].buttonActive == activeHigh)
        {
            for(loopIndex = 0; loopIndex < MINIMUM_NUMBER_OF_SAMPLES; loopIndex++)
            {
                if ((dataArray[buttonIndex][loopIndex] <= (VDDA_value)) && (dataArray[buttonIndex][loopIndex] > (0.6*VDDA_value)))
                {
                    highFlag++;
                }
                /*else if ((dataArray[buttonIndex][loopIndex] <= VDDA_value) && (dataArray[buttonIndex][loopIndex] > (0.9*VDDA_value)))
                {
                    SC_flag++ ;
                }*/
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
                if ((dataArray[buttonIndex][loopIndex] < (0.109*VDDA_value)) && (dataArray[buttonIndex][loopIndex] > 0))
                {
                    highFlag++;
                }
                else if (dataArray[buttonIndex][loopIndex] == 0)
                {
                    SC_flag++ ;
                }
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
        switch (pressType)
        {

        case justPressed:

            /* decision */
            if (highFlag == (MINIMUM_NUMBER_OF_SAMPLES) && (secondCountArray[buttonIndex] != (STUCK_SHORT - 1)))
            {
                lastStatusArray[buttonIndex] = buttonPressed;
                returnButtonState = buttonPressed;
                secondCountArray[buttonIndex]++;
            }
            else if ((lowFlag <= (MINIMUM_NUMBER_OF_SAMPLES)) && (lowFlag > NOT_PRESSED_PERCENTAGE(MINIMUM_NUMBER_OF_SAMPLES)) && (lastStatusArray[buttonIndex] == buttonPressed))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonReleased;
                secondCountArray[buttonIndex] = 0;
            }
            else if (SC_flag == (MINIMUM_NUMBER_OF_SAMPLES))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = shortCircuit;
                secondCountArray[buttonIndex] = 0;
            }
            else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (secondCountArray[buttonIndex] == (STUCK_SHORT - 1)))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonStuck;
                secondCountArray[buttonIndex] = 0;
            }
            else
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonNotPressed;
                secondCountArray[buttonIndex] = 0;
            }


            break;
        case longPress:
            if (highFlag == (MINIMUM_NUMBER_OF_SAMPLES) && (secondCountArray[buttonIndex] == 0) && (secondCountArray[buttonIndex] < (STUCK_LONG-1)))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonNotPressed;
                secondCountArray[buttonIndex]++;
            }
            else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (secondCountArray[buttonIndex] >=1 ) && (secondCountArray[buttonIndex] < ((2*STUCK_LONG) - 1)))
            {
                lastStatusArray[buttonIndex] = buttonPressed;
                returnButtonState = buttonPressed;
                secondCountArray[buttonIndex]++;
            }
            else if ((lowFlag <= (MINIMUM_NUMBER_OF_SAMPLES)) && (lowFlag > NOT_PRESSED_PERCENTAGE(MINIMUM_NUMBER_OF_SAMPLES)) && (lastStatusArray[buttonIndex] == buttonPressed))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonReleased;
                secondCountArray[buttonIndex] = 0;
            }
            else if (SC_flag == (MINIMUM_NUMBER_OF_SAMPLES))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = shortCircuit;
                secondCountArray[buttonIndex] = 0;
            }
            else if ((highFlag == (MINIMUM_NUMBER_OF_SAMPLES)) && (secondCountArray[buttonIndex] == ((2*STUCK_LONG) - 1)))
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonStuck;
                secondCountArray[buttonIndex] = 0;
            }
            else
            {
                lastStatusArray[buttonIndex] = buttonNotPressed;
                returnButtonState = buttonNotPressed;
                secondCountArray[buttonIndex] = 0;
            }

            break;
        }
    }
    else
    {
        /* do nothing */
    }

    return returnButtonState;
}





