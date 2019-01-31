#include "UART.h"
#include "UART_Cfg.h"
void trans (void);
void receive (void);

const UART_Cfg_Type UART_CfgParam [USED_UART_MODULES]={
             {
          0x00,                 //ID
          115200,               //Baud Rate
          Regular_Speed,        // 16 in the baud rate equation
          OneStopBit,
          Data_8,               //Data size
          Disabled,             // parity
          Disabled,             //stick parity
          Disabled,              //fifo
          FIFO_14,              //fifo tx
          FIFO_2,               //fifo rx
          SystemClock,          //UART clock = sytem clock.
          0x30,                 //interrupt mask.(enable TX,RX interrupts)
          0xFF,                 //9Bit mask
          0x0000,               //9Bit address.
          Disabled,             //DMA
          Disabled,              //IR
          trans,
          receive

         }
};
