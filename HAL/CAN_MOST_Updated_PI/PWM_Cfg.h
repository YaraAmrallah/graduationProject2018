/*
 * PWM_Cfg.h
 *
 *  Created on: Sep 17, 2018
 *      Author: yaraa
 */

#ifndef PWM_CFG_H_
#define PWM_CFG_H_
#include "GPIO.h"


/* Number of defined PWM groups for a maximum of  */
#define PWM_GROUPS_NUMBER 2U

/*A configuration to enable the clock division for the PWM module. */
#define CLOCK_DIVISOR_ENABLE ENABLE /* ENABLE/DISABLE */
#define CLOCK_DIVISOR_VALUE  DIVISOR_BY_64 /* DIVISOR_BY_N where N = {2,4,8,16,32,64} */
#define ANGLE_SHIFT  1u


#endif /* PWM_CFG_H_ */
