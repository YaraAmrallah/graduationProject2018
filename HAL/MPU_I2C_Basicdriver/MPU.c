#include "MPU.h"
#include "Reg.h"
#include "I2C.h"
#include <stdint.h>
#include "I2C_Cfg.h"

#define mpu6050_Slave_Address   MPU6050_RA_SIGNAL_PATH_RESET
#define MPU6050_GYRO_FS         MPU6050_RA_YG_OFFS_TC
#define MPU6050_ACCEL_FS        MPU6050_RA_YG_OFFS_TC
#define MPU_GROUP_ID  0

void mpu6050_init(void)
{
    MPU_InitBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT,
    MPU6050_PWR1_CLKSEL_LENGTH,
                 MPU6050_CLOCK_PLL_XGYRO);
    MPU_InitBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1, 0x0);

    while (MPU_writeByte(MPU6050_RA_CONFIG, 0x43) != MPU_OK)
        ;
    //set sampe rate
    while (MPU_writeByte(MPU6050_RA_SMPLRT_DIV, 0x9) != MPU_OK)
        ;

    //set gyro range
    MPU_InitBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT,
    MPU6050_GCONFIG_FS_SEL_LENGTH,
                 MPU6050_GYRO_FS);

    //set accel range
    MPU_InitBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT,
    MPU6050_ACONFIG_AFS_SEL_LENGTH,
                 MPU6050_ACCEL_FS);

}

mpu6050_CheckType MPU_writeByte(uint8_t regAddr, uint8_t data)
{

    static Mstr_Tx_State State = IDLE_TX;
    static I2C_CheckType Check;
    static mpu6050_CheckType RetVar = MPU_NOK;

    switch (State)
    {
    case IDLE_TX:

        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Transmit, MPU_GROUP_ID);
        I2C_SendData(&regAddr, MPU_GROUP_ID);
        RetVar = MPU_BUSY;
        State = START_TX;
        break;
    case START_TX:
        Check = I2C_CheckBusStat(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = START_TX;

        }
        else
        {
            I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN),
            MPU_GROUP_ID);
            State = CHECK_ADD_SENT_TX;
        }
        break;
    case CHECK_ADD_SENT_TX:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_ADD_SENT_TX;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_TX;
            }
            else
            {
                State = DATA_SENT;
            }
        }
        break;
    case DATA_SENT:
        I2C_SendData(&data, MPU_GROUP_ID);
        I2C_SetOperation((I2C_MCS_RUN | I2C_MCS_STOP), MPU_GROUP_ID);
        State = CHECK_DATA_SENT;
        break;
    case CHECK_DATA_SENT:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_DATA_SENT;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_TX;
            }
            else
            {
                RetVar = MPU_OK;
                State = IDLE_TX;
            }
        }
        break;
    case ERROR_TX:
        // Error Handling
        RetVar = MPU_NOK;
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        State = IDLE_TX;
        break;

    default:

        break;
    }
    return RetVar;

}

mpu6050_CheckType MPU_writeBytes(uint8_t regAddr, uint8_t Size, uint8_t* Data)
{
    static Mstr_Tx_State State = IDLE_TX;
    static I2C_CheckType Check;
    static uint16_t counter = 0;
    static mpu6050_CheckType RetVar = MPU_NOK;

    switch (State)
    {
    case IDLE_TX:
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Transmit, MPU_GROUP_ID);
        I2C_SendData(&regAddr, MPU_GROUP_ID);
        RetVar = MPU_BUSY;
        State = START_TX;
        break;
    case START_TX:
        Check = I2C_CheckBusStat(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = START_TX;
        }
        else
        {
            I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN),
            MPU_GROUP_ID);
            State = CHECK_ADD_SENT_TX;
            //RetVar = MPU_BUSY;
        }
        break;
    case CHECK_ADD_SENT_TX:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_ADD_SENT_TX;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_TX;
            }
            else
            {
                State = DATA_SENT;
            }
        }
        break;
    case DATA_SENT:
        if (counter < (Size - 1))
        {
            I2C_SendData(&Data[counter], MPU_GROUP_ID);
            I2C_SetOperation((I2C_MCS_RUN), MPU_GROUP_ID);
            State = CHECK_DATA_SENT;
        }
        else
        {
            I2C_SendData(&Data[counter], MPU_GROUP_ID);
            I2C_SetOperation((I2C_MCS_RUN | I2C_MCS_STOP),
            MPU_GROUP_ID);
            State = CHECK_DATA_SENT;
        }
        break;
    case CHECK_DATA_SENT:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_DATA_SENT;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_TX;
            }
            else if (counter < (Size - 1))
            {
                counter++;
                State = DATA_SENT;
            }
            else
            {
                RetVar = MPU_OK;
                counter = 0;
                State = IDLE_TX;

            }
        }
        break;
    case ERROR_TX:
        RetVar = MPU_NOK;
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        State = IDLE_TX;

        break;

    default:
        break;
    }
    return RetVar;

}

