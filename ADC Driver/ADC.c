/**
  * ADC definitions file (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */



#include "ADC.h"
#include "ADC_Confg.h"

#define GROUP0 0
// The coming macro gives the exact register address, and cast it then dereference it 
#define REGISTER(port_base,offset)   (*((volatile uint32_t*) (port_base+offset)))
#define CONC(x0,x1,x2,x3) CONC_HELPER(x0,x1,x2,x3)
#define CONC_HELPER(x0,x1,x2,x3) 0##x##x3##x2##x1##x0

static const uint32_t ADC_ModuleBase[]={ADC0BASEADDRESS, ADC1BASEADDRESS};

static const uint32_t ADC_SSMUXn[]={ADCSSMUX0, ADCSSMUX1, ADCSSMUX2, ADCSSMUX3};
static const uint32_t ADC_FIFOReg[]={ADCSSFIFO0, ADCSSFIFO1, ADCSSFIFO2, ADCSSFIFO3};


static const uint8_t ADC_SeqEnableBits[]={ASEN0, ASEN1, ASEN2, ASEN3};
static const uint8_t ADC_EMUXBITS[]={EM0, EM1, EM2, EM3};
static const uint8_t ADC_ISCBits[]={IN0, IN1, IN2, IN3};
static const uint8_t ADC_PSSIBits[]={SS0,SS1,SS2,SS3};
											
											
/** 
  * this function is used within this file only (static function)
  * This Function is used to check Sequencers priorities it has to be different and takes values from 0 to 3
  * returns 0 completed without error
  * return  completed with error
  * Prioritization look page 801
  * Multiple active sample sequencer with the same priority do not provide consistent results
  */
static ADC_FunctionReturn ADC_PrioritiesCheck(uint8_t Copy_Pri0, uint8_t Copy_Pri1, uint8_t Copy_pri2, uint8_t Copy_Pri3)
{
	
	ADC_FunctionReturn Function_Validationcheck = ADC_OK;
	int8_t ADC_PriorityCheck[]={0, 0, 0, 0}, ADC_LoopIdx, ADC_Err=ADC_OK;
	
	// Check that priority of the sample sequencers is different
	// First check right input 
	if( ((Copy_Pri0) < 0) || ((Copy_Pri0) > 3) || ((Copy_Pri1) < 0) || ((Copy_Pri1) > 3) || ((Copy_pri2) < 0) || ((Copy_pri2) > 3) || ((Copy_Pri3) < 0) || ((Copy_Pri3) > 3))
	{
		ADC_Err=ADC_NOK;
	}
	else
	{
		ADC_PriorityCheck[Copy_Pri0]++;
		ADC_PriorityCheck[Copy_Pri1]++;
		ADC_PriorityCheck[Copy_pri2]++;
		ADC_PriorityCheck[Copy_Pri3]++;
	
		for(ADC_LoopIdx = 0; ADC_LoopIdx < 4 && !(ADC_Err); ADC_LoopIdx++)
		{
			if(ADC_PriorityCheck[ADC_LoopIdx] > 1 || ADC_PriorityCheck[ADC_LoopIdx] < 0)
			{	
				ADC_Err = ADC_NOK;
			}
			else 
			{
				// Do Nothing
			}
		}
	}
	if(ADC_Err == ADC_NOK)
	{
			Function_Validationcheck = ADC_NOK;
	}
	else 
	{
		Function_Validationcheck = ADC_OK;
	}
	
	return Function_Validationcheck;
}



/**
  * This static function used to configure module 0 Run mode clock gating control
  * If user in ADC_Confg.h Choosed CONFIGUREMODULE0 as ADC_ModuleOn
  * Without calling this function in case the user choosed ADC_ModuleOff
  * The program can't access Module 0 and all its functions are disabled
  * This function is seen from this file only
  */
static ADC_FunctionReturn ADC_ConfigureModule0(void)
{
	uint32_t delay;
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn;
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	SYSCTL_RCGC0_R |= ADC_ModuleOn << ADC0_CLOCKGATINGBIT;
	delay = SYSCTL_RCGC0_R;
	// Specify Max sampling speed for module 0 bit 8, 9 in RCGC0 page 455.
	SYSCTL_RCGC0_R |= ADC0MAXSAMPLING << ADC0_MAXSAMPLESPEEDBITs;
	 // Check Priorities of sample sequencers Module 0.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		REGISTER(ADC0BASEADDRESS, ADCSSPRI) = CONC(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	}
	else 
	{
			Function_ValidationCheck=ADC_NOK;
	}
	
	return Function_ValidationCheck;
}
	
/**
  * This static function used to configure module 1 Run mode clock gating control
  * If user in ADC_Confg.h Chosed CONFIGUREMODULE1 as ADC_ModuleOn
  * Without calling this function in case the user chosed ADC_ModuleOff
  * The program can't access Module 1 and all its functions are disabled
  * This function is seen from this file only
  */
