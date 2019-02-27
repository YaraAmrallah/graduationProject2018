/*
 * adc_m.h
 *
 *  Created on: Feb 7, 2019
 *      Author: yaraa
 */

#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H


#include <stdint.h>
#include "ADC.h"
#include "ADC_Confg.h"


typedef enum {Manager_Uninit=0, Manager_Init, Manager_Busy, Manager_ConversionFinished,Manager_Idle} ADCManager_State;
typedef enum {ADC_M_OK=0, ADC_M_E_BUFFER_NOT_INITIALIZED, ADC_M_E_CALLBACK_NOT_INITIALIZED, ADC_M_E_START_CONVERSION, ADC_M_NOK} ADCManager_FunctionReturn;

ADCManager_FunctionReturn ADCManager_Init();
ADCManager_FunctionReturn ADCManager_StartConversion();
ADCManager_FunctionReturn ADC_GetData(uint8_t ADC_GroupIdx,Adc_Channels ADC_DefinitionIdx,uint16_t* ADCData_Buffer);


#endif

