/**
  * ADC Base addresses, offsets and important bit in registers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */

#ifndef ADC_REG_H
#define ADC_REG_H


/**
  * ADC Module bases Page 815 TM4C123GH6PM
  */
#define ADC0BASEADDRESS 0x40038000
#define ADC1BASEADDRESS	0x40039000



/*********************************************************************************************************************/
/**
  * ADC offsets Page 815, 816
  */
#define ADCACTSS 		0x000						// ADC Active Sample Sequencer
#define ADCRIS   		0x004						// ADC Raw Interrupt Status
#define ADCIM 			0x008						// ADC Interrupt Mask
#define ADCISC			0x00C						// ADC Interrupt Status And Clear
#define ADCOSTAT		0x010						// ADC Overflow Status
#define ADCEMUX			0x014						// ADC Event Multiplexer Select
#define ADCUSTAT		0x018						// ADC Underflow status
#define ADCTSSEL		0x01C						// ADC Trigger Source Select
#define ADCSSPRI		0x020						// ADC Sample Sequencer Priority
#define ADCSPC			0x024						// ADC Sample Phase Control
#define ADCPSSI		0x028						// ADC Processor Sample Sequence Initiate
#define ADCSAC			0x030						// ADC Sample Averaging Control
#define ADCDCISC		0x034						// ADC digital Comparator Interrupt Status Clear
#define ADCCTL			0x038						// ADC Control
#define ADCSSMUX0		0x040						// ADC Sample Sequence Input Multiplexer Select 0
#define ADCSSCTL0		0x044						// ADC Sequence Control 0
#define ADCSSFIFO0		0x048						// ADC Sample Sequence Result FIFO 0
#define ADCSSFSTAT0		0x04C						// ADC Sample Sequence FIFO 0 status
#define ADCSSOP0		0x050						// ADC Sample Sequence 0 Operation 
#define ADCSSDC0		0x054						// ADC Sample sequence 0 Digital Comparator select
#define ADCSSMUX1 		0x060						// ADC Sample Sequence Input Multiplexer Select 1
#define ADCSSCTL1		0x064						// ADC Sample Sequence Control one 
#define ADCSSFIFO1		0x068						// ADC Sample Sequence Result FIFO 1
#define ADCSSFSTAT1		0x06C						// ADC Sample sequence 1 statue
#define ADCSSOP1		0x070						// ADC Sample Sequence Operation 1
#define ADCSSDC1		0x074						// ADC Sample Sequence 1 digital Comparator
#define ADCSSMUX2		0x080						// ADC Sample Sequence input multiplexer select 2
#define ADCSSCTL2 		0x084						// ADC Sample Sequence Control 2
#define ADCSSFIFO2		0x088						// ADC Sample Sequence result FIFO 2	
#define ADCSSFSTAT2		0x08C						// ADC Sample Sequence FIFO 2 Status
#define ADCSSOP2		0x090						// ADC Sample Sequence 2 Operation
#define ADCSSDC2 		0x094						// ADC Sample Sequence 2 Digital Comparator select 
#define ADCSSMUX3		0x0A0						// ADC Sample Sequence input multiplexer select 3
#define ADCSSCTL3 		0x0A4						// ADC Sample Sequence Control 3
#define ADCSSFIFO3		0x0A8						// ADC Sample Sequence result FIFO 3	
#define ADCSSFSTAT3		0x0AC						// ADC Sample Sequence FIFO 3 Status
#define ADCSSOP3		0x0B0						// ADC Sample Sequence 3 Operation
#define ADCSSDC3		0x0B4						// ADC Sample Sequence 3 Digital Comparator select 
#define ADCDCRIC		0xD00						// ADC Digital Comparator Reset Initial Condition
#define ADCDCCTL0		0xE00						// ADC Digital Comparator Control 0
#define ADCDCCTL1		0xE04						// ADC Digital Comparator Control 1
#define ADCDCCTL2		0xE08						// ADC Digital Comparator Control 2
#define ADCDCCTL3		0xE0C						// ADC Digital Comparator control 3
#define ADCDCCTL4		0xE10						// ADC Digital Comparator Control 4
#define ADCDCCTL5		0xE14						// ADC Digital Comparator Control 5
#define ADCDCCTL6		0xE18						// ADC Digital Comparator Control 6
#define ADCDCCTL7		0xE1C						// ADC Digital Comparator Control 7
#define ADCDCCMP0		0xE40						// ADC Digital Comparator Range 0
#define ADCDCCMP1		0xE44						// ADC Digital Comparator Range 1
#define ADCDCCMP2		0xE48						// ADC Digital Comparator Range 2
#define ADCDCCMP3		0xE4C						// ADC Digital Comparator Range 3
#define ADCDCCMP4		0xE50						// ADC Digital Comparator Range 4
#define ADCDCCMP5		0xE54						// ADC Digital Comparator Range 5
#define ADCDCCMP6		0xE58						// ADC Digital Comparator Range 6
#define ADCDCCMP7		0xE5C						// ADC Digital Comparator Range 7
#define ADCPP			0xFC0						// ADC Peripheral Properties
#define ADCPC			0xFC4						// ADC Peripheral Configuration
#define ADCCC			0xFC8						// ADC Clock Configuration
/***************************************************************************************************************************************/