static ADC_FunctionReturn ADC_ConfigureModule1(void)
{
	uint32_t delay;
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn;
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	SYSCTL_RCGC0_R |= ADC_ModuleOn << ADC1_CLOCKGATINGBIT;
	 delay = SYSCTL_RCGC0_R;
	 // Specify Max sampling speed for module 0 bit 8, 9 in RCGC0 page 455.
	 SYSCTL_RCGC0_R |= ADC0MAXSAMPLING << ADC1_MAXSAMPLESPEEDBITs;
	 // Check Priorities of sample sequencers Module 0.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		REGISTER(ADC1BASEADDRESS, ADCSSPRI) = CONC(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	}
	else 
	{
		Function_ValidationCheck=ADC_NOK;
	}
	return Function_ValidationCheck;
}

/*****************************************************************************************************/
// SSnMUX function for choosing the inpput channel
/**
	* This Function used to choose the input channel for the coming sequence for sequencer three
	*/
// SS3
static ADC_FunctionReturn ADC_SS3MUX(uint8_t ADC_GroupIdx)
{
	// Function validation 
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
	// Sample Sequencer 3 channel register page 872
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX3) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << ADC_MUX0BITS);
	}
	//return the validation of the function
return Function_ValidationCheck;	
}

// SS2
static ADC_FunctionReturn ADC_SS2MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
		// Sample Sequencer 2 channel register page 864
		// MUX0
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << ADC_MUX0BITS);
		//MUX1
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << ADC_MUX1BITS);
		//MUX2
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << ADC_MUX2BITS);
		//MUX3
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << ADC_MUX3BITS);
	}
	return Function_ValidationCheck;
	
}

// SS1
static ADC_FunctionReturn ADC_SS1MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
	// Sample Sequencer 1 channel register page 864
	// MUX0
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << ADC_MUX0BITS);
	//MUX1
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << ADC_MUX1BITS);
	//MUX2
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << ADC_MUX2BITS);
	//MUX3
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << ADC_MUX3BITS);
	}
	return Function_ValidationCheck;
	
}


// SS0
static ADC_FunctionReturn ADC_SS0MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
	// Sample Sequencer 0 channel register page 848
	// MUX0
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << ADC_MUX0BITS);
	//MUX1
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << ADC_MUX1BITS);
	//MUX2
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << ADC_MUX2BITS);
	//MUX3
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << ADC_MUX3BITS);
	// MUX4
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample4_Channel << ADC_MUX4BITS);
	//MUX5
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample5_Channel << ADC_MUX5BITS);
	//MUX6
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample6_Channel << ADC_MUX6BITS);
	//MUX7
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( ADC_GroupConfg[ADC_GroupIdx].Sample7_Channel << ADC_MUX7BITS);
	
	}
	return Function_ValidationCheck;
	
}
/*****************************************************************************************************/

/*****************************************************************************************************/
// Setting Sample Sequencer Control Bits 
// SSCTL3
ADC_FunctionReturn ADC_SSCTL3(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else
	{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL3) |= 
						(ADC_GroupConfg[ADC_GroupIdx].sample0 << ADC_SAMPLE0BITS);
	}
	return Function_ValidationCheck;
}

ADC_FunctionReturn ADC_SSCTL2(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample0 << ADC_SAMPLE0BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample1 << ADC_SAMPLE1BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample2 << ADC_SAMPLE2BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample3 << ADC_SAMPLE3BITS);
	}
	return Function_ValidationCheck;
}

ADC_FunctionReturn ADC_SSCTL1(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample0 << ADC_SAMPLE0BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample1 << ADC_SAMPLE1BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample2 << ADC_SAMPLE2BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample3 << ADC_SAMPLE3BITS);
	}
	return Function_ValidationCheck;
}

ADC_FunctionReturn ADC_SSCTL0(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample0 << ADC_SAMPLE0BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample1 << ADC_SAMPLE1BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample2 << ADC_SAMPLE2BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample3 << ADC_SAMPLE3BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample4 << ADC_SAMPLE0BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample5 << ADC_SAMPLE1BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample6 << ADC_SAMPLE2BITS);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		(ADC_GroupConfg[ADC_GroupIdx].sample7 << ADC_SAMPLE3BITS);
	}
	return Function_ValidationCheck;
}

/**
  * This function is used in Initializing  ADC Groups and modules
  */