mpu6050_CheckType MPU_readByte(uint8_t regAddr, uint8_t *data)
{
    static Mstr_Rx_State State = IDLE_RX;
    static I2C_CheckType Check;
    static mpu6050_CheckType RetVar = MPU_NOK;

    switch (State)
    {
    case IDLE_RX:
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Transmit, MPU_GROUP_ID);
        I2C_SendData(&regAddr, MPU_GROUP_ID);
        RetVar = MPU_BUSY;
        State = START_RX;
        break;
    case START_RX:
        Check = I2C_CheckBusStat(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = START_RX;
        }
        else
        {
            I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN),
            MPU_GROUP_ID);
            State = CHECK_ADD_SENT_RX;
            //  RetVar = MPU_BUSY;
        }
        break;
    case CHECK_ADD_SENT_RX:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_ADD_SENT_RX;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_RX;
            }
            else
            {
                State = REPEAT_START;
            }
        }
        break;
    case REPEAT_START:
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Receive, MPU_GROUP_ID);
        I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP),
        MPU_GROUP_ID);
        State = CHECK_DATA_RECEIVED;
        break;
    case CHECK_DATA_RECEIVED:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_DATA_RECEIVED;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_RX;
            }
            else
            {
                State = READ_DATA;
            }
        }
        break;
    case READ_DATA:
        I2C_ReadData(data, MPU_GROUP_ID);
        RetVar = MPU_OK;
        State = IDLE_RX;

        break;

    case ERROR_RX:
        // Error Handling
        RetVar = MPU_NOK;
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        State = IDLE_RX;

        break;

    default:
        break;
    }
    return RetVar;

}

mpu6050_CheckType MPU_readBytes(uint8_t regAddr, uint8_t Size, uint8_t *data)
{

    static Mstr_Rx_State State = IDLE_RX;
    static I2C_CheckType Check;
    static uint16_t counter = 0;
    static mpu6050_CheckType RetVar = MPU_NOK;

    switch (State)
    {
    case IDLE_RX:

        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Transmit, MPU_GROUP_ID);
        I2C_SendData(&regAddr, MPU_GROUP_ID);
        RetVar = MPU_BUSY;
        State = START_RX;

        break;
    case START_RX:
        Check = I2C_CheckBusStat(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = START_RX;
        }
        else
        {
            I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN),
            MPU_GROUP_ID);
            State = CHECK_ADD_SENT_RX;
            //RetVar = MPU_BUSY;
        }
        break;
    case CHECK_ADD_SENT_RX:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_ADD_SENT_RX;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_RX;
            }
            else
            {
                State = REPEAT_START;
            }
        }
        break;
    case REPEAT_START:
        I2C_SetSlaveAdd(mpu6050_Slave_Address, I2C_Receive, MPU_GROUP_ID);
        I2C_SetOperation((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_ACK),
        MPU_GROUP_ID);
        State = CHECK_DATA_RECEIVED;
        break;

    case CHECK_DATA_RECEIVED:
        Check = I2C_CheckBUSY(MPU_GROUP_ID);
        if (Check == I2C_NOK)
        {
            State = CHECK_DATA_RECEIVED;
        }
        else
        {
            Check = I2C_CheckErrBit(MPU_GROUP_ID);
            if (Check == I2C_NOK)
            {
                State = ERROR_RX;
            }
            else if (counter < (Size - 1))
            {
                State = READ_DATA;
            }
            else
            {
                I2C_ReadData(&data[counter], MPU_GROUP_ID);
                RetVar = MPU_OK;
                counter = 0;
                State = IDLE_RX;

            }
        }
        break;

    case READ_DATA:
        counter++;

        if (counter < (Size - 1))
        {
            I2C_ReadData(&data[counter - 1], MPU_GROUP_ID);
            I2C_SetOperation((I2C_MCS_RUN | I2C_MCS_ACK),
            MPU_GROUP_ID);
            State = CHECK_DATA_RECEIVED;
        }
        else
        {
            I2C_ReadData(&data[counter - 1], MPU_GROUP_ID);

            I2C_SetOperation((I2C_MCS_RUN | I2C_MCS_STOP),
            MPU_GROUP_ID);
            State = CHECK_DATA_RECEIVED;

        }
        break;
    case ERROR_RX:

        RetVar = MPU_NOK;
        I2C_SetOperation((I2C_MCS_STOP), MPU_GROUP_ID);
        State = IDLE_RX;

        break;
    default:
        break;
    }
    return RetVar;

}

mpu6050_CheckType MPU_InitBits(uint8_t regAddr, uint8_t bitStart,
                               uint8_t length, uint8_t data)
{
    mpu6050_CheckType Status = MPU_NOK;
    uint8_t temp;
    uint8_t mask;
    if (length > 0)
    {
        while (Status != MPU_OK)
        {
            Status = MPU_readByte(regAddr, &temp);
        }
        mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);
        data &= mask;
        temp &= ~(mask);
        temp |= data;
        Status = MPU_NOK;
        while (Status != MPU_OK)
        {
            Status = MPU_writeByte(regAddr, temp);
        }
    }
    return MPU_OK;
}

mpu6050_CheckType MPU_Who(uint8_t *data)
{
    MPU_readByte(MPU6050_RA_WHO_AM_I, data);

    return MPU_OK;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
