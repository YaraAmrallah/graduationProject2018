/*
 * PWM.c
 *
 *  Created on: Sep 17, 2018
 *      Author: yaraa
 */

/* -------------------------------------------------- Modules' Description ----------------------------------------------------------------*/

/*
 * TI Tiva LaunchPad has:
 * - Two on-chip PWM (Pulse Width Modulation) modules: PWM0 and PWM1
 * - Each Module has 4 Generators
 * - Each Generator has a Timer (Counter) with 2 modes: Down or Up/Down
 * - Each Generator has two Compare registers CMPA and CMPB
 * - Each generator has two output pins, 8 PWM pins/module with 3 options: Toggle, driven Low, driven High
 * - When the counter value reaches zero, reloaded, matches the value of the compare registers, the option of the output pin change.
 *
 *
 * */

/* -------------------------------------------------- Headers to include ----------------------------------------------------------------*/

#include "GPIO.h"
#include "PWM.h"
#include "PWM_Cfg.h"
#include "M4MemMap.h"
#include <stdint.h>

/* -------------------------------------------------- Types' Definitions ----------------------------------------------------------------*/

typedef volatile uint32_t* const PWM_RegAddType;

/* Registers memory map */
#define M0PWM_BASE_ADDRESS 0x40028000
#define M1PWM_BASE_ADDRESS 0x40029000

static const uint32_t PWM_ModulesBaseAddressesLut[PWM_MODULES] =
{
      M0PWM_BASE_ADDRESS,
      M1PWM_BASE_ADDRESS
};

#if (CLOCK_DIVISOR_VALUE == DIVISOR_BY_2)
 uint8_t divisor = 2;
#elif (CLOCK_DIVISOR_VALUE == DIVISOR_BY_4)
 uint8_t divisor = 4;
#elif (CLOCK_DIVISOR_VALUE == DIVISOR_BY_8)
 uint8_t divisor = 8;
#elif (CLOCK_DIVISOR_VALUE == DIVISOR_BY_16)
 uint8_t divisor = 16;
#elif (CLOCK_DIVISOR_VALUE == DIVISOR_BY_32)
 uint8_t divisor = 32;
#elif (CLOCK_DIVISOR_VALUE == DIVISOR_BY_64)
 uint8_t divisor = 64;
#endif

#define PWM_REG_ADDRESS(MODULE,REG_OFFSET) (PWM_ModulesBaseAddressesLut[MODULE] + REG_OFFSET)


/* PWM Master & General Control */
#define PWM_M_CTL_R(MODULE)              *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x000))
#define PWM_SYNC_R(MODULE)               *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x004))
#define PWM_ENABLE_R(MODULE)             *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x008))
#define PWM_INVERT_R(MODULE)             *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x00C))
#define PWM_FAULT_R(MODULE)              *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x010))
#define PWM_INTEN_R(MODULE)              *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x014))
#define PWM_RIS_R(MODULE)                *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x018))
#define PWM_ISC_R(MODULE)                *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x01C))
#define PWM_STATUS_R(MODULE)             *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x020))
#define PWM_FAULTVAL_R(MODULE)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x024))
#define PWM_ENUPD_R(MODULE)              *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x028))
#define PWM_MPP_R(MODULE)                *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0xFC0))

/* PWM_X Module Control, where X = 0,1,2,3 represents the Generator */
#define PWM_X_CTL_R(MODULE,X)            *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x040 + X*0x040))
#define PWM_X_INTEN_R(MODULE,X)          *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x044 + X*0x040))
#define PWM_X_RIS_R(MODULE,X)            *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x048 + X*0x040))
#define PWM_X_ISC_R(MODULE,X)            *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x04C + X*0x040))
#define PWM_X_LOAD_R(MODULE,X)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x050 + X*0x040))
#define PWM_X_COUNT_R(MODULE,X)          *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x054 + X*0x040))
#define PWM_X_CMPA_R(MODULE,X)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x048 + X*0x040))
#define PWM_X_CMPB_R(MODULE,X)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x05C + X*0x040))
#define PWM_X_GENA_R(MODULE,X)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x060 + X*0x040))
#define PWM_X_GENB_R(MODULE,X)           *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x064 + X*0x040))
#define PWM_X_DBCTL_R(MODULE,X)          *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x068 + X*0x040))
#define PWM_X_DBRISE_R(MODULE,X)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x06C + X*0x040))
#define PWM_X_DBFALL_R(MODULE,X)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x070 + X*0x040))
#define PWM_X_FLTSRC0_R(MODULE,X)        *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x074 + X*0x040))
#define PWM_X_FLTSRC1_R(MODULE,X)        *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x078 + X*0x040))
#define PWM_X_MINFLTPER_R(MODULE,X)      *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x07C + X*0x040))

