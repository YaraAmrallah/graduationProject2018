/**
  * GPIO Driver for tm4c123GH6PM Micro-controller
  * written by Mohamed Ahmed Gebril
  * started 7/9/2018
  * Gebro
  */
#ifndef GPIO_H
#define GPIO_H 
#include <stdint.h>
#include "GPIO_Confg.h"
#include "./libraries/macros.h"

/* *************************************************************************************************************************************************** */ // Refer to page (657, 658) to check this offsets (GPIO register map)
#define GPIODIR 		0x400  			 // GPIO direction register offset
#define GPIODEN			0x51C   		 // GPIO digital function enable offset
#define GPIOPCTL		0x52C   		 // GPIO Port control
#define GPIOAMSEL		0x528   	   // Analog mode select for pins
#define GPIOAFSEL  	0x420        // Alternative function select offset 
#define GPIOLOCK   	0x520        // locking register offset as some pins need unlocking refer to page 654 in TM4C123 data sheet, for more info
#define GPIODATA  	0x3FC   		 // Data register offset
#define GPIOCR 			0x524   		 // commit change register offset
#define GPIOR2R			0x500			 	 // 2mA drive strength register offset
#define GPIOR4R			0x504 		   // 4mA drive strength register offset
#define GPIOR8R			0x508  			 // 8mA drive strength register offset
#define GPIOODR			0x50C				 // Open drain functionality offset
#define GPIOPUR 		0x510			  	// Pull up resistor functionality offset 
#define GPIOPDR			0x514				 // Pull down resistor functionality offset
#define GPIOSLR			0x518 	 	   // slew rate functionality offset
#define GPIOIS 			0x404 		 	 // GPIO interrupt sense 
#define GPIOIBE 		0x408					// GPIO INterrupt both egde
#define GPIOIEV			0x40C					// GPIO interrept event
#define GPIOIM			0x410					// GPIO interrupt mask
#define GPIORIS			0x414					// GPIO raw interrupt status
#define GPIOMIS			0x418					// GPIO masked interrupt status
#define GPIOICR			0x41C					//	GPIO interrupt clear
/* **************************************************************************************************************************************************** */
 
/* ********************************************************* */
#define INTEN0    	    0x100
#define INTEN1			0x104
#define INTEN2			0x108
#define INTEN3			0x10C
#define INTEN4			0x110
#define INTDIS0			0x180
#define INTDIS1			0x184
#define INTDIS2			0x188
#define INTDIS3			0x18C
#define INTDIS4			0x190
#define INTPRI0			0x400
#define INTPRI1			0x404
#define INTPRI2			0x408
#define INTPRI3			0x40C
#define INTPRI4			0x410
#define INTPRI5			0x414
#define INTPRI6			0x418
#define INTPRI7			0x41C
#define INTPRI8			0x420
/* ********************************************************** */
 
#define GPIO_LOCK_KEY 0x4C4F434B // The lock key that is used to unlock commit change register

//for locked pin(PC0-3, PF0, PD7)

#define PORTS 6

// PORTS bases
#define PORTA_BASEADDRESS 0x40004000
#define PORTB_BASEADDRESS 0x40005000
#define PORTC_BASEADDRESS 0x40006000
#define PORTD_BASEADDRESS 0x40007000
#define PORTE_BASEADDRESS 0x40024000 
#define PORTF_BASEADDRESS 0x40025000


// Interrupt Enable, Disable and Priority Bases 
#define INT_BASEADDRESS 0xE000E000

// interrupt number in the NVIC 
#define PORTA_INTNUM		16
#define PORTB_INTNUM		17
#define PORTC_INTNUM		18
#define PORTD_INTNUM    	19
#define PORTE_INTNUM		20
#define PORTF_INTNUM		46

#define SYSCTL_RCGC2_R         	 (*((volatile uint32_t *)0x400FE108))


	




