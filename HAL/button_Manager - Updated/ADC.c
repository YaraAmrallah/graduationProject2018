/**
  * ADC definitions file (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */



#include "ADC.h"

extern  AdcGroup Adc_GroupParameters[NUMBER_OF_ADC_GROUPS];

typedef volatile uint32_t* const Adc_RegAddType;



static void (*CbkFn_array[2][4])(void)={{0}}; // array to hold the call backs of each sequencer on its own 
static const uint32_t Adc_ModuleBaseAddressLut[]=
{
	ADC0BASEADDRESS,
	ADC1BASEADDRESS
};
static const uint32_t Adc_SsMuxOffset[]=
{
	ADCSSMUX0, 
	ADCSSMUX1, 
	ADCSSMUX2, 
	ADCSSMUX3};
static const uint32_t Adc_SsCtlOffset[]=
{
	ADCSSCTL0, 
	ADCSSCTL1, 
	ADCSSCTL2, 
	ADCSSCTL3};
static const uint32_t Adc_SsFIFO[]=
{
	ADCSSFIFO0, 
	ADCSSFIFO1, 
	ADCSSFIFO2, 
	ADCSSFIFO3};

#define ADC_REG_ADDRESS(MODULE,REG_OFFSET) 				((Adc_ModuleBaseAddressLut[MODULE]+REG_OFFSET))
#define ADC_SSMUX_REG_ADDRESS(MODULE,SEQUENCER)		((Adc_ModuleBaseAddressLut[MODULE]+Adc_SsMuxOffset[SEQUENCER]))
#define ADC_SSCTL_REG_ADDRESS(MODULE,SEQUENCER)		((Adc_ModuleBaseAddressLut[MODULE]+Adc_SsCtlOffset[SEQUENCER]))
#define ADC_SSFIFO_REG_ADDRESS(MODULE,SEQUENCER)	((Adc_ModuleBaseAddressLut[MODULE]+Adc_SsFIFO[SEQUENCER]))
 
//**********************************************************************************************************************************************
// 
// Register Map Defined for ADC Modules in TM4C123GH6PM
//
//***********************************************************************************************************************************************
#define ADCACTSS_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCACTSS)))
#define ADCRIS_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCRIS)))
#define ADCIM_REG(MODULE)					  	(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCIM)))
#define ADCISC_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCISC)))
#define ADCOSTAT_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCOSTAT)))
#define ADCEMUX_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCEMUX)))
#define ADCUSTAT_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCUSTAT)))
#define ADCTSSEL_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCTSSEL)))
#define ADCSSPRI_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSPRI)))
#define ADCSPC_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSPC)))
#define ADCPSSI_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCPSSI)))
#define ADCSAC_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSAC)))
#define ADCDCISC_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCDCISC)))
#define ADCCTL_REG(MODULE)						(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCCTL)))
#define ADCSSMUX0_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSMUX0)))
#define ADCSSCTL0_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSCTL0)))
#define ADCSSFIFO0_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFIFO0)))
#define ADCSSFIFO1_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFIFO1)))
#define ADCSSFIFO2_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFIFO2)))
#define ADCSSFIFO3_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFIFO3)))
#define ADCSSFSTAT0_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFSTAT0)))
#define ADCSSFSTAT1_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFSTAT1)))
#define ADCSSFSTAT2_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFSTAT2)))
#define ADCSSFSTAT3_REG(MODULE)				(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSFSTAT3)))
#define ADCSSMUX1_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSMUX1)))
#define ADCSSMUX2_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSMUX2)))
#define ADCSSCTL1_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSCTL1)))
#define ADCSSCTL2_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSCTL2)))
#define ADCSSOP1_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSOP1)))
#define ADCSSOP2_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSOP2)))
#define ADCSSMUX3_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSMUX3)))
#define ADCSSCTL3_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSCTL3)))
#define ADCSSOP3_REG(MODULE)					(*(Adc_RegAddType)(ADC_REG_ADDRESS(MODULE, ADCSSOP3)))
#define ADC_SSMUX(MODULE,SEQUENCER) 	(*(Adc_RegAddType)(ADC_SSMUX_REG_ADDRESS(MODULE,SEQUENCER)))
#define ADC_SSCTL(MODULE,SEQUENCER) 	(*(Adc_RegAddType)(ADC_SSCTL_REG_ADDRESS(MODULE,SEQUENCER)))
#define ADC_SSFIFO(MODULE,SEQUENCER) 	(*(Adc_RegAddType)(ADC_SSFIFO_REG_ADDRESS(MODULE,SEQUENCER)))

	
#define CONC(x0,x1,x2,x3) CONC_HELPER(x0,x1,x2,x3)
#define CONC_HELPER(x0,x1,x2,x3) 0##x##x3##x2##x1##x0
#define ADC0SS0PRINUMBER	14
#define ADC0SS1PRINUMBER	15
#define ADC0SS2PRINUMBER	16
#define ADC0SS3PRINUMBER	17
#define	ADC1SS0PRINUMBER	48
#define ADC1SS1PRINUMBER	49
#define ADC1SS2PRINUMBER	50	
#define ADC1SS3PRINUMBER	51