/* More Fault Related Registers */
/* PWM_Y Module Fault Sensing and Statuses, where Y = 0,1 represents the Generator */
#define PWM_Y_FLTSEN_R(MODULE,Y)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x800 + Y*0x080))
#define PWM_Y_FLTSTAT0_R(MODULE,Y)       *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x804 + Y*0x080))
#define PWM_Y_FLTSTAT1_R(MODULE,Y)       *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x808 + Y*0x080))
/* PWM_Z Module Fault Statuses, where Z = 2,3 represents the Generator */
#define PWM_2_FLTSTAT0_R(MODULE)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x904)
#define PWM_2_FLTSTAT1_R(MODULE)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x908)
#define PWM_3_FLTSTAT0_R(MODULE)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x984)
#define PWM_3_FLTSTAT1_R(MODULE)         *((PWM_RegAddType) PWM_REG_ADDRESS(MODULE,0x988)

 /* -------------------------------------------------- Functions ----------------------------------------------------------------*/

 /* -------------------------------------------------- Initialization Function ----------------------------------------------------------------*/

/* A function that initializes selected PWM groups */
PWM_CheckType PWM_Init ()
{
    uint8_t LoopIndex;
    PWM_CheckType RetVar = PWM_NOK;
    const PWM_ConfigType * CfgPtr;

    /* Enable/Disable Clock division */
    RCC_REG |= (CLOCK_DIVISOR_ENABLE << 20);
    RCC_REG |= (CLOCK_DIVISOR_VALUE << 17);

    for ( LoopIndex = 0; LoopIndex < PWM_GROUPS_NUMBER ; LoopIndex ++)
    {
            /* Enable Clock for the Module */
            CfgPtr = & PWM_ConfigParam[LoopIndex];
            RCGCPWM_REG |= 1 << CfgPtr->PWM_Module_ID;
            RetVar = PWM_OK;

    }

    return RetVar;
}

/* -------------------------------------------------- Interrupts Functions ----------------------------------------------------------------*/

PWM_CheckType PWM_SetInterruptEvent(uint8_t PWM_ID, PWM_IntEventType IntEvent)
{
    PWM_CheckType RetVar2=PWM_NOK;
    const PWM_ConfigType * CfgPtr2 = & PWM_ConfigParam[PWM_ID];
  if (PWM_ID < PWM_GROUPS_NUMBER)
    {
    PWM_INTEN_R(CfgPtr2->PWM_Module_ID) |= (1 << (CfgPtr2->PWM_Generator_ID)) ;
    if (CfgPtr2->PWM_Pin_ID %2 == 0 )
    {
      PWM_X_INTEN_R(CfgPtr2->PWM_Module_ID, CfgPtr2->PWM_Generator_ID) |= 1 << IntEvent;
    }
    else
    {
        if(IntEvent == EVENT_COUNTER_ZER0 || IntEvent == EVENT_COUNTER_LOAD) {PWM_X_INTEN_R(CfgPtr2->PWM_Module_ID,(CfgPtr2->PWM_Generator_ID)) |= (1<<IntEvent);}
        else {PWM_X_INTEN_R(CfgPtr2->PWM_Module_ID,(CfgPtr2->PWM_Generator_ID)) |= (1 << (IntEvent+2));}
    }
    // mesh 3aref homa mknoosh ktbinha leh
     if (CfgPtr2->PWM_Module_ID == 0)
     {   if(CfgPtr2->PWM_Generator_ID == 3 )
         {EN_INT(45);}
          else{EN_INT(10+CfgPtr2->PWM_Generator_ID);}
     }
     else { EN_INT(134+CfgPtr2->PWM_Generator_ID);}

    }
   RetVar2=PWM_OK;

return RetVar2 ;
}

/* -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- */

/*A function to clear a specific pin interrupt flag*/
PWM_CheckType PWM_ClrInterruptFlag(uint8_t PWM_ID)
{
    PWM_CheckType RetVar3=PWM_NOK;
      const PWM_ConfigType * CfgPtr3 = & PWM_ConfigParam[PWM_ID];
      if (PWM_ID<PWM_GROUPS_NUMBER)
        { PWM_X_ISC_R(CfgPtr3->PWM_Module_ID,(CfgPtr3->PWM_Generator_ID))|=0x1F;
        RetVar3 = PWM_OK;

            }else{
                RetVar3 = PWM_NOK;
            }
    return RetVar3;

}