typedef struct{
	uint8_t port_id;     		// From 0 to 5 ports(A, B, C, D, E, F)
	
	uint8_t port_mask;   		// Pins specified to that group
	
	
	// Direction Control 1 Output, 0 Input
	uint8_t Pins_InGroupDir;
	
	//Mode Control
	uint8_t Alternate_FunSelect;
	
	// To configure digital/analog pins
	uint8_t Pins_DigitalMode;   // Pin digital = 1.
	uint8_t Pins_AnalogMode;	// Set Pin Analog (disable the Analog isolation circuit) refer to page 653, 654 Mode control in data sheet.
	
	// Pad control
	uint8_t Pull_UpRes;  		// configure some pins with pull up resistors
	uint8_t Pull_DownRes;		// configure some pins with pull down
	uint8_t Slew_RateControl;	// Slew rate control (It's only available when using 8 mA drive strength option.
	uint8_t Open_DrainControl;	// Open drain functionality
	uint8_t Use_2mAmps; 		// configure 2mA drive for a specific pin
	uint8_t Use_4mAmps;  		// configure 4mA drive for a specific pin
	uint8_t Use_8mAmps;  		// configure 8mA drive for a specific pin
	
}Group_confgs;
 
typedef enum  { GPIO_OK=0, GPIO_NOK }GPIO_FunctionReturn;

// Interuptts Properties
typedef enum { GPIO_FallingEdge=0,GPIO_RisingEdge,GPIO_BothEdges,GPIO_LevelHigh,GPIO_LevelLow} IntEvent;

// Note To arm a device means to allow the hardware trigger to interrupt.
typedef enum {GPIO_Arm=0, GPIO_NotArm}Int_EnumArm;

extern  const Group_confgs Port_ConParameters[NUMBEROFGROUPS];


 
/**
  * This function is used in initializing GPIO ports which is configured in GPIO_Cfg.h
  * Takes no argument
  * Return 0 Completed successfully
  * Return 1 Error not completed successfully usually wrong port id 
  */
GPIO_FunctionReturn GPIO_Init(void);
 
 
/**
  * This function is used in setting Group value
  * Takes two arguments Group Id from 0 to Number f groups ,Value to put in the required group
  * Return 0 Completed successfully
  * Return 1 Error not completed successfully usually wrong GroupId
  */
GPIO_FunctionReturn GPIO_SetGroupValue(uint8_t Copy_GroupId, uint8_t Copy_GroupValue);
 
 
 /**
   * This function is used in getting Group value
   * Takes two argument first: Group Id, a pointer to get the value in it
   * Return 0 Completed successfully
   * Return 1 Error not completed successfully usually wrong group id 
   */
GPIO_FunctionReturn GPIO_GetGroupValue(uint8_t Copy_GroupId, uint8_t *Ptr_GroupVal);

/**
	* This funtion selects the trigger event for a specific group
	* Takes two argument Group Id And The event type
	*	return 1 completed successfully
	* return 0 not completed
	*/
GPIO_FunctionReturn GPIO_SetTriggerEvent( uint8_t Copy_GroupId, IntEvent Copy_IntEvent);

/**
	* This funtion selects the trigger event for a specific group
	* Takes two argument Group Id And Arm or Disarm
	* To arm a device means to allow the hardware trigger to interrupt.
	* to disarm a device means to shut off or disconnect the hardware trigger from the interrupts.
	*	return 1 completed successfully
	* return 0 not completed
	*/
GPIO_FunctionReturn GPIO_ArmInt(uint8_t Copy_GroupId, Int_EnumArm Copy_Arm,uint8_t Copy_Priority);

/**
	* This Function used to clear The corresponding bit in the RIS register
	*/
GPIO_FunctionReturn GPIO_InterruptAcknowledge(uint8_t Copy_GroupId);


GPIO_FunctionReturn GPIO_GetTriggerState(uint8_t Copy_GroupId,uint32_t *Ptr_TiggerState);
 #endif