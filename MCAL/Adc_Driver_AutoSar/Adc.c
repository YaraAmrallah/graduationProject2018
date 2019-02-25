#include "Adc.h"

/* Number of hw units not configurable hw limitations */
#define NUMBER_OF_HW_UNITS       (8u)

/* defines used in reporting development errors to the Development Error module */
#define ADC_MODULE_ID 					(uint16)123		/* List of Basic Software Modules */
#define ADC_INSTANCE_ID 				(uint8)0		/* on;y one adc driver */

/* API Ids from ADC_SWS document */
#define ADC_INIT_SID													0x00
#define ADC_SETUP_RESULT_BUFFER_SID 					0x0c
#define ADC_DEINIT_SID												0x01
#define ADC_START_GROUP_CONVERSION_SID				0x02
#define ADC_STOP_GROUP_CONVERSION_SID					0x03
#define ADC_READ_GROUP_SID										0x04
#define ADC_ENABLE_HARDWARE_TRIGGER_SID				0x05
#define ADC_DISABLE_HARDWARE_TRIGGER_SID			0x06
#define ADC_ENABLE_GROUP_NOTIFICATION_SID			0x07
#define ADC_DISABLE_GROUP_NOTIFICATION_SID 		0x08
#define ADC_GET_GROUP_STATUS_SID							0x09
#define ADC_GET_STREAM_LAST_POINTER_SID				0x0b
#define ADC_GET_VERSION_INFO									0x0a
#define ADC_SET_POWER_STATE										0x10
#define ADC_GET_CURRENT_POWER_STATE						0x11
#define ADC_GET_TARGET_POWER_STATE						0x12
#define ADC_PREPARE_POWER_STATE								0x13



/* Errors IDs P.48 Adc_SWS */
#define ADC_E_UNINIT						0x0A
#define ADC_E_BUSY							0x0B
#define ADC_E_IDLE							0x0C
#define ADC_E_ALREADY_INITIALIZED 			0x0D
#define ADC_E_PARAM_POINTER					0x0E
#define ADC_E_PARAM_GROUP					0x15
#define ADC_E_WRONG_CONV_MODE				0x16
#define ADC_E_WRONG_TRIGG_SRC				0x17
#define ADC_E_NOTIFY_CAPABILITY				0x18
#define ADC_E_BUFFER_UNINIT					0x19
#define ADC_E_NOT_DISENGAGED				0x1A
#define ADC_E_POWER_STATE_NOT_SUPPORTED		0x1B
#define ADC_E_TRANSITION_NOT_POSSIBLE		0x1C
#define ADC_E_PERIPHERAL_NOT_PREPARED		0x1D


/* base address for modules to facilitate accessing the registers by adding the required register offset */
static const uint32 Adc_ModuleBaseAddressLut[]=
{
	ADC0BASEADDRESS,
	ADC1BASEADDRESS
};






/* constants depends on hardware limitations for TM4C123GH6PM */
/* Maximum Number of Adc_Channel_Group TM4C123GH6PM */
#define ADC_MAXIMUM_NUMBER_OF_CHANNEL_GROUPS			(8u)

/* Maximum Number of Adc_Channels (Hardware limitations TM4C123GH6PM */
#define ADC_MAXIMUM_CHANNELS_PER_GROUP					(8u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 0 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER0			(8u)
					
