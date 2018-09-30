

#ifndef UART_H_
#define UART_H_

#include "UART_Cfg.h"
#include <stdint.h>


typedef struct {
uint8_t   UART_ID;
uint8_t   UARTIBRD;      //integer baud rate divisor
uint8_t   UARTFBRD;      //fraction baud rate divisor
uint8_t   UARTLCRH;      //line control H
uint8_t   UARTCC;        //Clock configuration
uint32_t  UARTCTL;       //Uart control


/*interrpts*/
uint8_t  UARTIFLS;   //fifo select
uint32_t UARTIM;     //interrupt mask

/*9 bit mode */
uint8_t  UART9BITAMASK;
uint32_t UART9BITADDR;


/*additional options*/
uint8_t  UARTDMACTL;  //DMA.
uint8_t  UARTILPR;    //IR low power register.

}UART_Cfg_Type;



typedef enum {UART_OK =0, UART_NOK} UART_RetType;
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
