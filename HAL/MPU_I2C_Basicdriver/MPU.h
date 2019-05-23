#ifndef MPU_H_
#define MPU_H_

#include <stdint.h>

/*
 **********************************************
 Type Definitions
 **********************************************
 */
typedef enum
{
    MPU_OK = 0, MPU_NOK, MPU_BUSY
} mpu6050_CheckType; // The return values for each function to be called
typedef enum
{
    IDLE_TX, START_TX, CHECK_ADD_SENT_TX, DATA_SENT, CHECK_DATA_SENT, ERROR_TX
} Mstr_Tx_State; // The Return Type of all the Driver's Functions
typedef enum
{
    IDLE_RX,
    START_RX,
    CHECK_ADD_SENT_RX,
    REPEAT_START,
    CHECK_DATA_RECEIVED,
    READ_DATA,
    ERROR_RX
} Mstr_Rx_State; // The Return Type of all the Driver's Functions

typedef enum
{
    /* Bit fields for Transaction Conditions*/
    I2C_MCS_ACK = 0x00000008,
    I2C_MCS_STOP = 0x00000004,
    I2C_MCS_START = 0x00000002,
    I2C_MCS_RUN = 0x00000001

} MPU_OPS;
/*
 **********************************************
 Functions Prototypes
 **********************************************
 */

void mpu6050_init(void);
mpu6050_CheckType MPU_writeBytes(uint8_t regAddr, uint8_t Size, uint8_t* Data);
mpu6050_CheckType MPU_readBytes(uint8_t regAddr, uint8_t Size, uint8_t *data);
mpu6050_CheckType MPU_readByte(uint8_t regAddr, uint8_t *data);
mpu6050_CheckType MPU_writeByte(uint8_t regAddr, uint8_t data);

mpu6050_CheckType MPU_InitBits(uint8_t regAddr, uint8_t bitStart,
                               uint8_t length, uint8_t data);
mpu6050_CheckType MPU_Who(uint8_t *data);
#endif