/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 1 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER1   		(4u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 2 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER2			(4u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 3 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER3			(1u)

/* Post build configuration extern*/
extern Adc_ChannelGroupConfigType Adc_ChannelGroupConfig[NUMBER_OF_CHANNEL_GROUPS];
extern Adc_HwUnitConfigType Adc_HwUnitConfig[NUMBER_OF_HW_UNITS];

/* static arrays to hold information belongs to one adc channnel group */
/* Id of the hardware unit */
static uint8 Adc_ChannelGroupHwUnitId[8]={0};

/* Input channels number corresponding to a channel group */
static uint8 Adc_ChannelGroupNumInputChannel[8]={0};

/* Number of samples That finished conversions */
static uint8 Adc_ChannelGroupFinishedSamples[8]={0};

/* Array to store whether the notification has been enabled to th corresponding 
	Group or not */
static boolean Adc_ChannelGroupNotificEnabled[8];

/* array to check whether the result buffer has been initialized for the corresponding group or not */
static boolean Adc_resultBufferInit[8];

/* Array of pointers to hold the address of the result buffer given by the user for each group. */
static Adc_ValueGroupType* DataBufferPtrAddr[8];

/* Driver state: ADC_Driver_INITIALIZED Adc_init has been called suuccessfully
				 ADC_Driver_NOT_INIT  han't been called successfully*/
static Adc_DriverStateType Adc_DriverState = ADC_DRIVER_NOT_INIT; 


/* Current status of the conversion of the requested ADC Channel group. */
static Adc_StatusType Adc_GroupStatus[ADC_MAXIMUM_NUMBER_OF_CHANNEL_GROUPS]=
{
	/* ADC_IDLE */ 
	/* The conversion of the specified group has not been started. - 
	No result is available.*/
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE
};


/* offset for sample_sequencer_mux */
static const uint32 Adc_SsMuxOffset[]=
{
	ADCSSMUX0, 
	ADCSSMUX1, 
	ADCSSMUX2, 
	ADCSSMUX3};
	
/* offset for sample_sequencer_control */
static const uint32 Adc_SsCtlOffset[]=
{
	ADCSSCTL0, 
	ADCSSCTL1, 
	ADCSSCTL2, 
	ADCSSCTL3};

/* offset for sample_sequencer_fifo */
static const uint32 Adc_SsFIFO[]=
{
	ADCSSFIFO0, 
	ADCSSFIFO1, 
	ADCSSFIFO2, 
	ADCSSFIFO3};


/* define these arrays only if group notification configuration is enabled */
#if(ADC_GRP_NOTIF_CAPABILITY == STD_ON)
/* 2-D array to hold Interrupt number for each sequencer in each module 
	module ID is the row number where clumn number is the sequencer ID */
static const uint32 Adc_SSIRQ[][4]=
{
	{ADC0SS0PRINUMBER, ADC0SS1PRINUMBER, ADC0SS2PRINUMBER, ADC0SS3PRINUMBER},
	{ADC1SS0PRINUMBER, ADC1SS1PRINUMBER, ADC1SS2PRINUMBER, ADC1SS3PRINUMBER}
};

/* The driver used this array to pass the idx
   to the ISR handler */
static uint8 GroupToHandler[2][4]={{0}};

/* Sample sequencer ISRs */
void ADC0SS0_Handler(void);
void ADC0SS1_Handler(void);
void ADC0SS2_Handler(void);
void ADC0SS3_Handler(void);
void ADC1SS0_Handler(void);
void ADC1SS1_Handler(void);
void ADC1SS2_Handler(void);
void ADC1SS3_Handler(void);
	
#endif


/* Static Functions used within this file to configure hw units used only one time */
static void Adc_ConfigureSequencern(Adc_GroupType Group);

/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
	uint8_least Adc_GebroLoopIdx;
	uint32 delay;    /* used in waiting 3 cycles till clock is on */
	Adc_HwUnitConfigType* HwUnitPtr;
	Adc_ChannelGroupConfigType* ChannelGroupPtr;
	
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Adc_DriverState == ADC_DRIVER_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_INIT_SID, ADC_E_ALREADY_INITIALIZED)
		}
	#endif

	if ((ConfigPtr->Cfg_Module0) == TRUE)
	{
		/* enable module zero clock */
		RCGC0_REG |= (ADC_MODULE0_RCGC0_FIELD);
	    delay = RCGC0_REG;
		
		/* configure sampling speed */
		RCGC0_REG |= ((ConfigPtr->Adc_Module0SamplingSpeed) << MAXADC0SPD);
		
		/* Configure sequencer priorities values in register */
		ADCSSPRI_REG(ADC_MODULE_0)= CONC(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);

	}
	else 
	{
		// Do Nothing
	}
	/* module 1*/
	if ((ConfigPtr->Cfg_Module1) == TRUE)
	{
		/* enable module zero clock */
		RCGC0_REG |= (ADC_MODULE1_RCGC0_FIELD);
	    delay = RCGC0_REG;
		
		/* configure sampling speed */
		RCGC0_REG |= ((ConfigPtr->Adc_Module1SamplingSpeed) << MAXADC1SPD);
		
		/* Configure sequencer priorities values in register */
		ADCSSPRI_REG(ADC_MODULE_1) = CONC(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
		
	}
	else 
	{
		// Do Nothing
	}
	for( Adc_GebroLoopIdx = 0 ; Adc_GebroLoopIdx < NUMBER_OF_HW_UNITS ; Adc_GebroLoopIdx++)
	{
		HwUnitPtr = &Adc_HwUnitConfig[Adc_GebroLoopIdx];
		// if sequencer of this hw unit is configured to use
		if((HwUnitPtr->Adc_Sequencer) == TRUE)
		{
			
			
			//ChannelGroupPtr=&Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
			/* initialize the static global variables corresponding to the Adc_channel_group */
			Adc_ChannelGroupNumInputChannel[(HwUnitPtr->Adc_Group)] = (HwUnitPtr->Adc_NumberOfInputChannels);
			Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group]=0;
			Adc_ChannelGroupNotificEnabled[HwUnitPtr->Adc_Group]=FALSE;
			Adc_resultBufferInit[HwUnitPtr->Adc_Group]=FALSE;
			Adc_ChannelGroupHwUnitId[(HwUnitPtr->Adc_Group)] = Adc_GebroLoopIdx;
			/* configure and enable the sequencer */
			Adc_ConfigureSequencern(HwUnitPtr->Adc_Group);
		}
		}
		Adc_DriverState = ADC_DRIVER_INITIALIZED;
}

