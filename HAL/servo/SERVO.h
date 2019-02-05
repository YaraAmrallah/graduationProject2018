/*
 * SERVO.h
 *
 *  Created on: Jan 29, 2019
 *      Author: Ayman-Pc
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "PWM.h"
PWM_CheckType Rotate_Servo (uint8_t PWM_ID, uint8_t degree );
PWM_CheckType Initiate_Servo (uint8_t PWM_ID );
PWM_CheckType AngleUp_Servo(uint8_t PWM_ID,uint8_t Pre_angle , uint8_t * Upcom_angle);
PWM_CheckType AngleDown_Servo(uint8_t PWM_ID,uint8_t Pre_angle , uint8_t * Upcom_angle);


#endif /* SERVO_H_ */
