#include "UART.h"
#include "UART_Cfg.h"
#include "../M4MemMap.h"
#include <stdint.h>



typedef volatile uint32_t* const UART_ADDR_CASTING;
#define UART_NUMBER 8
/******memory map******/
#define UART0_BASE_ADDRESS 0x4000C000
#define UART1_BASE_ADDRESS 0x4000D000
#define UART2_BASE_ADDRESS 0x4000E000
#define UART3_BASE_ADDRESS 0x4000F000
#define UART4_BASE_ADDRESS 0x40010000
#define UART5_BASE_ADDRESS 0x40011000
#define UART6_BASE_ADDRESS 0x40012000
#define UART7_BASE_ADDRESS 0x40013000


static const uint32_t UARTBaseAddLuT[UART_NUMBER] =
{
    UART0_BASE_ADDRESS,
    UART1_BASE_ADDRESS,
    UART2_BASE_ADDRESS,
    UART3_BASE_ADDRESS,
    UART4_BASE_ADDRESS,
    UART5_BASE_ADDRESS,
    UART6_BASE_ADDRESS,
    UART7_BASE_ADDRESS
};

#define UART_ADDRESS(BASE,OFFSET) (UARTBaseAddLuT[BASE] + OFFSET)

/****UART control*****/
#define UARTSend(DATA,UART_ID)     *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x000)) = DATA
#define UARTReceive(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x000))

/**Main Registers**/
#define UARTRSR_REG(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x004)) //receive status/ error clean
#define UARTFR_REG(UART_ID)        *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x018))
#define UARTILPR_REG(UART_ID)      *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x020))
#define UARTIBRD_REG(UART_ID)      *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x024))
#define UARTFBRD_REG(UART_ID)      *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x028))
#define UARTLCRH_REG(UART_ID)      *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x02C))
#define UARTCTL_REG(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x030))

/*Interrupts*/
#define UARTIFLS_REG(UART_ID)      *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x034))
#define UARTIM_REG(UART_ID)        *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x038))
#define UARTRIS_REG(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x03C))
#define UARTMIS_REG(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x040))
#define UARTICR_REG(UART_ID)       *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x044))

/*General */
#define UARTDMACTL_REG(UART_ID)    *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x048))
#define UART9BITADDR_REG(UART_ID)  *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x0A4))
#define UART9BITAMASK_REG(UART_ID) *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0x0A8))
#define UARTPP_REG(UART_ID)        *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0xFC0))
#define UARTCC_REG(UART_ID)        *((UART_ADDR_CASTING)UART_ADDRESS(UART_ID,0xFC8))





UART_RetType UART_Init(void){
uint8_t i,ErrorFlag=0;
UART_RetType RetVar;
const UART_Cfg_Type * CfgPtr;

for(i=0;(i<USED_UART_MODULES&&ErrorFlag!=1);i++){
    CfgPtr = &UART_CfgParam[i];

    if(UART_CfgParam[i].UART_ID < UART_NUMBER){

    RCGCUART_REG |= 1 <<(CfgPtr->UART_ID);                      //generate UART clock
    UARTCTL_REG(CfgPtr->UART_ID) &= ~(1<<0);                    //DISABLE uart
    UARTCTL_REG(CfgPtr->UART_ID) &= ~(1<<4);                    //Flush the FIFO transmit.
    UARTIBRD_REG(CfgPtr->UART_ID) = CfgPtr->UARTIBRD;           //IBRD
    UARTFBRD_REG(CfgPtr->UART_ID) = CfgPtr->UARTFBRD;           //FBRD
    UARTLCRH_REG(CfgPtr->UART_ID) = CfgPtr->UARTLCRH;           //LCRH
    UARTCC_REG(CfgPtr->UART_ID)  |= CfgPtr->UARTCC;             //CLOCK
    UARTCTL_REG(CfgPtr->UART_ID) = CfgPtr->UARTCTL;            //enable Transmit, receive.
    UARTIFLS_REG(CfgPtr->UART_ID) = CfgPtr->UARTIFLS;           //FIFO select.
    UARTMIS_REG(CfgPtr->UART_ID) |= CfgPtr->UARTIM;             // interrupt mask.
    UART9BITADDR_REG(CfgPtr->UART_ID) |=CfgPtr->UART9BITADDR;   // 9 BIT mode (ENABLE/DISABLE)
    UART9BITAMASK_REG(CfgPtr->UART_ID)|=CfgPtr->UART9BITAMASK;  // 9 BIT MASK
    UARTDMACTL_REG(CfgPtr->UART_ID)   |=CfgPtr->UARTDMACTL;     // DMA CONTROL
    UARTILPR_REG(CfgPtr->UART_ID)     |=CfgPtr->UARTILPR;       //LOW POWER MODE
    UARTCTL_REG(CfgPtr->UART_ID) |= (1<<0);                     //enable UART.
}
}
if(i==USED_UART_MODULES && ErrorFlag ==0){
    RetVar=UART_OK;
}
else
{
    RetVar=UART_NOK;
}
return RetVar;
}

