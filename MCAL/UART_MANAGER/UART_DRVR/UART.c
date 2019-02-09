#include "UART.h"
#include "UART_Cfg.h"
#include "M4MemMap.h"
#include <stdint.h>

typedef volatile uint32_t* const UART_ADDR_CASTING;
#define UART_NUMBER 8
#define SysClck 16000000
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

//store the state of UART (initialized , transmitting, receiving).
uint8_t UARTDriverStates[USED_UART_MODULES] = {0};
//store the length of the bytes to be transmitted
uint32_t UARTTxLength[USED_UART_MODULES];
//store the number of already transmitted bytes
uint32_t UARTTxCount[USED_UART_MODULES];
//saves the pointer of the first character that was transmitted such that it is not lost due to overwriting in the stack
uint8_t *UARTTxBuffPtr[USED_UART_MODULES];

//store the length of the bytes to be Received
uint32_t UARTRxLength[USED_UART_MODULES];
//store the number of already Received bytes
uint32_t UARTRxCount[USED_UART_MODULES];
//saves the pointer of the first character that was Received such that it is not lost due to overwriting in the stack
uint8_t *UARTRxBuffPtr[USED_UART_MODULES];

//pointers to function to save TxDone callback function address.
UART_CallBackPtr_t TxCallBck[USED_UART_MODULES];
//pointers to function to save RxDone callback function address.
UART_CallBackPtr_t RxCallBck[USED_UART_MODULES];



/*****************************UART STATES*********************************/
#define UARTState_uninit        0x00
#define UARTState_init          0x01
#define UARTState_Transmitting  0x02
#define UARTState_Receiving     0x03
/*************************************************************************/

/********
 * Helper Functions
 *              **************/
 static void TxManage(uint8_t UART_ID);
 static void RxManage(uint8_t UART_ID);
 static void IntManage(UARTNumber UARTX);
 /*************************/



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
double BaudRateDivisor=0;
const UART_Cfg_Type * CfgPtr;