static Adc_ReturnType ADC_ConfigureModule1(void);
static Adc_ReturnType ADC_PrioritiesCheck(uint8_t Copy_Pri0, uint8_t Copy_Pri1, uint8_t Copy_pri2, uint8_t Copy_Pri3);
static Adc_ReturnType ADC_ConfigureModule0(void);
static void Adc_ConfigureSequencern(uint8_t Group);


// Array of pointers to hold the address of the result buffer given by the user for each group.
static Adc_ValueGroupType* DataBufferPtrAddr[8];
// Array to check that Result_Data_Buffer has been initialized to each group.
static uint8_t ResultBufferInit[8]={0};
static uint8_t Adc_ModuleInit=0;

static const uint32_t Adc_SSIRQ[][4]=
{
	{ADC0SS0PRINUMBER, ADC0SS1PRINUMBER, ADC0SS2PRINUMBER, ADC0SS3PRINUMBER},
	{ADC1SS0PRINUMBER, ADC1SS1PRINUMBER, ADC1SS2PRINUMBER, ADC1SS3PRINUMBER}
};


/* The driver used this array to pass the idx
   to the ISR handler */
static uint8_t GroupToHandler[2][4]={{0}};


void ADC0SS0_Handler(void);
void ADC0SS1_Handler(void);
void ADC0SS2_Handler(void);
void ADC0SS3_Handler(void);
void ADC1SS0_Handler(void);
void ADC1SS1_Handler(void);
void ADC1SS2_Handler(void);
void ADC1SS3_Handler(void);
	
