#include "UART.h"
#include "UART_Cfg.h"

const UART_Cfg_Type UART_CfgParam [USED_UART_MODULES]={
     //Port ID,
     //115200 , 8bit word length, disabled fifo, one stop bit , odd parity, parity disabled
     //Disable send break,  use system clock ,enable transmit& receive, UART disable ,fifo = 1/2
     //Disable all interrupt error masks, 9 bit address mask, 9bit mode disabled,
     //DMA disabled , low power divisor value=0 (disabled);
                  { 0x00,                       //ID
                    0x08,0x2C,0x60,             //UARTIBRD, UARTFBRD, UARTLCRH
                    0x00,0x0300,0x12,            //UARTCC, UARTCTL,UARTIFLS
                    0x0000,0xFF,0x0000,         //UARTIM,9BIT MASK,9BIT ADDRESS
                    0x00,0x00                   //DMA, LOW POWER
                  }
};