for(i=0;(i<USED_UART_MODULES&&ErrorFlag!=1);i++){
    CfgPtr = &UART_CfgParam[i];

    if(UART_CfgParam[i].UART_ID < UART_NUMBER){

    RCGCUART_REG |= 1 <<(CfgPtr->UART_ID);                      //generate UART clock
    UARTCTL_REG(CfgPtr->UART_ID) &= ~(1<<0);                    //DISABLE uart
    UARTCTL_REG(CfgPtr->UART_ID) &= ~(1<<4);                    //Flush the FIFO transmit.
    /******Baud Rate Assignment ******/
    BaudRateDivisor = (double)SysClck / ((CfgPtr->BR_Speed) * (CfgPtr->BaudRate));  //Divisor calculation
    UARTIBRD_REG(CfgPtr->UART_ID) = (uint32_t) BaudRateDivisor;             //integer value assignment.
    BaudRateDivisor-=(uint32_t)BaudRateDivisor;                             //removing integer part.
    UARTFBRD_REG(CfgPtr->UART_ID) =(uint32_t)((BaudRateDivisor * 64) + 0.5); //floating value assignment.
    UARTCTL_REG(CfgPtr->UART_ID) |= ( ((CfgPtr->BR_Speed)/8 == 2 ? 0 : 1) << 5); //Divisor Assignment

    UARTLCRH_REG(CfgPtr->UART_ID) |= ((CfgPtr->StopBits) << 3);       //STOP BITS
    UARTLCRH_REG(CfgPtr->UART_ID) |= ((CfgPtr->WordLen) << 5);        // data length
    UARTLCRH_REG(CfgPtr->UART_ID) |= ((CfgPtr->Parity) << 1);         //parity assignment
    UARTLCRH_REG(CfgPtr->UART_ID) |= ((CfgPtr->StickParity) << 7);    //stick parity

    UARTLCRH_REG(CfgPtr->UART_ID) |= ((CfgPtr->FIFOEN) << 4);         //fifo Enable/Disable
    UARTIFLS_REG(CfgPtr->UART_ID) = CfgPtr->FIFO_Level_TX;            //fifo tx
    UARTIFLS_REG(CfgPtr->UART_ID) |= (CfgPtr->FIFO_Level_RX <<3);     //fifo rx
    UARTCC_REG(CfgPtr->UART_ID) = CfgPtr->ClockCfg;                   //configuring clock

    UARTMIS_REG(CfgPtr->UART_ID) |= CfgPtr->UARTIM;             // interrupt mask.
    UART9BITADDR_REG(CfgPtr->UART_ID) |=CfgPtr->UART9BITADDR;   // 9 BIT mode (ENABLE/DISABLE)
    UART9BITAMASK_REG(CfgPtr->UART_ID)|=CfgPtr->UART9BITAMASK;  // 9 BIT MASK
    UARTDMACTL_REG(CfgPtr->UART_ID)   |=CfgPtr->UARTDMACTL;     // DMA CONTROL
    UARTILPR_REG(CfgPtr->UART_ID)     |=CfgPtr->UARTILPR;       //LOW POWER MODE
    UARTCTL_REG(CfgPtr->UART_ID) |= (1<<0);                     //enable UART.

    UARTDriverStates[CfgPtr->UART_ID] = UARTState_init;         //Switch to initialized state

    TxCallBck[CfgPtr->UART_ID] = CfgPtr->TxDonePtr ;            //store the address of the Tx callback pointer
    RxCallBck[CfgPtr->UART_ID] = CfgPtr->RxDonePtr ;            //store the address of the Rx callback pointer

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



UART_RetType UART_StartTransmission(uint8_t* Tx_text,uint32_t TxLength,uint8_t UART_ID){
    UART_RetType RetVar;
    const UART_Cfg_Type * CfgPtr;
    //check channel ID
    if(UART_ID < USED_UART_MODULES){
        //get teh corresponding channel ID
        CfgPtr = &UART_CfgParam[UART_ID];
        //check if the mentioned module is initialized
        if(UARTDriverStates[CfgPtr->UART_ID] & UARTState_init){
           //change state to be transmitting.
           UARTDriverStates[CfgPtr->UART_ID] = UARTState_Transmitting;
           //save the pointer of the first character of the data to be transmitted.
           UARTTxBuffPtr[CfgPtr->UART_ID] = Tx_text;
           //save the length of the data to be transmitted in a global array.
           UARTTxLength[CfgPtr->UART_ID] = TxLength;
           //initialize UART corresponding character counter to zero.
           UARTTxCount[CfgPtr->UART_ID] = 0;
           //Transmit the first byte
           UARTSend(Tx_text[0],UART_ID);
           RetVar = UART_OK;

        }else
        {
            RetVar = UART_NOK;
        }
    }
    else
    {
        RetVar = UART_NOK;
    }

    return RetVar;
}

/**A function to stop the current transmission**/
UART_RetType UART_StopCurrentTransmission(uint8_t UART_ID){
    UART_RetType RetVar;
    const UART_Cfg_Type * CfgPtr;
    //check channel ID
    if(UART_ID < USED_UART_MODULES){
        //get the corresponding channel ID
        CfgPtr = &UART_CfgParam[UART_ID];
        //check if the mentioned module is Transmitting
        if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Transmitting){
           //disable transmitter
          UARTCTL_REG(UART_ID) &= ~(1<<8);

            //change state to be Uninitialized.
            UARTDriverStates[CfgPtr->UART_ID] = UARTState_uninit;

            RetVar = UART_OK;
        }else {
            RetVar = UART_NOK;
        }
    }
    else{
        RetVar = UART_NOK;
    }
    return RetVar;
}

/**A Function to get the number of transmitted bytes during transmission****/
UART_RetType UART_GetNumofTxBytes(uint8_t UART_ID, uint32_t *NumPtr){
    UART_RetType RetVar;
       const UART_Cfg_Type * CfgPtr;
       //check channel ID
       if(UART_ID < USED_UART_MODULES){
           //assign the address of the corresponding channel ID
           CfgPtr = &UART_CfgParam[UART_ID];
           //CHECK  the state of the UART channel
           if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Transmitting){
               *NumPtr = UARTTxCount[CfgPtr -> UART_ID];
               RetVar = UART_OK;
           }else
           {
               RetVar = UART_NOK;
           }

       }
       else{
           RetVar = UART_NOK;
       }
       return RetVar;
}



/**A function to request reception**/

UART_RetType UART_StartReceiving(uint8_t UART_ID, uint8_t* Rx_Text, uint32_t RxLength){
    UART_RetType RetVar;
         const UART_Cfg_Type * CfgPtr;
         //check channel ID
         if(UART_ID < USED_UART_MODULES){

             //assign the address of the corresponding channel ID
                CfgPtr = &UART_CfgParam[UART_ID];

             //check if uart is initialized
             if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_init){

                 //enable the receiver
                 UARTCTL_REG(UART_ID) = (1<<9);

                 //store the Receiving pointer address
                 UARTRxBuffPtr[CfgPtr->UART_ID] = Rx_Text;

                 //store the length of data
                 UARTRxLength[CfgPtr->UART_ID] = RxLength;

                 //initialize the Rx count by zero
                 UARTRxCount[CfgPtr->UART_ID] = 0;

                 //change UART state to receiving
                 UARTDriverStates[CfgPtr->UART_ID] = UARTState_Receiving;

                 RetVar = UART_OK;
             }
             else{
                 RetVar = UART_NOK;
             }
         }
         else{
             RetVar = UART_NOK;
         }
         return RetVar;
}


