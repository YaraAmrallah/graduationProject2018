/**
  * GPIO driver written by gebro (Mohamed Ahmed Gebril)
  * started 9/9/2018
  */

#include "GPIO.h"


#define SETPRI(PRIREG,priority,bits) 	PRIREG |= (priority & 1 ) << (8*bits+5); \
																			PRIREG |= ((priority >> 1) & 1 ) << (8*bits+6); \
																			PRIREG |= ((priority >> 2) & 1 ) << (8*bits+7)

// Macro like function take  a base address for a specific port and the offset of 
// The required function to return an address
#define REGISTER(port,offset) (*((volatile uint32_t*)(port+offset))) 
	
// Macros to read or write specific pins
#define GPIOWRITE(port,port_mask,data) (REGISTER(port,(port_mask << 2))) = data
#define GPIOREAD(port,port_mask) (REGISTER(port,((port_mask) << 2)))

// This Macro like function used in getting the IRQ number of the interrupt 
// To find out which priority register, priority bits and enable reg it uses
#define INTIRQ(Int_Num)	(Int_Num-16)


static const uint32_t ports[]={PORTA_BASEADDRESS,PORTB_BASEADDRESS,
PORTC_BASEADDRESS,PORTD_BASEADDRESS,PORTE_BASEADDRESS,PORTF_BASEADDRESS};

static const uint8_t Interrupt_NumberNVIC[]={PORTA_INTNUM,PORTB_INTNUM,
PORTC_INTNUM,PORTD_INTNUM,PORTE_INTNUM,PORTF_INTNUM};

static const uint32_t Interrupt_EnOffset[]={INTEN0,INTEN1,INTEN2,INTEN3,INTEN4};
static const uint32_t Interrupt_DisOffset[]={INTDIS0,INTDIS1,INTDIS2,INTDIS3,INTDIS4};
static const uint32_t Interrupt_PriOffset[]={INTPRI0,INTPRI1,INTPRI2,INTPRI3,INTPRI4,INTPRI5,INTPRI6,INTPRI7,INTPRI8};


GPIO_FunctionReturn GPIO_Init(void)
{
	
		GPIO_FunctionReturn FunctionValidationCheck=GPIO_OK;
	
		uint8_t Group_Idx=0,Err=0;
		uint32_t delay;
		for(Group_Idx=0 ; Group_Idx < NUMBEROFGROUPS && !(Err) ; Group_Idx++)
		{
			if(Port_ConParameters[Group_Idx].port_id >= 0 && Port_ConParameters[Group_Idx].port_id < PORTS){
			// Enable the clock for the i-th port 
			SYSCTL_RCGC2_R |= 1<<Port_ConParameters[Group_Idx].port_id;	
			//SYSCTL_RCGC2_R |=0x01;
			// Wait some cycles, cause there must be a delay of three cycles before any register
			// In that module is accessed after enabling its clock refer to page 462 for more details
			delay = SYSCTL_RCGC2_R;
				
							// Unlocking Locked pins needed for (PC0-3 ,PF0, PD7)
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOLOCK) = GPIO_LOCK_KEY;
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOCR) |= Port_ConParameters[Group_Idx].port_mask;
			
			//Set the direction of the pins of the required port
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIODIR) |= (Port_ConParameters[Group_Idx].port_mask & 
				Port_ConParameters[Group_Idx].Pins_InGroupDir);
			
			// Enable hardware control to let the required alternate function control the required pin by setting the corresponding bit to 1
			// You should then select the required peripheral to control the pin by setting the required byte in the PCTL register refer to page 653 (mode control)
			// For details using GPIO_AlternateFunc() function 
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOAFSEL) |= 
				(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Alternate_FunSelect) ;
			
			// Pad control selecting the drive strength for the pins 
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOR2R) |= 
				(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Use_2mAmps);
				
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOR4R) |= 
			(Port_ConParameters[Group_Idx].port_mask &		Port_ConParameters[Group_Idx].Use_4mAmps);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOR8R) |= 
			(Port_ConParameters[Group_Idx].port_mask &Port_ConParameters[Group_Idx].Use_8mAmps);
			
			// Pull up Res, Pull down, open drain and slew rate functions
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOODR) |= 
			(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Open_DrainControl);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOPUR) |= 
			(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Pull_UpRes);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOPDR) |= 
			(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Pull_DownRes);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOSLR) |= 
			(Port_ConParameters[Group_Idx].port_mask &  Port_ConParameters[Group_Idx].Slew_RateControl);
			
			// Digital enable, Analog functions enable
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIODEN) |= 
			(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Pins_DigitalMode);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOAMSEL) |= 
			(Port_ConParameters[Group_Idx].port_mask & Port_ConParameters[Group_Idx].Pins_AnalogMode);
			
			REGISTER(ports[Port_ConParameters[Group_Idx].port_id],GPIOPCTL) = 0x00;
				

			}
			
			else 
			{
				FunctionValidationCheck = GPIO_NOK;
				Err=1;
			}
			
		}
		return FunctionValidationCheck;
}