/* Initializes ADC driver with the group specific result buffer start address where the conversion results will be stored.
	The application has to ensure that the application buffer, where DataBufferPtr points to, 
	can hold all the conversion results of the specified group.
	The initialization with Adc_SetupResultBuffer is required after reset, before a group conversion can be started.*/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
	Std_ReturnType Std_Return=E_OK;
	/* Developement error reporting */
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_GROUP);
						}
						if(Adc_GroupStatus[Group] != ADC_IDLE)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_BUSY);
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_UNINIT);
						}
	#endif
	/* take the address of the result buffer and save it in global variable */
	DataBufferPtrAddr[Group] = DataBufferPtr;
						
	/* Result buffer has been setup successfully for the required group */
	Adc_resultBufferInit[Group]=TRUE;

	return Std_Return;
}

/* Deinit Function 
	Returns all ADC HW Units to a state comparable to their power on reset state. */
#if(ADC_DEINIT_API==STD_ON)
void Adc_DeInit(void)
{
	/* This Feature is not supported for now */
}
#endif

/* Starts the conversion of all channels of the requested ADC Channel group. */
#if(ADC_ENABLE_START_STOP_GROUP_API==STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group)
{
	Adc_ChannelGroupConfigType* ChannelGroupPtr;
	Adc_HwUnitType		Adc_HwUnitId;
	Adc_HwUnitConfigType* HwUnitPtr;
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_START_GROUP_CONVERSION_SID, ADC_E_PARAM_GROUP);
						}
						else 
						{/* Do nothing */}
	#endif
	ChannelGroupPtr = &Adc_ChannelGroupConfig[Group];
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if((ChannelGroupPtr->AdcGroupTriggSrc)==ADC_TRIGG_SRC_HW)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_START_GROUP_CONVERSION_SID, ADC_E_WRONG_TRIGG_SRC);
						}
						else 
						{/* Do nothing */}
	#endif
		Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
		HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
		/* change the sate of the group to busy */
		Adc_GroupStatus[Group]=ADC_BUSY;
		ADCPSSI_REG(HwUnitPtr->AdcModuleId) |= (1 << HwUnitPtr->AdcSequencerId);
}

/* Stops the conversion of the requested ADC Channel group. */
void Adc_StopGroupConversion(void)
{
	/* This Api is not supported for now */
}
#endif