/**********************************************ADC MOdule Run Mode Clocl************************************************************/
/* Clock gating control */
#define SYSCTL_RCGC0_R          (*((volatile uint32_t *)0x400FE100)) // Run mode clock gating control 
#define ADC0_CLOCKGATINGBIT 16
#define ADC1_CLOCKGATINGBIT 17
#define ADC0_MAXSAMPLESPEEDBITs 8		// (8, 9)
#define ADC1_MAXSAMPLESPEEDBITs 10		// (10, 11)
/**********************************************************************************************************/

/******************************************** Sample Sequencer priority Bits***************************************************************/
#define ADC_SAMPLESEQ0PRIBITs 0
#define ADC_SAMPLESEQ1PRIBITs 4
#define ADC_SAMPLESEQ2PRIBITs 8
#define ADC_SAMPLESEQ3PRIBITs 12
/*****************************************************************************************************************************************/

/******************************************* Sample  Control Bits ************************************************************************/
#define ADC_SAMPLE0BITS 0
#define ADC_SAMPLE1BITS 4
#define ADC_SAMPLE2BITS 8
#define ADC_SAMPLE3BITS 12
#define ADC_SAMPLE4BITS 16
#define ADC_SAMPLE5BITS 20
#define ADC_SAMPLE6BITS 24
#define ADC_SAMPLE7BITS 28
/*************************************************************** ADCACTSS ***********************************************************/
#define ASEN0	0									// Sequencer0 enable bit 0 
#define ASEN1	1									// Sequencer1 enable bit 1
#define ASEN2	2									// Sequencer2 enable bit 2
#define ASEN3 3									// Sequencer3 enable bit 3
/************************************************************************************************************************************/


/************************************************* Sample in channel mux bits *****************************/
#define ADC_MUX0BITS 0
#define ADC_MUX1BITS 4
#define ADC_MUX2BITS 8
#define ADC_MUX3BITS 12
#define ADC_MUX4BITS 16
#define ADC_MUX5BITS 20
#define ADC_MUX6BITS 24
#define ADC_MUX7BITS 28
/***********************************************************************************************************/


/********************************* Event Multiplexer ******************************************************/
#define EM0 0
#define EM1	4
#define EM2 8
#define EM3 12
/***********************************************************************************************************/

/************************************ ADCISC *************************************************************/
#define IN0 0
#define IN1 1
#define IN2 2
#define IN3 3
/*********************************************************************************************************/

/********************************* ADCPSSI **************************************************************/
#define SS0 0
#define SS1 1
#define SS2 2
#define SS3 3
/*********************************************************************************************************/
#endif
