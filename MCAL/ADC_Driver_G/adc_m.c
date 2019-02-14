#include "adc_m.h"

// flags to know that the corrsponding groups has finished
static uint8_t group0_ConversionComplete = 0;

//**************************************************************************************************
// Buffers for Groups.
static uint16_t Group0_Buffer[5]={0}; 

//**************************************************************************************************
// Call Backs for Groups
void ADCGroup0_CallBack(void)
{
	group0_ConversionComplete = 1;
}

//***************************************************************************************************
// Cyclic function used in start the conversion of the Adc_Groups one at a time cyclic.
ADCManager_FunctionReturn ADCManager_StartConversion()
{
	static uint8_t ADCGroup_To_StartConversion = 0;
	ADCManager_FunctionReturn ADCManager_ValidationCheck = ADC_M_OK;
	Adc_FunctionValidation ADC_FunctionCheck = ADC_OK;
	ADC_FunctionCheck = Adc_StartGroupConversion(ADCGroup_To_StartConversion);
	if(ADC_FunctionCheck != ADC_OK)
	{
		ADCManager_ValidationCheck = ADC_M_E_START_CONVERSION;
	}
	else
	{
		 ADCGroup_To_StartConversion++;
		 if(ADCGroup_To_StartConversion >= NUMBER_OF_ADC_GROUPS)
		 {
			 ADCGroup_To_StartConversion=0;
		 }
	 }
	return ADCManager_ValidationCheck;
 }

ADCManager_FunctionReturn ADC_GetData(uint8_t ADC_GroupIdx,uint8_t ADC_DefinitionIdx,uint16_t* ADCData_Buffer)
{
	 Adc_FunctionValidation Function_Validation = ADC_OK;
	 ADCManager_FunctionReturn Manager_Function_Validation = ADC_M_OK;
	 Adc_StatusType Adc_Status;
	 uint8_t loop_idx = 0;
	 return 0;
}
	 
			