/* -------------------------------------------------- LA Square Signal generation function ----------------------------------------------------------------*/

/* A function that generates a square signal based on a Left aligned (DOWN) counter mode, frequency and duty cycle given by the user */
/* N.B.: LOAD (frequency) is specified for all the pins of a generator (multiple pins on a single generator CANNOT have different frequencies)
 *       Duty cycle can be specific for each pin independently
 */

/* Enter PWM group's ID, the Clock frequency in KiloHertz, the duty cycle per % */

PWM_CheckType PWM_GenerateLASquareWave (uint8_t PWM_ID, uint16_t PWM_Freq, uint16_t PWM_DCycle )
{
    PWM_CheckType RetVar = PWM_NOK;
    const PWM_ConfigType * CfgPtr;
    CfgPtr = & PWM_ConfigParam[PWM_ID];
    uint16_t load;
    uint16_t clkFreq;
    if ( PWM_ID < PWM_GROUPS_NUMBER )
    {
        /* Reset and Choose immediate update settings */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x00000000;
        /* Set Counter Mode */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= (0 << 1);
        /* Set PWM LOAD register value */
        if ((CLOCK_DIVISOR_ENABLE << 20) == 1)
        {
            clkFreq = 16000/divisor;
        }
        else
        {
            clkFreq = 16000; /* System's clock default frequency */
        }

        load = clkFreq/PWM_Freq;
        PWM_X_LOAD_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = load-1;

        /* Set duty cycle */

        if ((CfgPtr->PWM_Pin_ID % 2) == 0)
        {
            PWM_X_CMPA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - PWM_DCycle)/100 )*(load - 1);
            PWM_X_GENA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
            /*
             * 1000 1100
             * Bits 1:0 = 00 -> Action for when counter = 0, Do nothing
             * Bits 3:2 = 11 -> Action for when counter = LOAD, Drive PWM HIGH
             * Bits 7:6 = 10 -> Action for when counter = comparator A Down, Drive PWM LOW
             */
        }
        else
        {
            PWM_X_CMPB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - PWM_DCycle)/100 )*(load - 1);
            PWM_X_GENB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
        }

        /* Start the timers in PWM generator */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= 1 <<0;

        /* PWM Output Enable */
        PWM_ENABLE_R(CfgPtr->PWM_Module_ID) |= 1 << (CfgPtr->PWM_Pin_ID) ;

        RetVar = PWM_OK;
    }
    else
    {
        RetVar = PWM_NOK;
    }


    return RetVar;

}

/* -------------------------------------------------- DC Motor ----------------------------------------------------------------*/

/* This function is meant to drive a DC Motor based on the frequency, the speed, the polarity given by the user*/
/* N.B.: The selected frequency depends on the motor's type. DC motors have minimum and maximum frequency values to which they can respond */

/* Enter PWM group's ID, the frequency in KiloHertz, the desired mapped speed and the polarity */

PWM_CheckType PWM_DCMotor (uint8_t PWM_ID, uint16_t PWM_Freq, uint16_t motorSpeed, PWM_Polarity motorPolarity )
{
    PWM_CheckType RetVar = PWM_NOK;
    const PWM_ConfigType * CfgPtr;
    CfgPtr = & PWM_ConfigParam[PWM_ID];
    uint16_t absoluteMappingValue;
    uint8_t PWM_DCycle;
    uint16_t load;
    uint16_t clkFreq;

    absoluteMappingValue = CfgPtr->endValue - CfgPtr->startValue;
    PWM_DCycle = ((motorSpeed - CfgPtr->startValue)/absoluteMappingValue)*100;

    if ( PWM_ID < PWM_GROUPS_NUMBER )
    {
        /* Reset and Choose immediate update settings */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x00000000;
        /* Set Counter Mode */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= (0 << 1);

        /* Set PWM LOAD register value */
        if ((CLOCK_DIVISOR_ENABLE << 20) == 1)
        {
            clkFreq = 16000/divisor;
        }
        else
        {
            clkFreq = 16000; /* System's clock default frequency */
        }

        load = clkFreq/PWM_Freq;
        PWM_X_LOAD_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = load-1;

        /* Set duty cycle */

        if ((CfgPtr->PWM_Pin_ID % 2) == 0)
        {
            PWM_X_CMPA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - PWM_DCycle)/100 )*(load - 1);
            PWM_X_GENA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
            /*
             * 1000 1100
             * Bits 1:0 = 00 -> Action for when counter = 0, Do nothing
             * Bits 3:2 = 11 -> Action for when counter = LOAD, Drive PWM HIGH
             * Bits 7:6 = 10 -> Action for when counter = comparator A Down, Drive PWM LOW
             */
        }
        else
        {
            PWM_X_CMPB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - PWM_DCycle)/100 )*(load - 1);
            PWM_X_GENB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
        }

        /* PWM Polarity */
        if (motorPolarity == 1)
        {
            PWM_INVERT_R(CfgPtr->PWM_Module_ID) |= motorPolarity << (CfgPtr->PWM_Pin_ID);
        }
        else
        {
            PWM_INVERT_R(CfgPtr->PWM_Module_ID) &= motorPolarity << (CfgPtr->PWM_Pin_ID);
        }

        /* Start the timers in PWM generator */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= 1 <<0;

        /* PWM Output Enable */
        PWM_ENABLE_R(CfgPtr->PWM_Module_ID) |= 1 << (CfgPtr->PWM_Pin_ID) ;

        RetVar = PWM_OK;
    }
    else
    {
        RetVar = PWM_NOK;
    }


    return RetVar;

}

