/*
 * temp.c
 *
 *  Created on: Jan 27, 2019
 *      Author: Gebro
 */
#include "temp.h"
#define MAX_DIGITAL_VALUE 4095
#define MAX_TEMPSENSOR_NUMBER_PER_GROUP 8


extern TempGroup Temp_GroupParameters[NUMBER_OF_TEMPS_GROUPS];

Temperature_FunctionValidation Read_Temperature(uint8_t TempGroupIdx, int16_t* Temperature_Reading)
{
    Adc_StatusType Adc_Statues;
	Adc_FunctionValidation AdcFunc_Validate=ADC_OK;
	Temperature_FunctionValidation TempFunc_Validate = Button_OK;
	
	TempGroup* GrpPtr= &Temp_GroupParameters[TempGroupIdx];
	
	uint16_t Adc_ReadingsBuffer[MAX_TEMPSENSOR_NUMBER_PER_GROUP]={0};
	uint8_t i = 0;
    if(TempGroupIdx >= NUMBER_OF_GROUPS)
        return Temperature_E_ADC;
    else
    {
			AdcFunc_Validate = Adc_ReadGroup(GrpPtr->AdcGroupIdx,Adc_ReadingsBuffer);
			if(AdcFunc_Validate != ADC_OK)
			{
				TempFunc_Validate = Temperature_E_ADC;
			}
			else 
			{
				for(i = 0;i < GrpPtr->Buttons_Number;i++)
				{
					Adc_ReadingsBuffer[i] = (Adc_ReadingsBuffer[i]*MAX_VOLT)/MAX_DIGITAL_VALUE;
				}
				AdcFunc_Validate = Adc_StartGroupConversion(GrpPtr->AdcGroupIdx);
			}
    }
	return TempFunc_Validate;
}


