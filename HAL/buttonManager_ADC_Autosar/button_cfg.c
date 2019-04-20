/*
 * button_cfg.c
 *
 *  Created on: Jan 27, 2019
 *      Author: yaraa
 */


#include "button_cfg.h"
#include "button.h"

const button_ConfigType buttonConfigParam [BUTTON_GROUPS_NUMBER] =
{{0,activeLow,justPressed}, {1,activeLow,justPressed}, {2,activeLow,justPressed}, {3,activeLow,justPressed}};

uint8_t buttonUpdateFlag[BUTTON_GROUPS_NUMBER] = {0};