/**A function to stop the current reception**/
UART_RetType UART_StopCurrentReception(uint8_t UART_ID){
    UART_RetType RetVar;
             const UART_Cfg_Type * CfgPtr;
             //check channel ID
             if(UART_ID < USED_UART_MODULES){
                 //assign the address of the corresponding channel ID
                 CfgPtr =&UART_CfgParam[UART_ID];

                 if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Receiving){

                     //disable UARTRx
                     UARTCTL_REG(CfgPtr->UART_ID) = ~(1<<9);

                     //change UART current state to be uninitialized
                     UARTDriverStates[CfgPtr->UART_ID] = UARTState_uninit;

                     RetVar = UART_OK;
                 }
                 else
                 {
                     RetVar = UART_NOK;
                 }


             }
             else{
                 RetVar = UART_NOK;
             }
             return RetVar;
}



/**A function to get the number of received Rx bytes**/
UART_RetType UART_GetNumofRxBytes(uint8_t UART_ID, uint32_t *NumPtr){
    UART_RetType RetVar;
       const UART_Cfg_Type * CfgPtr;
       //check channel ID
       if(UART_ID < USED_UART_MODULES){
           //assign the address of the corresponding channel ID
           CfgPtr = &UART_CfgParam[UART_ID];
           //CHECK  the state of the UART channel
           if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Transmitting){
               *NumPtr = UARTRxCount[CfgPtr -> UART_ID];
               RetVar = UART_OK;
           }else
               {
               RetVar = UART_NOK;
               }

               }
               else{
               RetVar = UART_NOK;
               }

       return RetVar;
}

/**Transmission and reception handling**/
UART_RetType UART_ManageOngoingOperations(uint8_t UART_ID){
    UART_RetType RetVar;

    if(UART_ID < USED_UART_MODULES){
        TxManage(UART_ID);
        RxManage(UART_ID);
        RetVar = UART_OK;
    }
    else
    {
        RetVar = UART_NOK;
    }
    return RetVar;
}



/****A function is used to completely receive data
 * increment the counter one by one till cout = length ******/
static void RxManage(uint8_t UART_ID){
    const UART_Cfg_Type * CfgPtr;
    //assign the address of the corresponding channel ID
      CfgPtr = &UART_CfgParam[UART_ID];
    //check if the UART is receiving
      if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Receiving){
          //check if it is done or not
          if(UARTRxCount[CfgPtr->UART_ID] <UARTRxLength[CfgPtr->UART_ID]){
              //if the receiver is empty
              if((UARTFR_REG(UART_ID) & (1<<6))){
                  //Receive the byte in the buffer
                  *(UARTRxBuffPtr[CfgPtr->UART_ID] + UARTRxCount[CfgPtr->UART_ID]) = UARTReceive(UART_ID);
                  //increment the counter
                  UARTRxCount[CfgPtr->UART_ID] ++;
                  //clear the flag


              }
              else{}
          }
          else
          {
              //disable receiver
              UARTCTL_REG(CfgPtr->UART_ID) = ~(1<<9);
              //execute call back to indicate that reception is finished
              RxCallBck[CfgPtr->UART_ID]();
              //Change the state to uninitialized
              UARTDriverStates[CfgPtr->UART_ID] = UARTState_uninit;
          }
      }
      else {}

}


/****A function is used to completely receive data
 * increment the counter one by one till cout = length ******/
