/*
 *
 *	Authors: Hussien mohamed	
 *	Microcontroller: TIVAC
 *
 */

#include "I2C.h"

void I2C_TxDone(void);
void I2C_RxDone(void);

const I2C_ConfigType I2C_ConfigParam[I2C_PERIPHERAL_NUMBER]=
{
	{
		2,
		I2C_0,
        100000,    
		
		PERIPHERAL_CLOCK_16MHz,
		
        MASTER,
		
		STANDARD_MODE,
		
		DISABLE_GENERAL_CALL,

		I2C_TxDone,
		I2C_RxDone
	}
};