/**
  * This function is used in setting group value
  * Takes Three arguments PinNumber from 0 to 7,Port ID 0 to 5 and the pin value high or low
  * Return 0 Completed successfully
  * Return 1 Error not completed successfully usually wrong port id 
  */
GPIO_FunctionReturn GPIO_SetGroupValue(uint8_t Copy_GroupId, uint8_t Copy_GroupValue)
{
	uint8_t FunctionValidationCheck = GPIO_OK;
	
	if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS) 
	FunctionValidationCheck = GPIO_NOK;
	else 
	{
		// Write the data to specific pins
		GPIOWRITE(ports[Port_ConParameters[Copy_GroupId].port_id],Port_ConParameters[Copy_GroupId].port_mask,Copy_GroupValue);
		//REGISTER(PORTA_BASEADDRESS,GPIODATA) |= data;
	}
	return FunctionValidationCheck;
}


/**
   * This function is used in getting Group value
   * Takes two argument first: Group Id, a pointer to get the value in it
   * Return 0 Completed successfully
   * Return 1 Error not completed successfully usually wrong group id 
   */

GPIO_FunctionReturn GPIO_GetGroupValue(uint8_t Copy_GroupId, uint8_t *Ptr_GroupVal)
{
	// Variable to be returned from the function  /
	// To check the validaty
	uint8_t FunctionValidationCheck = GPIO_OK;
	
	// Check the id number between 0 and NUMBEROFGROUPS /
	// Specified by the user in the configuration file
	if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS) 
	{
			// Wrong Group Id
			FunctionValidationCheck = GPIO_NOK;
	}
	else 
	{
		// Getting the data existing on the desired port then anding with port mask 
		// To get the required bits
		*Ptr_GroupVal=
		GPIOREAD(ports[Port_ConParameters[Copy_GroupId].port_id],Port_ConParameters[Copy_GroupId].port_mask);
		
		FunctionValidationCheck=GPIO_OK;
	}
	
	return FunctionValidationCheck;
}



/**
	* This funtion selects the trigger event for a specific group
	* Takes two argument Group Id And The event type
	*	return 0 completed successfully
	* return 1 not completed
	*/
GPIO_FunctionReturn GPIO_SetTriggerEvent( uint8_t Copy_GroupId, IntEvent Copy_IntEvent)
{
	uint8_t Function_ValidationCheck= GPIO_OK;
	
	// Check Arguments validaty 
	if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS) Function_ValidationCheck=GPIO_NOK;
	
	else if (Copy_IntEvent < 0 || Copy_IntEvent > 4)
		Function_ValidationCheck=GPIO_NOK;
	
	// If ARguments is Ok
	else 
	{
			// Check if User wannt interrupt on levels
			if(Copy_IntEvent == GPIO_LevelHigh || Copy_IntEvent == GPIO_LevelLow)
			{
						// First Set Interrupt sense Register To detect levels
						REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIS) |= 
						Port_ConParameters[Copy_GroupId].port_mask;
						
						// Check If user want High or Low Level
						if(Copy_IntEvent == GPIO_LevelHigh)
						{
								// Set the corresponding bits on the interrupt event to detect high levels
								REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIEV) |= 
										Port_ConParameters[Copy_GroupId].port_mask;
						}
						else 
						{
								// Clear the corresponding bits on the Interrupt event register to detect low levels
								REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIEV) &= 
										~(Port_ConParameters[Copy_GroupId].port_mask);
						}
			}
			else 
			{
						// Clear the corresponding bits on interrupt sense to detect edges
						REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIS) &= 
						~(Port_ConParameters[Copy_GroupId].port_mask);
						
						// If user wanna Both edges Set the corresponding bitts on interrupt
						// both edges
						if(Copy_IntEvent == GPIO_BothEdges)
						{
								REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIBE) |= 
								Port_ConParameters[Copy_GroupId].port_mask;
						}
					
						else 
						{	
								// User wanna rising or falling edge 
								// First clear the corresponding bits on IBE
								REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIBE) &= 
								~(Port_ConParameters[Copy_GroupId].port_mask);
								if(Copy_IntEvent == GPIO_RisingEdge)
								{
										// Rising Edge
										REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIEV) |= 
										Port_ConParameters[Copy_GroupId].port_mask;
								}
								else 
								{		
										// Falling Edge
										REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIEV) &= 
										~(Port_ConParameters[Copy_GroupId].port_mask);
								}
						}
			}
	}
	return Function_ValidationCheck;
}


