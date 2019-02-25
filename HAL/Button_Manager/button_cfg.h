/*
 * button_cfg.h
 *
 *  Created on: Jan 27, 2019
 *      Author: yaraa
 */

#ifndef BUTTON_CFG_H_
#define BUTTON_CFG_H_

#define VDDA_ADC_VALUE 330u; /* multiply the VDDA voltage by 100 */
#define BUTTON_GROUPS_NUMBER 1u
#define MINIMUM_NUMBER_OF_SAMPLES 5u
#define NOT_PRESSED_PERCENTAGE(X) (uint16_t)(0.2*X-1)
#define STUCK_SHORT 10u
#define STUCK_LONG 3u

#endif /* BUTTON_CFG_H_ */