/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
Adc_ReturnType ADC_Init(void)
{
	Adc_ReturnType Adc_Return = ADC_OK, ADC_CalledFunctionReturn = ADC_OK;
	uint8_t Adc_LoopIdx, ADC_Err=0;
	
	// Check Module 0 Configure
	if (CONFIGUREMODULE0 == ADC_ModuleOn)
	{
		ADC_CalledFunctionReturn = ADC_ConfigureModule0();
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
		Adc_Return = ADC_NOK;
	}
	else 
	{
		 AdcGroup *GrpPtr;
		for( Adc_LoopIdx = 0 ; Adc_LoopIdx < NUMBER_OF_ADC_GROUPS ; Adc_LoopIdx++)
		{
			
			(GrpPtr) = &Adc_GroupParameters[Adc_LoopIdx];
		  (GrpPtr->Adc_Status) = ADC_IDLE;
		  (GrpPtr->Adc_GroupNotificaton)= ADC_GROUP_NOTIFICATION_DISABLED;
			(GrpPtr->NumberOfSamplesFinished) = 0;
			// disable the sequencer of this group to adjust input channels and sample control
			ADCACTSS_REG(GrpPtr->AdcModule) &= ~(1 << (GrpPtr->AdcSequencer));
			// adjust the input channels and the control bits for that channel-group.
			Adc_ConfigureSequencern(Adc_LoopIdx);
			// Set the Conversion Event Trigger.
			ADCEMUX_REG(GrpPtr->AdcModule) &= ~(0xF) << (GrpPtr -> AdcSequencer*4);
			ADCEMUX_REG(GrpPtr->AdcModule) |= ((GrpPtr -> Adc_TriggerSource) << ((GrpPtr->AdcSequencer)*4));
 			// set the CbkFn in the ISR to this group call back
			CbkFn_array[GrpPtr->AdcModule][GrpPtr->AdcSequencer] = (GrpPtr->CbkFnPtr);
			GroupToHandler[GrpPtr->AdcModule][GrpPtr->AdcSequencer] = Adc_LoopIdx;	
			// enable the sample sequencer.
			ADCACTSS_REG(GrpPtr->AdcModule) |= (1 << (GrpPtr->AdcSequencer));
			// Put zero in the result_buffer_init to indicate result buffer not initialized.
			ResultBufferInit[Adc_LoopIdx] = 0;
		}
		Adc_ModuleInit = 1;
	}
	
	return Adc_Return;
	
}


//********************************************************************************************************
// Used in initializing the result buffer for the adc group.
Adc_ReturnType Adc_SetupResultBuffer(uint8_t Group, Adc_ValueGroupType* DataBufferPtr)
{
	Adc_ReturnType Adc_Return = ADC_OK;
	AdcGroup* GrpPtr = &Adc_GroupParameters[Group];
	if(Group >= NUMBER_OF_ADC_GROUPS)
	{
		Adc_Return = ADC_E_PARAM_GROUP;
	}
	else if(GrpPtr->Adc_Status != ADC_IDLE)
	{
		Adc_Return = ADC_E_BUSY;
	}
	else if(Adc_ModuleInit == 0)
	{
		Adc_Return = ADC_E_UNINIT;
	}
	else 
	{
		DataBufferPtrAddr[Group] = DataBufferPtr;
		ResultBufferInit[Group]=1;
	}
	return Adc_Return;
}


/*******************************************************************************************
  * This Function is used in getting trigger state for the running sequence 
  * return 0 completed successfully
	* return 1 terminated unsuccessful
	* takes the adc group idx, a pointer to a variable
  * return the trigger state in the variable pointed to it by the second parameter
  *******************************************************************************************/
Adc_StatusType Adc_GetGroupStatus(uint8_t Group)
{
		Adc_ReturnType Adc_Return = ADC_OK;
		AdcGroup *GrpPtr;
		Adc_StatusType Adc_Status;
	  uint8_t loop_idx = 0;

	  // This variable to hold the number of channels and the number of samples required per channel.
	  uint8_t Number_Of_Samples_Req_Group;
	  uint8_t Number_Of_Samples_Finished;
		if(Group >= NUMBER_OF_ADC_GROUPS)
		{
			Adc_Return = ADC_E_PARAM_GROUP;
		}
		else 
		{
			GrpPtr = &Adc_GroupParameters[Group];
			Number_Of_Samples_Req_Group = (GrpPtr->AdcChannelsNum) * (GrpPtr->AdcStreamingNumSamples);
			Number_Of_Samples_Finished =  (GrpPtr->NumberOfSamplesFinished);
			if(((ADCRIS_REG(GrpPtr->AdcModule)) & (1<<(GrpPtr->AdcSequencer))) != 0)
			{
				// Acknowledge that conversion by clearing the interrupt flag bit.in ADCISC Register.
			  ADCISC_REG(GrpPtr->AdcModule) |= GrpPtr->AdcSequencer;
			
				// Move the result in the group result buffer.
				for(loop_idx = (Number_Of_Samples_Finished*Number_Of_Samples_Req_Group);
				loop_idx < (Number_Of_Samples_Req_Group*(Number_Of_Samples_Finished+1)) ; loop_idx++)
				{
					*((DataBufferPtrAddr[Group])+loop_idx) =  ADC_SSFIFO((GrpPtr -> AdcModule), (GrpPtr -> AdcSequencer));
				}				
				// Increment the number of samples finished by one
				(GrpPtr->NumberOfSamplesFinished)++;
				// check if the stream has finished.
				if(GrpPtr->NumberOfSamplesFinished == GrpPtr->AdcStreamingNumSamples)
				{
					    // put the status to stream completed.
							// start the number of samples finised from zero again.
							GrpPtr->Adc_Status = ADC_STREAM_COMPLETED;
							GrpPtr->NumberOfSamplesFinished=0;
				}
				else 
				{
					GrpPtr->Adc_Status = ADC_BUSY;
				}
			}
			else
			{
				GrpPtr->Adc_Status = ADC_BUSY;
			}
		}
		Adc_Status = (GrpPtr -> Adc_Status);
		return Adc_Status;
}

