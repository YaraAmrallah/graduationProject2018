#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H
#include "I2C.h"

typedef enum
{
    I2C_WRITE = 0, I2C_READ = 1
} I2C_Direction;

typedef enum
{
    I2C_UN_INIT = 0,
    I2C_IDLE = 1,
    WRITE_DATA = 2,
    SINGLE_SEND_START = 3,
    STATUS_SEND_START = 4,
    BURST_SEND = 5,
    BURST_SENDSTAT = 6,
    READ_DATA = 7,
    SINGLE_READ = 8,
    I2C_READ_DATA_STATUS = 9,
    BURST_READ = 10,
    I2C_BURST_READSTAT = 11

} I2C_States;

void I2C_Manager(void);
void I2C_RequestWrite(uint8_t SlaveAddress, uint8_t* DataPointer,
                      uint8_t NumOfBytes);
void I2C_RequestRead(uint8_t SlaveAddress, uint8_t* DataPointer,
                     uint8_t NumOfBytes);
extern void delay100ms(uint64_t time);
extern void BlinkLed(uint8_t period, uint8_t groupId);

#endif