/*Reads the group conversion result of the last completed conversion round of the requested group 
and stores the channel values starting at the DataBufferPtr address. 
The group channel values are stored in ascending channel number order*/
#if(ADC_READ_GROUP_API==STD_ON)
Std_ReturnType Adc_ReadGroup(
								Adc_GroupType Group, 
								Adc_ValueGroupType* DataBufferPtr
					)
{
	Std_ReturnType Std_Return = E_OK;
	uint8_least Adc_GebroLoopIdx=0;
	uint8 NumberOfChannels;
	uint8 NumberOfSamples;
	Adc_HwUnitType		Adc_HwUnitId;
	Adc_HwUnitConfigType* HwUnitPtr;
	Adc_ChannelGroupConfigType* ChannelGroupPtr;
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_READ_GROUP_SID, ADC_E_PARAM_GROUP);
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_READ_GROUP_SID, ADC_E_UNINIT);
						}
	#endif
	Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
	HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
	ChannelGroupPtr= &Adc_ChannelGroupConfig[Group];
	NumberOfChannels=HwUnitPtr->Adc_NumberOfInputChannels;
	NumberOfSamples=ChannelGroupPtr->AdcStreamingNumSamples;
	for(Adc_GebroLoopIdx = 0; Adc_GebroLoopIdx< ((GrpPtr->AdcChannelsNum)*(GrpPtr->AdcStreamingNumSamples)) ; loop_idx++)
	{
				(*(DataBufferPtr+Adc_GebroLoopIdx)) = (*((DataBufferPtrAddr[Group])+Adc_GebroLoopIdx));
	}
	if((ChannelGroupPtr->AdcGroupConversionMode) == ADC_CONV_MODE_ONESHOT)
	{
		Adc_GroupStatus[Group]=ADC_IDLE;
	}
	else 
	{
		Adc_GroupStatus[Group]=ADC_BUSY;
	}
	return Std_Return;
}
#endif

 /* Returns the conversion status of the requested ADC Channel group. */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
	Adc_StatusType Adc_Status;
	
	return Adc_Status;
}
/* reentrant function 
   Synchronus function used in init one sequencer */
static void Adc_ConfigureSequencern(Adc_GroupType Group)
{
	/* local variables defined and used inside the function */
	Adc_ChannelGroupConfigType *ChannelGroupPtr;
	Adc_HwUnitConfigType* HwUnitPtr;
	uint8_least Adc_GebroLoopIdx=0;
	/* Id of hw unit coressponding to the channel group */
	Adc_HwUnitType Adc_HwUnit = Adc_ChannelGroupHwUnitId[Group];
	Adc_SequencerType Adc_SequencerId;
	Adc_ModuleType Adc_ModuleId;
	Adc_ChannelType Adc_ChannelNumber;
	HwUnitPtr=&Adc_HwUnitConfig[Adc_HwUnit];
	ChannelGroupPtr = &Adc_ChannelGroupConfig[Group];
	Adc_SequencerId=(HwUnitPtr->AdcSequencerId);
	Adc_ModuleId=(HwUnitPtr->AdcModuleId);
	Adc_ChannelNumber=(HwUnitPtr->Adc_NumberOfInputChannels);
	
	
	/* disble the corresponding sequencer before configuration */
	ADCACTSS_REG(Adc_ModuleId) &= ~(1 << (Adc_SequencerId));
	
	// Adjust input channels of the corresponding sequencer. 
	for (Adc_GebroLoopIdx=0; Adc_GebroLoopIdx < Adc_ChannelNumber ; Adc_GebroLoopIdx++)
	{
		//clear the corresponding four bits
		ADC_SSMUX(Adc_ModuleId, Adc_SequencerId) &= ~(0x0F << (Adc_GebroLoopIdx*4)); 
		ADC_SSMUX(Adc_ModuleId, Adc_SequencerId) |= ((ChannelGroupPtr->AdcGroupDefinition[Adc_GebroLoopIdx]) << (Adc_GebroLoopIdx*4));
	}
	// first clear all control bits for the range of this channel-group samples.
	for (Adc_GebroLoopIdx=0; Adc_GebroLoopIdx < (Adc_ChannelNumber) ; Adc_GebroLoopIdx++)
	{
		ADC_SSCTL(Adc_ModuleId, Adc_SequencerId) &= ~(0x0F << (Adc_GebroLoopIdx*4));
	}
	
	// Set the IE bit and the end bit at the nibble of the last conversion. 
	ADC_SSCTL(Adc_ModuleId, Adc_SequencerId) |= (6 << (((Adc_ChannelNumber)-1)*4));
	
	/* configure the input event trigger */
	ADCEMUX_REG(Adc_ModuleId) &= ~(0xF) << (Adc_SequencerId*4);
	if((ChannelGroupPtr->AdcGroupTriggSrc) == ADC_TRIGG_SRC_SW)
	{
		ADCEMUX_REG(Adc_ModuleId) |= ((ChannelGroupPtr->AdcGroupConversionMode) << ((Adc_SequencerId)*4));
	}
	else 
	{
		#if(ADC_HW_TRIGGER_API==STD_ON)
			ADCEMUX_REG(Adc_ModuleId) |= ((ChannelGroupPtr -> AdcHwTriggerSource) << ((ChannelGroupPtr->AdcSequencer)*4));
		#endif
	}
	/* enable the sequencer */
	ADCACTSS_REG(Adc_ModuleId) |= (1 << (Adc_SequencerId));	
}