// Read Group Results.
Adc_ReturnType  Adc_ReadGroup(uint8_t Group, Adc_ValueGroupType *DataBufferPtr)
{
		Adc_ReturnType Adc_Return = ADC_OK;
		uint8_t loop_idx = 0;
		 AdcGroup *GrpPtr;
		if(Group >= NUMBER_OF_ADC_GROUPS)
		{
			Adc_Return = ADC_E_PARAM_GROUP;
		}
		else 
		{
			GrpPtr =  &Adc_GroupParameters[Group];
			for(loop_idx = 0; loop_idx <  ((GrpPtr->AdcChannelsNum)*(GrpPtr->AdcStreamingNumSamples)) ; loop_idx++)
			{
				(*(DataBufferPtr+loop_idx)) = (*((DataBufferPtrAddr[Group])+loop_idx));
			}
			// Check the next state to this ADC_Group.
			// If the trigger source Continuous or Hardware Trigger then State will be busy.
			// else state is idle.
			if( (GrpPtr -> Adc_TriggerSource) == Adc_OneShot)
			{
				GrpPtr->Adc_Status = ADC_IDLE;
			}
			else
			{
				GrpPtr->Adc_Status = ADC_BUSY;
			}
		}
		return Adc_Return;
}

// ***************************************************************************************************************************
// This Function is used to Start group conversion assigning channels to sample sequencers and set sequencers control
// ***************************************************************************************************************************
Adc_ReturnType Adc_StartGroupConversion(uint8_t Group)
{
	Adc_ReturnType Adc_Return = ADC_OK;	// Function Validation Check.
	uint8_t Number_Of_Busy_Groups=0;
	AdcGroup *GrpPtr;
	uint8_t loop_idx = 0;
	
	// validation of input parameter. 
	if( Group >= NUMBER_OF_ADC_GROUPS)
	{
		Adc_Return = ADC_E_PARAM_GROUP;		// wrong parameter number.
	}
	
	else 
	{
		GrpPtr = &Adc_GroupParameters[Group];
		// validation of trigger source SW.
		if(((GrpPtr->Adc_TriggerSource) != Adc_OneShot) && ((GrpPtr->Adc_TriggerSource) != Adc_Continous))
		{
			Adc_Return = ADC_E_WRONG_TRIGG_SRC;
		}
		else 
		{
			/*// check that the required group module is not busy
			if(((ADCACTSS_REG(GrpPtr->AdcModule)) & BUSY_BITFIELD))
			{
				Adc_Return = ADC_E_BUSY;
			}
			// Not busy then adjust the group sequencer and start conversion*/
			// 8 groups for 8 different sequencers with different priorities 
			// need checking if higher priority interrupted lower one. 
		  //	else
			 //{
					GrpPtr->Adc_Status = ADC_BUSY;
					// Initiate the conversion.
					ADCPSSI_REG(GrpPtr -> AdcModule) |= (1 << GrpPtr->AdcSequencer);
			//}
		}
	}
	return Adc_Return;
}
 