/* -------------------------------------------------- Servo Motor ----------------------------------------------------------------*/

PWM_CheckType PWM_Initiate_Servo (uint8_t PWM_ID )
{
    PWM_CheckType RetVar = PWM_NOK;
    const PWM_ConfigType * CfgPtr;
    CfgPtr = & PWM_ConfigParam[PWM_ID];
    uint16_t load;
    uint32_t clkFreq;
    if ( PWM_ID < PWM_GROUPS_NUMBER )
    {
        /* Reset and Choose immediate update settings */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x00000000;
        /* Set Counter Mode */
        PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= (0 << 1);
        /* Set PWM LOAD register value */
        if ((CLOCK_DIVISOR_ENABLE << 20) == 1)
        {
            clkFreq = 16000000/divisor;
        }
        else
        {
            clkFreq = 16000000;
        }
        load = clkFreq/50;
        PWM_X_LOAD_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = load-1;

        if ((CfgPtr->PWM_Pin_ID % 2) == 0)
                {
                    PWM_X_CMPA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - 5)/100 )*(load - 1);
                    PWM_X_GENA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;

                }
                else
                {
                    PWM_X_CMPB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - 5)/100 )*(load - 1);
                    PWM_X_GENB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
                }

                /* Start the timers in PWM generator */
                PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= 1 <<0;

                /* PWM Output Enable */
                PWM_ENABLE_R(CfgPtr->PWM_Module_ID) |= 1 << (CfgPtr->PWM_Pin_ID) ;

                RetVar = PWM_OK;

    }

    else
        {
            RetVar = PWM_NOK;
        }
    return RetVar;
}

/* -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- */

PWM_CheckType PWM_Rotate_Servo (uint8_t PWM_ID, uint8_t degree )
{
    PWM_CheckType RetVar = PWM_NOK;
    const PWM_ConfigType * CfgPtr;
    CfgPtr = & PWM_ConfigParam[PWM_ID];
    uint16_t load;
    uint32_t clkFreq;
    float duty= 5+ ((5*degree)/180);
    while(PWM_X_COUNT_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID)!=0);
    PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x00000000;
    PWM_ENABLE_R(CfgPtr->PWM_Module_ID) &= (~ (1 << CfgPtr->PWM_Pin_ID)) ;

        if ((CLOCK_DIVISOR_ENABLE << 20) == 1)
           {
               clkFreq = 16000000/divisor;
           }
           else
           {
               clkFreq = 16000000;
           }
           load = clkFreq/50;
           PWM_X_LOAD_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = load-1;
   if ((CfgPtr->PWM_Pin_ID % 2) == 0)
            {
                         PWM_X_CMPA_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 -duty  )/100 )*(load - 1);


             }
   else
             {
                    PWM_X_CMPB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = ((100 - duty)/100 )*(load - 1);
                    PWM_X_GENB_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) = 0x0000008C;
             }

                           /* Start the timers in PWM generator */
                    PWM_X_CTL_R(CfgPtr->PWM_Module_ID,CfgPtr->PWM_Generator_ID) |= 1 <<0;

                           /* PWM Output Enable */
                  PWM_ENABLE_R(CfgPtr->PWM_Module_ID) |= 1 << (CfgPtr->PWM_Pin_ID) ;

                           RetVar = PWM_OK;



return RetVar;

}

/*-----------------------------------------------------------------------------------------------------------------------------------------*/