ADC_FunctionReturn ADC_Init(void)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn = ADC_OK;
	uint8_t ADC_LoopIdx, ADC_Err=0;
	
	// Check Module 0 Configure
	if (CONFIGUREMODULE0 == ADC_ModuleOn)
	{
		ADC_CalledFunctionReturn = ADC_ConfigureModule0();
		if( ADC_CalledFunctionReturn == ADC_NOK)
			ADC_Err=1;
		else
		{
			// Do NOthing
		}
	}
	else 
	{
		// Do Nothing
	}
	
	if(CONFIGUREMODULE1 == ADC_ModuleOn)
	{
		ADC_CalledFunctionReturn = ADC_ConfigureModule1();
		if( ADC_CalledFunctionReturn == ADC_NOK)
		{
			ADC_Err=1;
		}
		else 
		{
			// Do NOthing
		}
	}
	else 
	{
		// Do Nothing
	}
	
	if(ADC_Err == 1)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
		for( ADC_LoopIdx = GROUP0 ; ADC_LoopIdx < ADCNUMBEROFGROUPS ; ADC_LoopIdx++)
		{
			// First step disable the required sample seq in the required module 
			// as disabling the sequencer during initialization prevents erroneous execution if a trigger
			// event were to occur during the configuration process
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCACTSS) 
			&= ~(1 << ADC_GroupConfg[ADC_LoopIdx].ADC_SSId);
			 
			 
			 // Setting the triggering event
			 REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCEMUX) 
		  	|= (ADC_GroupConfg[ADC_LoopIdx].ADC_TriggeringEvent << ADC_EMUXBITS[ADC_GroupConfg[ADC_LoopIdx].ADC_SSId] );
			 
			 // Choosing input channel for the sequencer depending on the sequencer id (0, 1, 2, 3)
			 switch(ADC_GroupConfg[ADC_LoopIdx].ADC_SSId)
			 {
				case 3:
						ADC_CalledFunctionReturn = ADC_SS3MUX(ADC_LoopIdx);
						break;
				case 2:
						ADC_CalledFunctionReturn = ADC_SS2MUX(ADC_LoopIdx);
						break;
				case 1:
						ADC_CalledFunctionReturn = ADC_SS1MUX(ADC_LoopIdx);
						break;
				case 0:
						ADC_CalledFunctionReturn = ADC_SS0MUX(ADC_LoopIdx);
						break;
				default :
						Function_ValidationCheck = ADC_NOK;
						break;
			 }
			 if(ADC_CalledFunctionReturn == ADC_NOK)
			 {
					Function_ValidationCheck = ADC_NOK;
			 }
			 else 
			 {
				 // Do Nothing
			 }
			 
			 
			 // Setting sample control bit depending on the sequencer id(0, 1, 2, 3)
			 switch(ADC_GroupConfg[ADC_LoopIdx].ADC_SSId)
			 {
				case 3:
						ADC_CalledFunctionReturn = ADC_SSCTL3(ADC_LoopIdx);
						break;
				case 2:
						ADC_CalledFunctionReturn = ADC_SSCTL2(ADC_LoopIdx);
						break;
				case 1:
						ADC_CalledFunctionReturn = ADC_SSCTL1(ADC_LoopIdx);
						break;
				case 0:
						ADC_CalledFunctionReturn = ADC_SSCTL0(ADC_LoopIdx);
						break;
				default:
					Function_ValidationCheck= ADC_NOK;
					break;
			}
			if( ADC_CalledFunctionReturn == ADC_NOK)
			{
					Function_ValidationCheck= ADC_NOK;
			}
			else 
			{
				// Do Nothing
			}
			//enable SSid
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCACTSS) |= 
			(1 << ADC_GroupConfg[ADC_LoopIdx].ADC_SSId);
		}
		
	}
	return Function_ValidationCheck;
	
}

/**
  * This Function is used in getting trigger state for the running sequence 
  * return 0 completed successfully
	* return 1 terminated unsuccessful
	* takes the adc group idx, a pointer to a variable
  * return the trigger state in the variable pointed to it by the second parameter
  */
 
ADC_FunctionReturn ADC_GetTriggerState(uint8_t ADC_GroupIdx,uint8_t *ADC_TriggerState)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			*ADC_TriggerState =
			((REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCRIS) >> ADC_GroupConfg[ADC_GroupIdx].ADC_SSId) & 1);
		}
		return Function_ValidationCheck;
}


// Get sample from FIFO
ADC_FunctionReturn  ADC_SSFIFO(uint8_t ADC_GroupIdx,uint32_t* ADC_SampleRes)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			*ADC_SampleRes =
			(REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_FIFOReg[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
			&0xFFF);
		}
		return Function_ValidationCheck;
}


ADC_FunctionReturn ADC_SampleAcknowledge(uint8_t ADC_GroupIdx)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCISC)|=
			(1 << ADC_ISCBits[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId]);
		}
		return Function_ValidationCheck;
	}

	
	
ADC_FunctionReturn ADC_ProcessorInitiateSampling(uint8_t ADC_GroupIdx)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCPSSI)|=
			(1 << ADC_PSSIBits[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId]);
		}
		return Function_ValidationCheck;
}
	
