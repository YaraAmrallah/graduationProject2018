/*
 *
 *	Authors: Hussien mohamed	
 *	Microcontroller: TIVAC
 *
 */

#include "I2C.h"

void I2C_TxDone(void);
void I2C_RxDone(void);
/*uint8_t                             I2C_GPIO_Structure_ID;

    Peripheral_ID                      I2C_Peripheral_ID;

    uint32_t                           I2C_FrequencyOf_SCL;

    PeripheralFrequency                I2C_PeripheralFrequency;

    Type                               I2C_TYPE;

    SpeedMode                          I2C_SpeedMode;

    Genera Call                        I2C_GeneralCall;

    I2C_CallBackFunctionType            I2C_TransmissionDoneCallBack;

    I2C_CallBackFunctionType            I2C_ReceptionDoneCallBack;*/

const I2C_ConfigType I2C_ConfigParam[I2C_PERIPHERAL_NUMBER]=
{
	{
		1,
		I2C_1,
        400000,
		
        PERIPHERAL_CLOCK_16MHz_Fast,
		
        MASTER,
		
        FAST_MODE,
		
		DISABLE_GENERAL_CALL,

		I2C_TxDone,
		I2C_RxDone
	}
};