/**
	* Takes two argument Group Id And Arm or Disarm
	* To arm a device means to allow the hardware trigger to interrupt. Conversely,
	* to disarm a device means to shut off or disconnect the hardware trigger from the interrupts.
	*	return 0 completed successfully
	* return 1 not completed
	*/

GPIO_FunctionReturn GPIO_ArmInt(uint8_t Copy_GroupId,Int_EnumArm Copy_Arm,uint8_t Copy_Priority)
{
	uint8_t Function_ValidationCheck=GPIO_OK;
	if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS) 
		Function_ValidationCheck = GPIO_NOK;
	else if(Copy_Arm < 0 || Copy_Arm > 1)
		Function_ValidationCheck = GPIO_NOK;
	else if(Copy_Priority < 0 || Copy_Priority > 7)
		Function_ValidationCheck = GPIO_NOK;
	
	else 
	{
		if(Copy_Arm == GPIO_NotArm)
		{
			REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIM) &= 
										~(Port_ConParameters[Copy_GroupId].port_mask);
		}
		else 
		{
				uint8_t Port_IRQNum, Port_EnId, Port_EnBit, Port_PriId, Port_PriBits;
			
				REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOIM) |= 
										Port_ConParameters[Copy_GroupId].port_mask;
			
		
			
			Port_IRQNum=INTIRQ(Interrupt_NumberNVIC[Port_ConParameters[Copy_GroupId].port_id]);
			
			Port_EnId = (Port_IRQNum / 32);
			Port_EnBit = (Port_IRQNum % 32);
			
			REGISTER(INT_BASEADDRESS,Interrupt_EnOffset[Port_EnId])|=(1 << Port_EnBit);
			
			Port_PriId=Port_IRQNum / 4;
			Port_PriBits=Port_IRQNum % 4;
		
			SETPRI(REGISTER(INT_BASEADDRESS,Interrupt_PriOffset[Port_PriId]),Copy_Priority,Port_PriBits);
			
			
			
		}
	}
	return Function_ValidationCheck;
}

/**
	* This Function used to clear The corresponding bit in the RIS register
	*/
GPIO_FunctionReturn GPIO_InterruptAcknowledge(uint8_t Copy_GroupId)
{
		uint8_t FunctionValidationCheck=GPIO_OK;
		
	if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS) 
		FunctionValidationCheck=GPIO_NOK;
	else 
	{
					REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIOICR) |= 
										Port_ConParameters[Copy_GroupId].port_mask;
	}
	return FunctionValidationCheck;
}

/**
	* This Function is used to get 
	* the trigger state of the pin
	* Takes group ID and pointer to get value in
	*/

GPIO_FunctionReturn GPIO_GetTriggerState(uint8_t Copy_GroupId,uint32_t *Ptr_TriggerState)
{
		uint8_t Function_CheckValidation = GPIO_OK;
		if(Copy_GroupId < 0 || Copy_GroupId >= NUMBEROFGROUPS)
				Function_CheckValidation = GPIO_NOK;
		else {
	*Ptr_TriggerState = 
	(REGISTER(ports[Port_ConParameters[Copy_GroupId].port_id],GPIORIS) & Port_ConParameters[Copy_GroupId].port_mask);}
	
	return Function_CheckValidation;
}