/***************************************************************************************
 * This function used to enable interrupt for specific group
 * takes group id.
 ***************************************************************************************/
 void Adc_EnableGroupNotification(uint8_t Group)
{
	AdcGroup* GrpPtr;  
	
	GrpPtr = &Adc_GroupParameters[Group];
	
	// Group Notificaion is enabled.
	(GrpPtr->Adc_GroupNotificaton) = ADC_GROUP_NOTIFICATION_ENABLED;
	
	// Configure the interrupt by enabling and arming the interrupt.
	ADCIM_REG(GrpPtr -> AdcModule) |= (1 << GrpPtr -> AdcSequencer);
	SET_INT_PRI(Adc_SSIRQ[GrpPtr->AdcModule][GrpPtr->AdcSequencer],GrpPtr->Adc_GroupPriority);
	EN_INT(Adc_SSIRQ[GrpPtr->AdcModule][GrpPtr->AdcSequencer]);	
}

/********************************************************************************************************************************************************************
 **															Static Functions 
 *******************************************************************************************************************************************************************/							
/*********************************************************************************************************** 
  * this function is used within this file only (static function)
  * This Function is used to check Sequencers priorities it has to be different and takes values from 0 to 3
  * returns 0 completed without error
  * return  completed with error
  * Prioritization look page 801
  * Multiple active sample sequencer with the same priority do not provide consistent results
  **********************************************************************************************************/
static Adc_ReturnType ADC_PrioritiesCheck(uint8_t Copy_Pri0, uint8_t Copy_Pri1, uint8_t Copy_pri2, uint8_t Copy_Pri3)
{
	
	Adc_ReturnType Adc_Return = ADC_OK;
	int8_t ADC_PriorityCheck[]={0, 0, 0, 0}, Adc_LoopIdx, ADC_Err=ADC_OK;
	
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
	
		for(Adc_LoopIdx = 0; Adc_LoopIdx < 4 && !(ADC_Err); Adc_LoopIdx++)
		{
			if(ADC_PriorityCheck[Adc_LoopIdx] > 1 || ADC_PriorityCheck[Adc_LoopIdx] < 0)
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
			Adc_Return = ADC_NOK;
	}
	else 
	{
		Adc_Return = ADC_OK;
	}
	
	return Adc_Return;
}


/**
  * This static function used to configure module 0 Run mode clock gating control
  * If user in ADC_Confg.h Choosed CONFIGUREMODULE0 as ADC_ModuleOn
  * Without calling this function in case the user choosed ADC_ModuleOff
  * The program can't access Module 0 and all its functions are disabled
  * This function is seen from this file only
  */
static Adc_ReturnType ADC_ConfigureModule0(void)
{
	uint32_t delay;
	Adc_ReturnType Adc_Return = ADC_OK, ADC_CalledFunctionReturn;
	
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	RCGC0_REG |= ADC0;
	delay = RCGC0_REG;
	
	// Specify Max sampling speed for module 0 bit 8, 9 in RCGC0 page 455.
	RCGC0_REG |= (ADC0MAXSAMPLING << MAXADC0SPD);
	
	 // Check Priorities of sample sequencers Module 0.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		ADCSSPRI_REG(ADC_MODULE0) = CONC(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	}
	else 
	{
			Adc_Return=ADC_NOK;
	}
	
	return Adc_Return;
}
	
/**
  * This static function used to configure module 1 Run mode clock gating control
  * If user in ADC_Confg.h Chosed CONFIGUREMODULE1 as ADC_ModuleOn
  * Without calling this function in case the user chosed ADC_ModuleOff
  * The program can't access Module 1 and all its functions are disabled
  * This function is seen from this file only
  */
