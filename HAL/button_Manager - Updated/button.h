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
#include "ADC.h"

typedef enum {buttonNotPressed = 0, buttonPressed = 1, buttonReleased = 2, buttonStuck = 3, shortCircuit = 4, buttonError} buttonState;
typedef enum {justPressed = 0, longPress = 1}buttonPressType;
typedef enum {button_OK = 0, button_NOK = 1} button_CheckType;
typedef enum {activeHigh = 0, activeLow = 1} activeType;

#define ADC_SAMPLING_VALUE 4095

typedef struct
{
    /* Button ID */

    uint8_t button_ID;

    /* Button Group Index */

    uint8_t buttonIndex;

    /* Active high or low  */

    activeType buttonActive;

    /* Button press type*/

    buttonPressType buttonHighStateType;

} button_ConfigType;

extern const button_ConfigType buttonConfigParam [BUTTON_GROUPS_NUMBER];

/*---------------------------------------------- Functions ------------------------------------------------------*/

button_CheckType buttonInitialization ();
button_CheckType buttonStartADC_conversion(uint8_t buttonIndex);
button_CheckType mainButtonRequest (uint8_t buttonIndex);
buttonState returnButtonState (uint8_t buttonIndex);


#endif /* BUTTON_H_ */
