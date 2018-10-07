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
#define PWM_GROUPS_NUMBER 1U

/*A configuration to enable the clock division for the PWM module. */
#define CLOCK_DIVISOR_ENABLE DISABLE /* ENABLE/DISABLE */
#define CLOCK_DIVISOR_VALUE  DIVISOR_BY_2 /* DIVISOR_BY_N where N = {2,4,8,16,32,64} */


#endif /* PWM_CFG_H_ */
