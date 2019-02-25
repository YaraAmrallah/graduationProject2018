#include "Adc.h"
#define NUMBER_OF_HW_UNITS       (8u)
/* configuration for Adc_channel Group */
Adc_ChannelGroupConfigType Adc_ChannelGroupConfig[NUMBER_OF_CHANNEL_GROUPS]=
{
	{
		0,							/* Group_Id */
		ADC_ACCESS_MODE_SINGLE, 	/* Group_Access_Mode */
		ADC_CONV_MODE_CONTINUOUS,		/* Group_Conv_Mode	*/
		/* preprocessor remove unconfigured variable in preprossing time */
	  /* removed not configured by sw */
		#if(ADC_PRIORITY_IMPLEMENTATION==ADC_PRIORITY_HW_SW)
			0,
		#endif
		ADC_TRIGG_SRC_SW,			/* Group Event Trigger */
		#if(ADC_HW_TRIGGER_API==STD_ON)
			EXTERNAL,
			ADC_HW_TRIG_RISING_EDGE,
		#endif
		
		ADC_STREAM_BUFFER_CIRCULAR,	/* Software buffer mode */
		1,			            			
		#if(ADC_GRP_NOTIF_CAPABILITY==STD_ON)
			NULL_PTR,
		#endif
		ADC_GROUP_REPL_ABORT_RESTART, 
		{
			ADC_CHANNEL_4,
			ADC_CHANNEL_5,
			ADC_CHANNEL_3,
			ADC_CHANNEL_1
		}
	}
};
/* ***************************************************************** */

/* Configuration for hw uits 
	Two modules each with four sequencers 
	then eight Hw units 
	NUMBER_OF_HW_UNITS defined in Adc_Cfg.h
	*/
Adc_HwUnitConfigType Adc_HwUnitConfig[NUMBER_OF_HW_UNITS]=
{
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_0, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 8 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_1, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_2, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_3, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 1 */
	 1,			
	 0	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_0, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 8 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_1, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_2, /* Numberic Id of the Adc_Sequencer */
	 TRUE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 4,			
	 0 /* Pointer to the Adc_ChannelGroup*/
	},
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_3, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 1 */
	 1,			
	 255	/* Id of Adc_ChannelGroup*/
	},
};



/* ***************************************************************** */
/* The Overall configuration to be passed to Adc_init Function */
const Adc_ConfigType Adc_Config=
{
	TRUE, /* True: Configure and enable clock for module 1, False: Don't configure */
	ADC_500_K_SAMPLING_SPEED,	/* 500 kilo sample per second. */
	TRUE, /* True: Configure and enable clock for module 1, False: Don't configure */
	ADC_500_K_SAMPLING_SPEED,
	&Adc_HwUnitConfig[0]
};