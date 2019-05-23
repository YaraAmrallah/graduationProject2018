#include "I2C_Cfg.h"
#include "I2C.h"

const I2C_CfgType I2C_ConfigParam [I2C_GROUPS_NUMBER]=
{
        I2C_1, // 0, 1, 2, or 3
        MASTER, // Master -> 0 - Slave  -> 1
        PERIPHERAL_CLOCK_16MHz_Standard,//
        STANDARD_MODE,
        100000
};


