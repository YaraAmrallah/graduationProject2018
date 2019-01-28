Author: Mohamed Ahmed Gebril
Data: 10/7/2010
ADC Driver for TM4C123GH6PM.

This file is intended to help users of this device driver to easily use it. 

First user shall specify if he'll use ADC module0 or ADC Module1 or both  in ADC_Confg.h file in addition to
the sampling speed besides the sequencers priorities.

then user shall enter the NUMBEROFGROUPS he'll use in ADC_Confg.h.

after that user shall enter the groups in the order specified in the comments in ADC_Confg.c.

finally user should initialize the groups, modules and sequencers using ADC_Init() function in his main program.

User can use the ADC now as he configured Let's take for example the Processor event trigger for sequencer 3 module 0 sampling speed ADC_125K:

/****************** ADC_Confg.h**********************************************************************************/
so first I will update ADC_Confg.h Making CONFIGUREMODULE0 ADC_ModuleOn.

second specifing the sampling speed ADC_125k for example

third specifing the module sequencers priorities as a precaution making sequencer three the highest priority 0.


/**************** ADC_Confg.c ***********************************************************************************/
fourth in ADC_Confg.c I'll choose the module id (0), Sequencer Id (3) Input channel(Pin) Channel 0
(Don't forget to configure this pin as analog input using the GPIO driver) then the sample control bits.

/**************** main *****************************************************************************************/
Fifth in my main program I use ADC_Init() fun to initialize the adc..

Now I'm ready to start conversion and get my result using the following three functions

ADC_ProcessorInitiateSampling(Group Idx);

ADC_SampleAcknowledge(Group Idx);

ADC_SSnFIFO(Group Idx, Pointer to var to get the result);