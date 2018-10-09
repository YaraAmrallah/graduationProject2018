

#ifndef UART_H_
#define UART_H_

#include "UART_Cfg.h"
#include <stdint.h>

typedef enum {UART_OK = 0, UART_NOK} UART_RetType;
typedef enum {OneStopBit = 0, TwoStopBit = 1} UART_StopBitsType;
typedef enum {Data_5 = 0, Data_6 = 1, Data_7 = 2, Data_8 = 3} UART_WordLength;
typedef enum {Disabled = 0, Enabled = 1} UART_EnableType;
typedef enum {FIFO_2=0, FIFO_4=1, FIFO_8=2, FIFO_12=3, FIFO_14=4}FIFO_INT_Level ;
typedef enum {Regular_Speed = 16, High_speed = 8} BaudRate_Speed;
typedef enum {SystemClock=0, PIOSC=5}UART_ClockCfg; // if PIOSC is selected, Must be 9MHz or higher.


typedef struct {

uint8_t   UART_ID;
uint32_t BaudRate;
BaudRate_Speed BR_Speed;
UART_StopBitsType StopBits;
UART_WordLength WordLen;
UART_EnableType Parity;
UART_EnableType StickParity;
UART_EnableType FIFOEN;
FIFO_INT_Level FIFO_Level_TX;
FIFO_INT_Level FIFO_Level_RX;
UART_ClockCfg ClockCfg;

/*interrpts*/
uint32_t UARTIM;     //interrupt mask

/*9 bit mode */
uint8_t  UART9BITAMASK;
uint32_t UART9BITADDR;

/*additional options*/
UART_EnableType  UARTDMACTL;  //DMA.
UART_EnableType  UARTILPR;    //IR low power register.

}UART_Cfg_Type;




UART_RetType UART_Init(void);
UART_RetType UART_SendByte(uint8_t UART_ID,uint8_t BYTE);
UART_RetType UART_ReceiveByte(uint8_t UART_ID,uint8_t *data);
UART_RetType UART_SendString(uint8_t UART_ID,uint8_t *String);
void MinUARTDelay(uint64_t time);
UART_RetType UART_ReceiveStatus(uint8_t UART_ID,uint8_t *status,uint8_t shift);
UART_RetType ErrorClear(uint8_t UART_ID,uint8_t shift);
UART_RetType UART_GetInterruptStatus(uint8_t UART_ID,uint16_t *status,uint8_t shift);
UART_RetType UART_ClearInterruptStatus(uint8_t UART_ID,uint8_t shift);
UART_RetType UART_PeripheralProperties(uint8_t UART_ID,uint8_t *status,uint8_t peripheral);
extern const UART_Cfg_Type UART_CfgParam [USED_UART_MODULES];
#endif /* UART_H_ */
