#include "UART.h"
#include "UART_Cfg.h"

const UART_Cfg_Type UART_CfgParam [USED_UART_MODULES]={
     //UART ID,
     //115200 , 8bit word length, disabled fifo, one stop bit , odd parity, parity disabled
     //Disable send break,  use system clock ,enable transmit& receive, UART disable ,fifo = 1/2
     //Disable all interrupt error masks, 9 bit address mask, 9bit mode disabled,
     //DMA disabled , low power divisor value=0 (disabled);

         {
          0x00,                 //ID
          115200,               //Baud Rate
          Regular_Speed,        // /16 in the baud rate equation
          OneStopBit,
          Data_8,               //Data size
          Disabled,             // parity
          Disabled,             //stick parity
          Disabled,             //fifo
          FIFO_2,               //fifo tx
          FIFO_2,               //fifo rx
          SystemClock,          //UART clock = sytem clock.
          0x00,                 //interrupt mask.
          0xFF,                 //9Bit mask
          0x0000,               //9Bit address.
          Disabled,             //DMA
          Disabled              //IR
         }
};
