#include<stdint.h>
#include<string.h>
#include"UART.h"

#ifndef GPS_H_
#define GPS_H_

// enum for the return value of functions either Ok or BUSY
typedef enum {GPS_OK = 0, GPS_NOK = 1}GPS_RetType;
typedef enum {GPS_DONE = 0, GPS_READY= 1}GPS_State;


GPS_RetType GPS_Init();
GPS_RetType GPS_GetData(uint8_t InputArray[],uint8_t GPRMCData[]);
GPS_RetType GPS_ReqData(uint8_t UART_ID);


#endif /* GPS_H_ */
