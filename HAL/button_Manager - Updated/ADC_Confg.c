/**
  * ADC Configuration (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */

#include "ADC_Confg.h"


AdcGroup Adc_GroupParameters[NUMBER_OF_ADC_GROUPS]=
{
	{1,1,0, ADC_MODULE0, ADC_SEQ2 ,Adc_Continous,ADC_IDLE,ADC_GROUP_NOTIFICATION_DISABLED,ADC_GROUP_PRI_0,
	NULL_PTR, {ADC_CH2}
	/*{2,1,0, ADC_MODULE0, ADC_SEQ3 ,Adc_OneShot,ADC_IDLE,ADC_GROUP_NOTIFICATION_DISABLED,ADC_GROUP_PRI_0,
	NULL_PTR, {ADC_CH0,ADC_CH1}*/
}
/*{1, 1, 0, ADC_MODULE0, ADC_SEQ2 ,Adc_OneShot,ADC_IDLE,ADC_GROUP_NOTIFICATION_DISABLED,ADC_GROUP_PRI_1,
	NULL_PTR, {ADC_CH1}
}*/
};
// seq2 working ch1 working pri 1 working