UART_RetType UART_SendByte(uint8_t UART_ID,uint8_t BYTE){
    UART_RetType RetVar;
    if(UART_ID < USED_UART_MODULES){
    if(!(UARTFR_REG(UART_ID) & (1<<5))){   //TRANSMITTER IS  empty
        UARTSend(BYTE,UART_ID);
        RetVar=UART_OK;
    }
    else
    {
        RetVar=UART_NOK;
    }
    }
    return RetVar;
}


UART_RetType UART_ReceiveByte(uint8_t UART_ID,uint8_t *data){
    UART_RetType RetVar;
    if(UART_ID < USED_UART_MODULES){
    if((UARTFR_REG(UART_ID) & (1<<6))){   //Receiver is  empty
        *data=UARTReceive(UART_ID);
        RetVar=UART_OK;
    }else{RetVar=UART_NOK;}
    }
    return RetVar;
}

UART_RetType UART_SendString(uint8_t UART_ID,uint8_t *String){
    UART_RetType RetVar;
    uint8_t i=0;
        if(UART_ID < USED_UART_MODULES){
            while(String[i] !='\0'){
                UART_SendByte(UART_ID, String[i]);
                MinUARTDelay(1);
                i++;
            }
            RetVar= UART_OK;

        }else{RetVar=UART_NOK;}
        return RetVar;
}

void MinUARTDelay(uint64_t time){ // in case of disabled FIFO.
    unsigned long i;
    while(time>0){
        i=133;
        while(i>0){
            i--;
        }
        time--;
    }
}

UART_RetType UART_ReceiveStatus(uint8_t UART_ID,uint8_t *status,uint8_t shift){
    UART_RetType RetVar;
    if(UART_ID < USED_UART_MODULES){
        if(shift==0xff){//read all
        *status=UARTRSR_REG(UART_ID);
        ErrorClear(UART_ID,0xff);
        }else if(shift <3){ //register limit
        *status=UARTRSR_REG(UART_ID);
        ErrorClear(UART_ID,0xff);
        }
        RetVar=UART_OK;
    }else{RetVar=UART_NOK;}
    return RetVar;
}

UART_RetType ErrorClear(uint8_t UART_ID,uint8_t shift){
    UART_RetType RetVar;
        if(UART_ID < USED_UART_MODULES){
            UARTRSR_REG(UART_ID) |= 0xFF; //clears all error flags
            RetVar=UART_OK;
        }else{RetVar=UART_NOK;}
        return RetVar;
}


UART_RetType UART_GetInterruptStatus(uint8_t UART_ID,uint16_t *status,uint8_t shift){
    UART_RetType RetVar;
            if(UART_ID < USED_UART_MODULES){
                if(shift==0xff){//read all
                *status= UARTRIS_REG(UART_ID);
                }else if(shift==0||shift==2||shift==3||shift==11){
                    RetVar=UART_NOK;  //reserved bits
                }else if(shift <=12){
                    *status= (UARTRIS_REG(UART_ID) &= (1<<shift)) >>shift;
                }
                RetVar=UART_OK;
            }else{RetVar=UART_NOK;}
            return RetVar;
}


UART_RetType UART_ClearInterruptStatus(uint8_t UART_ID,uint8_t shift){
    UART_RetType RetVar;
            if(UART_ID < USED_UART_MODULES){
                if(shift==0xff){//clear all
                UARTICR_REG(UART_ID) |= 0xff;
                }else if(shift==0||shift==2||shift==3||shift==11){
                    RetVar=UART_NOK;  //reserved bits
                }else if(shift <=12){
                    UARTICR_REG(UART_ID) |= 1<<shift;
                }
                RetVar=UART_OK;
            }else{RetVar=UART_NOK;}
            return RetVar;
}

UART_RetType UART_PeripheralProperties(uint8_t UART_ID,uint8_t *status,uint8_t peripheral){
    // Peripheral = 0 -> smart card support check.
    // Peripheral = 1 -> 9bit support check.
    UART_RetType RetVar;
                if(UART_ID < USED_UART_MODULES){
                    if(peripheral<=1){
                    *status=UARTPP_REG(UART_ID) & 1<<peripheral;
                    RetVar=UART_OK;
                    }else{
                        RetVar=UART_NOK;
                    }
                }else{RetVar=UART_NOK;}
        return RetVar;
}