static void TxManage(uint8_t UART_ID)
{
    const UART_Cfg_Type * CfgPtr;
    //assign the address of the corresponding channel ID
      CfgPtr = &UART_CfgParam[UART_ID];
     //check that uart is transmitting
      if(UARTDriverStates[CfgPtr->UART_ID] == UARTState_Transmitting)
      {
          //check if the tx register is empty and ready to assign new data
          if(!(UARTFR_REG(UART_ID) & (1<<5)))
          {
              //increment the count to send the next character
              UARTTxCount[CfgPtr->UART_ID]++;
              //check if TX done or not
              if(UARTTxCount[CfgPtr->UART_ID] < UARTTxLength[CfgPtr->UART_ID])
              {
                  //send the nex character
                  UARTSend(*(UARTTxBuffPtr[CfgPtr->UART_ID] + UARTTxCount[CfgPtr->UART_ID]),UART_ID);
              }
              else
              {
                  //check for the last byte
                  //Disable transmitter
                  UARTCTL_REG(CfgPtr->UART_ID) = ~(1<<8);
                  //execute callback function which indicates finishing transmission.
                  TxCallBck[CfgPtr->UART_ID]();
                  //change the state of the uart TO UN INITIALIZED
                  UARTDriverStates[CfgPtr->UART_ID]= UARTState_uninit;

              }
          }
          else {}
      }
      else{}
}

/****A function to handle TX and RX interrupts******/
static void IntManage(UARTNumber UARTX)
{
    //check if the uartx is initialized
    if(UARTX <= USED_UART_MODULES)
    {
    //CHECK THE STATE OF THE UART
    //in case of transmission
        if(UARTDriverStates[UARTX] == UARTState_Transmitting)
        {
            //check if the tx register is empty and ready to assign new data
                   if(!(UARTFR_REG(UARTX) & (1<<5)))
                   {
                       //INCREMENT THE counter to send new byte
                       UARTTxCount[UARTX]++;
                       //Check if transmission is done or not
                       if(UARTTxCount[UARTX]<UARTTxLength[UARTX])
                       {
                           //Send the next character
                           UARTSend(*(UARTTxBuffPtr[UARTX] + UARTTxCount[UARTX]),UARTX);
                       }
                       else
                       {
                           //clear RIS and MIS for the transmitter of the corresponding uart
                           UARTICR_REG(UARTX) = (1<<5);
                           //disable interrupt for the corresponding UART module transmitter.
                           UARTMIS_REG(UARTX) = ~(1<<5);
                           //disable transmitter
                           UARTCTL_REG(UARTX) = ~(1<<8);
                           //change the state of UART to uninitialized
                           UARTDriverStates[UARTX] = UARTState_uninit;
                           //execute call back function to indicate the end of transmission
                           TxCallBck[UARTX]();
                           //clear a flag.


                       }
                   }

        }


        //in case of receiving
        if(UARTDriverStates[UARTX] == UARTState_Receiving)
        {
            // check if all data is receved or not
            if(UARTRxCount[UARTX] <UARTRxLength[UARTX])
            {
                         //if the receiver is empty
                         if((UARTFR_REG(UARTX) & (1<<6)))
                         {
                             //Receive the byte in the buffer
                             *(UARTRxBuffPtr[UARTX] + UARTRxCount[UARTX]) = UARTReceive(UARTX);
                             //increment the counter
                             UARTRxCount[UARTX] ++;
                             //clear a flag
                         }
            }
            else
            {
                //clear RIS and MIS for the receiver of the corresponding uart
                UARTICR_REG(UARTX) = (1<<4);
                //disable interrupt for the corresponding UART module receiver.
                UARTMIS_REG(UARTX) = ~(1<<4);
                //disable receiver
                UARTCTL_REG(UARTX) = ~(1<<9);
                //change the state of UART to uninitialized
                UARTDriverStates[UARTX] = UARTState_uninit;
                //execute call back function to indicate the end of transmission
                RxCallBck[UARTX]();
                //clear a flag.
            }
         }

    }
}



/***************INTERRUPT handlers*********************/
/****UART0*****/
void UART0_Handler(void)
{
    IntManage(UART0);
}

/****UART1*****/
void UART1_Handler(void)
{
    IntManage(UART1);
}

/****UART2*****/
void UART2_Handler(void)
{
    IntManage(UART2);
}

/****UART3*****/
void UART3_Handler(void)
{
    IntManage(UART3);
}

/****UART4*****/
void UART4_Handler(void)
{
    IntManage(UART4);
}

/****UART5*****/
void UART5_Handler(void)
{
    IntManage(UART5);
}

/****UART6*****/
void UART6_Handler(void)
{
    IntManage(UART6);
}

/****UART7*****/
void UART7_Handler(void)
{
    IntManage(UART7);
}






/***********************************************old implementation***********************************************/

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
                while((UARTFR_REG(UART_ID) & (1<<5)));
                UART_SendByte(UART_ID, String[i]);

                i++;
            }
            RetVar= UART_OK;

        }else{RetVar=UART_NOK;}
        return RetVar;
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

/******************************************/
