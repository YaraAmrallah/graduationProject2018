#include<stdint.h>
#include<CommonFunctions.h>
#include "UART.h"
#include "GPS_Stack_Cfg.h"

#ifndef GPS_H_
#define GPS_H_

#define DEBUG_MODE             1
#define TOTAL_ARR_LENGTH       1024
#define TRANSFER_SIZE          512
#define GPRMC_DATA_LENGTH      40


// enum for the return value of functions either Ok or BUSY
typedef enum {GPS_OK = 0, GPS_NOK = 1}GPS_RetType;
typedef enum {GPS_DONE_PARSING = 0, GPS_DONE_RECEIVING = 1, GPS_READY = 2, GPS_INITIALIZED = 3}GPS_State;


GPS_RetType GPS_Init();
GPS_RetType GPS_Parse(uint8_t InputArray[],uint8_t GPRMCData[][GPRMC_DATA_LENGTH]);
GPS_RetType GPS_ReqData(uint8_t UART_ID);
void GPS_Main_Function_Handling();

extern UARTNumber UsedUartModuleNum;

#endif /* GPS_H_ */
