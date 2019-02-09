/*
 * PWM_Cfg.c
 *
 *  Created on: Sep 17, 2018
 *      Author: yaraa
 */
#include "PWM_cfg.h"
#include "PWM.h"
/* Enter Your Configurations as follows:
 * PWM Module Number: PWM0/PWM1,
 * PWM Generator Number: G0/G1/G2/G3,
 * PWM Pin Number: PB6_4/PB7_4/PB4_4/PB5_4/PE4_4/PE5_4/(PC4_4/PD0_4)/(PC5_4/PD1_4)/PD0_5/PD1_5/(PA6_5/PE4_5)/(PA7_5/PE5_5)/PF0_5/PF1_5/PF2_5/PF3_5,
 *                 N.B.: - Pin Number Format = PPortxPinx_AFbit - Ports between brackets CANNOT be used together
 * PWM Count Mode: DOWN/UP_DOWN
 * DC motor start and end mapping value: used to map voltage values to numbers. Ex.: 0 - 255
 */

const PWM_ConfigType PWM_ConfigParam [PWM_GROUPS_NUMBER]=
{ {PWM0, G0, PB6_4, DOWN, 0, 0} };


