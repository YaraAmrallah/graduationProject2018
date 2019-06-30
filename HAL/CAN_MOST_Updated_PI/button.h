/*
 * button.h
 *
 *  Created on: Jan 27, 2019
 *      Author: yaraa
 */


#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <stdlib.h>
#include "button_cfg.h"
#include "GPIO.h"
#include "Adc.h"

typedef enum {buttonNotPressed = 0, buttonPressed = 1, buttonReleased = 2, buttonStuck = 3, shortCircuit = 4} buttonState;
typedef enum {justPressed = 0, longPress = 1}buttonPressType;
typedef enum {button_OK = 0, button_NOK = 1} button_CheckType;
typedef enum {activeHigh = 0, activeLow = 1} activeType;
typedef enum {ButtonIdle = 0, ButtonBusy, ButtonReady} buttonStatus;
typedef void (*BUTTON_CallBackPtr)(void);
#define ADC_SAMPLING_VALUE 4095

typedef struct
{
    /* Button ID */

    Adc_GroupType button_ID;

    /* Active high or low  */

    activeType buttonActive;

    /* Button press type*/

    buttonPressType buttonHighStateType;

    BUTTON_CallBackPtr stateReadyCallBack ;

} button_ConfigType;

extern const button_ConfigType buttonConfigParam [BUTTON_GROUPS_NUMBER];
extern uint8_t buttonUpdateFlag[BUTTON_GROUPS_NUMBER];

/*---------------------------------------------- Functions ------------------------------------------------------*/

button_CheckType Button_Init ();
void Button_EnableStateRequest(uint8_t buttonIndex);
void Button_DisableStateRequest(uint8_t buttonIndex);
void Button_MainFunction_Handling ();
static void Button_StateCalculation (uint8_t buttonIndex);
buttonState Button_ReturnState (uint8_t buttonIndex);


#endif /* BUTTON_H_ */
