#include "GPIO.h"
#include "GPIO_Confg.h"

/**
	* The arrengemnt of the objects in the structure
	*	uint8_t port_id;     		// From 0 to 5 ports(A, B, C, D, E, F)
	* uint8_t port_mask;   		// Direction output pins = 1, input pins = 0.
	* 	uint8_t Pins_InGroupDir;
	* uint8_t Alternate_FunSelect;
	* uint8_t Pins_DigitalMode;   // Pin digital = 1.
	* uint8_t Pins_AnalogMode;	// Set Pin Analog (disable the Analog isolation circuit) refer to page 653, 654 Mode control in data sheet.
	* uint8_t Pull_UpRes;  		// configure some pins with pull up resistors
	* uint8_t Pull_DownRes;		// configure some pins with pull down
	* uint8_t Slew_RateControl;	// Slew rate control (It's only available when using 8 mA drive strength option.
	* uint8_t Open_DrainControl;	// Open drain functionality
	* uint8_t Use_2mAmps; 		// configure 2mA drive for a specific pin
	* uint8_t Use_4mAmps;  		// configure 4mA drive for a specific pin
	* uint8_t Use_8mAmps;  		// configure 8mA drive for a specific pin
	*/
	
// For every Set of pins share the same job (callm them group)
const Group_confgs Port_ConParameters[NUMBEROFGROUPS]=
{

		{1,0x01,0x00,
		0x00,
		0x01,0x00,0x01,
		0x00,0x00,0x00,
		0x00,0x00,0x00},

		{1,0x02,0x02,
		0x00,
		0x02,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00},


		
};