/*
 * PWM.h
 *
 *  Created on: Sep 17, 2018
 *      Author: yaraa
 */

#ifndef PWM_H_
#define PWM_H_
#include <stdint.h>
#include "PWM_Cfg.h"
#include "GPIO.h"

#define PWM_MODULES 2U
#define PWM_GEN 4U

#define DIVISOR_BY_2   0xFFF1FFFF
#define DIVISOR_BY_4   0xFFF3FFFF
#define DIVISOR_BY_8   0xFFF5FFFF
#define DIVISOR_BY_16  0xFFF7FFFF
#define DIVISOR_BY_32  0xFFF9FFFF
#define DIVISOR_BY_64  0xFFFFFFFF


#define ENABLE  1
#define DISABLE 0

typedef enum {PWM_OK = 0, PWM_NOK = 1} PWM_CheckType;
typedef enum {PWM0 = 0, PWM1 = 1} PWM_ModuleNumber;
typedef enum {G0 = 0, G1 = 1, G2 = 2, G3 = 3} PWM_GeneratorNumber;
typedef enum {PB6_4 = 0 , PB7_4 = 1, PB4_4 = 2, PB5_4 = 3,PE4_4 = 4 ,PE5_4 = 5 ,PC4_4 = 6 ,PD0_4 = 6,PC5_4 = 7 ,PD1_4 = 7 ,PD0_5 = 0,PD1_5 = 1 ,
              PA6_5 = 2, PE4_5 = 2, PA7_5 = 3, PE5_5 = 3,PF0_5 = 4,PF1_5 = 5,PF2_5 = 6,PF3_5 = 7 } PWM_PinNumber;
typedef enum {DOWN = 0, UP_DOWN = 1} PWM_CountMode;
typedef enum {NOT_INVERTED = 0, INVERTED = 1} PWM_Polarity;

/* Interrupt related definitions */
typedef enum {EVENT_COUNTER_ZER0 = 0, EVENT_COUNTER_LOAD = 0x01, EVENT_COUNTER_COPMUP= 0x02, EVENT_COUNTER_COPMDOWN= 0x03} PWM_IntEventType;
typedef enum {MASK1_ENABLED = 0xff,MASK1_DISABLED = 0x00} PWM_IntMaskStatus;
typedef enum {INT1_PENDING = 0xff,INT1_INACTIVE = 0x00}PWM_IntStatus;

/* A defined type structure that contains the PWM user configurations */

typedef struct
{
    /* PWM Module number */

    PWM_ModuleNumber PWM_Module_ID;

    /* PWM Generator number */

    PWM_GeneratorNumber PWM_Generator_ID;

    /* PWM pin number */

    PWM_PinNumber PWM_Pin_ID;

    /*PWM Counter Mode */

    PWM_CountMode PWM_Counter_Mode;

    /* These parameters are filled when using a DC motor */

    /* Mapping start value */

    uint8_t startValue;

    /* Mapping end value */

    uint8_t endValue;

} PWM_ConfigType;

extern const PWM_ConfigType PWM_ConfigParam [PWM_GROUPS_NUMBER];

/* A function that initializes selected PWM groups */
PWM_CheckType PWM_Init ();

/* A function that generates a customized square wave using left aligned counter */
/* Enter PWM group's ID, the Clock frequency in KiloHertz, the duty cycle per % */
PWM_CheckType PWM_GenerateLASquareWave (uint8_t PWM_ID, uint16_t PWM_Freq, float PWM_DCycle );

/* This function is meant to drive a DC Motor based on the frequency, the speed, the polarity given by the user*/
/* N.B.: The selected frequency depends on the motor's type. DC motors have minimum and maximum frequency values to which they can respond */
/* Enter PWM group's ID, the frequency in KiloHertz, the desired mapped speed and the polarity */
PWM_CheckType PWM_DCMotor (uint8_t PWM_ID, uint16_t PWM_Freq, uint16_t motorSpeed, PWM_Polarity motorPolarity );

PWM_CheckType PWM_SetInterruptEvent(uint8_t PWM_ID, PWM_IntEventType IntEvent);

/*A function to clear a specific pin interrupt flag*/
PWM_CheckType PWM_ClrInterruptFlag(uint8_t PWM_ID);


#endif /* PWM_H_ */