static Adc_ReturnType ADC_ConfigureModule1(void)
{
	uint32_t delay;
	Adc_ReturnType Adc_Return = ADC_OK, ADC_CalledFunctionReturn;
	
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	RCGC0_REG |= ADC1;
	delay = RCGC0_REG;
	
	 // Specify Max sampling speed for module 1 bit 10, 11 in RCGC0 page 455.
	RCGC0_REG |= ( ADC1MAXSAMPLING << MAXADC1SPD );
	
	 // Check Priorities of sample sequencers Module 1.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		ADCSSPRI_REG(ADC_MODULE1) = CONC(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	}
	else 
	{
		Adc_Return=ADC_NOK;
	}
	return Adc_Return;
}

static void Adc_ConfigureSequencern(uint8_t Group)
{
	const AdcGroup *GrpPtr;
	uint8_t loop_idx=0;
	GrpPtr = &Adc_GroupParameters[Group];
	
	// Adjust input channels of the corresponding sequencer. 
	for (loop_idx=0; loop_idx < (GrpPtr->AdcChannelsNum) ; loop_idx++)
	{
		//clear the corresponding four bits
		ADC_SSMUX(GrpPtr->AdcModule, GrpPtr->AdcSequencer) &= ~(0x0F << (loop_idx*4)); 
		ADC_SSMUX(GrpPtr->AdcModule, GrpPtr->AdcSequencer) |= ((GrpPtr->AdcGroupDefinition[loop_idx]) << (loop_idx*4));
	}
	
	
	// first clear all control bits for the range of this channel-group samples.
	for (loop_idx=0; loop_idx < (GrpPtr->AdcChannelsNum) ; loop_idx++)
	{
		ADC_SSCTL(GrpPtr->AdcModule, GrpPtr->AdcSequencer) &= ~(0x0F << (loop_idx*4));
	}
	
	// Set the IE bit and the end bit at the nibble of the last conversion. 
		ADC_SSCTL(GrpPtr->AdcModule, GrpPtr->AdcSequencer) |= (6 << (((GrpPtr->AdcChannelsNum)-1)*4));
	
}


//*************************************************************************************************
//                                ADC_Handlers
//*************************************************************************************************
/**
 * In the ISR function I increment the number of conversion done till now.
 * transfer the result in the group buffer.
 * Check whether to fire the call back, this decision depends on the number of samples...
 * configured by the user.
 */
void ADC0SS0_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[0][0]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[0][0]])+loop_idx) = ADC_SSFIFO(0,0);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}

void ADC0SS1_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[0][1]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[0][1]])+loop_idx) = ADC_SSFIFO(0,1);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}	
void ADC0SS2_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[0][2]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[0][2]])+loop_idx) = ADC_SSFIFO(0,2);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}	

void ADC0SS3_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[0][3]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;

	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[0][3]])+loop_idx) = ADC_SSFIFO(0,3);
	}
		Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}	
//*************************************************************************************************************
void ADC1SS0_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[1][0]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[1][0]])+loop_idx) = ADC_SSFIFO(1,0);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}

void ADC1SS1_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[1][1]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[1][1]])+loop_idx) = ADC_SSFIFO(1,1);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}	
void ADC1SS2_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[1][2]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;
	
	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[1][2]])+loop_idx) = ADC_SSFIFO(1,2);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}	

void ADC1SS3_Handler(void)
{
	
	AdcGroup* GrpPtr = &Adc_GroupParameters[GroupToHandler[1][3]];
	uint8_t Cnt = GrpPtr->NumberOfSamplesFinished,loop_idx=0;

	for(loop_idx = (Cnt*(GrpPtr->AdcStreamingNumSamples));(loop_idx<(GrpPtr->AdcChannelsNum*(Cnt+1)));loop_idx++)
	{
		*((DataBufferPtrAddr[GroupToHandler[1][3]])+loop_idx) = ADC_SSFIFO(1,3);
	}
	Cnt++;
	if(Cnt == (GrpPtr->AdcStreamingNumSamples))
	{
		 Cnt=0;
		(GrpPtr->CbkFnPtr());
	}
	GrpPtr->NumberOfSamplesFinished = Cnt;
}
