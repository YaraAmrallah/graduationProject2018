/*
 * button_cfg.c
 *
 *  Created on: Jan 27, 2019
 *      Author: yaraa
 */


#include "button_cfg.h"
#include "button.h"

void nullFunction(void);


const button_ConfigType buttonConfigParam [BUTTON_GROUPS_NUMBER] =
{{0,activeLow,justPressed,nullFunction}, {1,activeLow,justPressed,nullFunction}, {2,activeLow,justPressed,nullFunction}, {3,activeLow,justPressed,nullFunction}};

uint8_t buttonUpdateFlag[BUTTON_GROUPS_NUMBER] = {0};

