/*
 * SERVO.c
 *
 *  Created on: Jan 29, 2019
 *      Author: Ayman-Pc
 */

#include "SERVO.h"


PWM_CheckType Rotate_Servo (uint8_t PWM_ID, uint8_t degree )
{
    PWM_CheckType RetVar = PWM_NOK;
    float duty= ((2.5)+ ((10*degree)/180.0));
    RetVar=  PWM_GenerateLASquareWave ( PWM_ID, 50, duty );


return RetVar;

}
PWM_CheckType Initiate_Servo (uint8_t PWM_ID )
{
    PWM_CheckType RetVar = PWM_NOK;
    RetVar=  PWM_GenerateLASquareWave ( PWM_ID, 50, 2.5 );
    return RetVar;
}

/* -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- */

PWM_CheckType AngleUp_Servo(uint8_t PWM_ID,uint8_t Pre_angle , uint8_t * Upcom_angle)
{
	 PWM_CheckType RetVar = PWM_NOK;
	 float duty= ((2.5)+ ((10*(Pre_angle+3))/180.0));
	 RetVar=  PWM_GenerateLASquareWave ( PWM_ID, 50, duty );
	 * Upcom_angle= (Pre_angle+3);


	 return RetVar;
}



PWM_CheckType AngleDown_Servo(uint8_t PWM_ID,uint8_t Pre_angle , uint8_t * Upcom_angle)
{
	PWM_CheckType RetVar = PWM_NOK;
		 float duty= ((2.5)+ ((10*(Pre_angle-3))/180.0));
		 RetVar=  PWM_GenerateLASquareWave ( PWM_ID, 50, duty );
		 * Upcom_angle= (Pre_angle-3);


		 return RetVar;
}
