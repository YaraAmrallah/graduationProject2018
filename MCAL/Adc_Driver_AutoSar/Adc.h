/* Adc_Driver,
	 Author Mohamed Ahmed Gebril 
   Faculty of engineering Alexandria university */
/* Most of the comments are description from the autosar document */
#ifndef ADC_H
#define ADC_H

#include "../Std_Types/Std_Types.h"
#include "M4MemMap.h"  /* clocks and prioity */
#include "Adc_TypeDefs.h"
#include "Adc_Reg.h"
#include "Adc_Cfg.h"



 
extern const Adc_ConfigType Adc_Config;

/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/* Initializes ADC driver with the group specific result buffer start address where the conversion results will be stored.
	The application has to ensure that the application buffer, where DataBufferPtr points to, 
	can hold all the conversion results of the specified group.
	The initialization with Adc_SetupResultBuffer is required after reset, before a group conversion can be started.*/
Std_ReturnType Adc_SetupResultBuffer(
							 Adc_GroupType Group, 
							 Adc_ValueGroupType* DataBufferPtr
							 );

/* Deinit Function 
	Returns all ADC HW Units to a state comparable to their power on reset state. 
	This Api is not supported for now.*/
void Adc_DeInit(void);

/* Starts the conversion of all channels of the requested ADC Channel group. */
void Adc_StartGroupConversion(Adc_GroupType Group);


/* Stops the conversion of the requested ADC Channel group.
	This Api is not supported for now. */
void Adc_StopGroupConversion(void);

/*Reads the group conversion result of the last completed conversion round of the requested group 
and stores the channel values starting at the DataBufferPtr address. 
The group channel values are stored in ascending channel number order*/
Std_ReturnType Adc_ReadGroup(
								Adc_GroupType Group,
								Adc_ValueGroupType* DataBufferPtr
							);

/* Returns the conversion status of the requested ADC Channel group. */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);



#endif