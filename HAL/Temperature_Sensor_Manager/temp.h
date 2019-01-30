/*
 * button.h
 *
 *  Created on: Jan 27, 2019
 *      Author: Gebro
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "E:\TIVA c\projects\ADC_17_9\ADC.h"
#include "button_cfg.h"

typedef void(*TEMPERATURE_FnPtrType)(void);
typedef enum{Temperature_OK=0, Temperature_E_PARAM_GROUP,Temperature_E_ADC}Temperature_FunctionValidation;

typedef struct{
    uint8_t AdcGroupIdx;
	uint8_t Temperature_Sensor_Number;
    // TEMPERATURE_FnPtrType CbkFnPtr;
}TempGroup;

Temperature_FunctionValidation Read_Temperature(uint8_t Button_Group, int16_t* Temperature_Reading);


#endif /* BUTTON_H_ */
