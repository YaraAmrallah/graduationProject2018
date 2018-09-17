#include "GPIO.h"
#include "GPIO_Cfg.h"
#include "M4MemMap.h"
#include <stdint.h>
typedef volatile uint32_t* const GPIO_RegAddType;
#define PORTS_NUMBER 6u
/*Register memory map*/
#define PORTA_BASE_ADDRESS 0x40004000
#define PORTB_BASE_ADDRESS 0x40005000
#define PORTC_BASE_ADDRESS 0x40006000
#define PORTD_BASE_ADDRESS 0x40007000
#define PORTE_BASE_ADDRESS 0x40024000
#define PORTF_BASE_ADDRESS 0x40025000
static const uint32_t PortsBaseAddressLut[PORTS_NUMBER] =
{       PORTA_BASE_ADDRESS,
	PORTB_BASE_ADDRESS,
	PORTC_BASE_ADDRESS,
	PORTD_BASE_ADDRESS,
	PORTE_BASE_ADDRESS,
	PORTF_BASE_ADDRESS
};
#define GPIO_REG_ADDRESS(CHANNEL_ID,REG_OFFSET)\
(PortsBaseAddressLut[CHANNEL_ID] + REG_OFFSET)

/*Port Control*/
#define GPIODATA_WRITE(DATA,MASK,PORT_ID)  *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x000) + (MASK << 2))) = DATA
#define GPIODATA_READ(MASK,PORT_ID)        *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x000) + (MASK << 2)))
#define GPIODIR_REG(PORT_ID)               *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x400))

/*Interrupt Control*/
#define GPIOIS_REG(PORT_ID)               *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x404))
#define GPIOIBE_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x408))
#define GPIOIEV_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x40C))
#define GPIOIM_REG(PORT_ID)               *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x410))
#define GPIORIS_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x414))
#define GPIOMIS_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x418))
#define GPIOICR_REG(PORT_ID)              *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x41C))

/*Mode Control*/
#define GPIOAFSEL_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x420))
#define GPIOPCTL_REG(PORT_ID)             *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x52C))
#define GPIOADCCTL_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x530))
#define GPIODMACTL_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x534))

/*Pad control*/
#define GPIODR2R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x500))
#define GPIODR4R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x504))
#define GPIODR8R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x508))
#define GPIOODR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x50C))
#define GPIOPUR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x510))
#define GPIOPDR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x514))
#define GPIOSLR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x518))
#define GPIODEN_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x51C))
#define GPIOAMSEL_REG(PORT_ID)          *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x528))

/*Commit control*/
#define GPIOLOCK_REG(PORT_ID)          *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x520))
#define GPIOCR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x524))

/*Internal symbols*/
#define GPIO_PORT_UNLOCK_VALUE 0x4C4F434B
#define GPIO_INT_SENCE_BIT_NUM 1
#define GPIO_INT_SENCE_MASK (1 << GPIO_INT_EVENT_BIT_NUM)
#define GPIO_INT_EVENT_BIT_NUM 0
#define GPIO_INT_EVENT_MASK (1 << GPIO_INT_EVENT_BIT_NUM)
#define GPIO_INT_BE_BIT_NUM 2
#define GPIO_INT_BR_MASK (1 << GPIO_INT_BE_BIT_NUM)

static uint8_t GPIO_GroupState[GPIO_GROUPS_NUMBER] = {0};

/*A function to initialize all the GPIO Groups in the configurations*/
GPIO_CheckType GPIO_Init(void)
{
	uint8_t LoopIndex;
	uint8_t ErrorFlag = 0;
	GPIO_CheckType RetVar;
	const GPIO_CfgType * CfgPtr;


	for(LoopIndex = 0; (LoopIndex < GPIO_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
	{
		if(GPIO_ConfigParam[LoopIndex].PortId < PORTS_NUMBER)
		{
			/*Enable port clock gate*/
			CfgPtr = & GPIO_ConfigParam[LoopIndex];
			RCGCGPIO_REG |= 1 << CfgPtr->PortId;
			/*Unlock the group*/
			GPIOLOCK_REG(CfgPtr->PortId) = GPIO_PORT_UNLOCK_VALUE;
			GPIOCR_REG(CfgPtr->PortId)  |= (CfgPtr->PortMask);
			/*Data Control*/
			GPIODIR_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->PortDirection);
			/*Pad Control*/
			GPIODR2R_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->Use2mACrt);
			GPIODR4R_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->Use4mACrt);
			GPIODR8R_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->Use8mACrt);

			GPIOPDR_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->UsePullDown);
			GPIOPUR_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->UsePullUp);
			GPIOODR_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->UseOpenDrain);

			GPIODEN_REG(CfgPtr->PortId)   |= (CfgPtr->PortMask & CfgPtr->SetPinType);
			GPIOAMSEL_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & ~CfgPtr->SetPinType);

			/*Mode control*/
			GPIOAFSEL_REG(CfgPtr->PortId)  |= (CfgPtr->PortMask & CfgPtr->UseAlterFun);
			GPIOADCCTL_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->UseACDTrig);
			GPIODMACTL_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->UseDMATrig);
			GPIO_GroupState[LoopIndex] = 1;
			RetVar = GPIO_OK;
		}
		else
		{
			/*Invalid GroupId*/
			RetVar = GPIO_NOK;
			ErrorFlag = 1;
		}


	}
	return RetVar;
}


/*A function to Digital Write data to a specific group*/
GPIO_CheckType GPIO_Write(uint8_t GroupId,uint8_t GroupData)
{
	const GPIO_CfgType * CfgWrhit;
	GPIO_CheckType RetVar;
	if (( GroupId <GPIO_GROUPS_NUMBER))
	{
		CfgWrhit =  & GPIO_ConfigParam[GroupId];

		if ((CfgWrhit->PortDirection == 0xff)&&(CfgWrhit->SetPinType == 0xff)&&(CfgWrhit->UseAlterFun==0x00) &&(GPIO_GroupState[GroupId]==1 ))
		{
		GPIODATA_WRITE(GroupData,CfgWrhit->PortMask,CfgWrhit->PortId);
		RetVar = GPIO_OK;
	    }else {RetVar = GPIO_NOK ; }
	}else {RetVar = GPIO_NOK ; }
	return RetVar;
}




/*A function to Digital read data from a specific group*/
GPIO_CheckType GPIO_Read(uint8_t GroupId,uint8_t* GroupDataPtr)
{
    const GPIO_CfgType * CfgRead;
        GPIO_CheckType RetVar;
        if ((GroupId<GPIO_GROUPS_NUMBER)){
            CfgRead =  & GPIO_ConfigParam[GroupId];
            if ((CfgRead->PortDirection == 0x00)&&(CfgRead->SetPinType == 0xff)&&(CfgRead->UseAlterFun==0x00)&&(GPIO_GroupState[GroupId]==1)){
                *GroupDataPtr  = GPIODATA_READ(CfgRead->PortMask,CfgRead->PortId);
                RetVar = GPIO_OK;
            }else {RetVar = GPIO_NOK; }
        }else {RetVar = GPIO_NOK; }
        return RetVar;
		}




/*A function to select which peripheral will be connected to a GPIO pin*/
GPIO_CheckType GPIO_SetAlternFuntion(uint8_t GroupId,uint8_t AlternFuncId)
{
    GPIO_CheckType RetVar;
    const GPIO_CfgType * CfgSel;
    CfgSel = & GPIO_ConfigParam[GroupId];
    if ((GroupId <GPIO_GROUPS_NUMBER)){
        switch (AlternFuncId){
        case 0:
            GPIOAFSEL_REG(CfgSel->PortId) &= ~(CfgSel->PortMask);
            break;
        case 1:
            GPIOAFSEL_REG(CfgSel->PortId) |= (CfgSel->PortMask);
            switch (CfgSel->PortId){
            //PORT A
            case 0x00: //UART 0
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<0 || 1<<4);     //pin0 & pin1
                break;
            //PORT B
            case 0x01: //UART 1
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<0 || 1<<4);     //pin0 & pin1
                break;
            //PORT C
            case 0x02: //UART 4 && UART 3
                if(CfgSel->PortMask==1<<4||CfgSel->PortMask==1<<5){ //UART4
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<16 || 1<<20);  //pin4 & pin5
                }else if(CfgSel->PortMask==1<<6 || CfgSel->PortMask == 1<<7){//UART3
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<24 || 1<<28);  //pin6 & pin7
                }else if(CfgSel->PortMask==1<<0||CfgSel->PortMask==1<<1||CfgSel->PortMask==1<<2||CfgSel->PortMask==1<<3){ //m4 3arf eh el peripheral da xD
                    //no Idea what is going on here xD
                }
                break;
             //PORT D
            case 0x03: //SSI &UART6 &UART2
                if(CfgSel->PortMask==1<<0||CfgSel->PortMask==1<<1||CfgSel->PortMask==1<<2||CfgSel->PortMask==1<<3){//SSI
                    // ma 3leena
                }else if(CfgSel->PortMask==1<<4||CfgSel->PortMask==1<<5){//UART6
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<16 || 1<<20);  //pin4 & pin5
                }else if(CfgSel->PortMask==1<<6||CfgSel->PortMask==1<<7){//UART2
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<24 || 1<<28);  //pin6 & pin7
                }
                break;
              //PORT E
            case 0x04:
                if(CfgSel->PortMask==1<<0||CfgSel->PortMask==1<<1){//UART7
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<0 || 1<<4);     //pin0 & pin1
                }else if(CfgSel->PortMask==1<<4 || CfgSel->PortMask==1<<5){//UART5
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<16 || 1<<20);  //pin4 & pin5
                }
                break;
              //PORT F
            case 0x05:  //UART1 RTS,CTS
                GPIOPCTL_REG(CfgSel->PortId) |= (1<<0 || 1<<4);     //pin0 & pin1
                break;
            default:
            }
            break;

            case 2:
                if(CfgSel->PortId==0x02 &&(CfgSel->PortDirection==1<<4||CfgSel->PortDirection==1<<5) ){
                    GPIOPCTL_REG(CfgSel->PortId) |= (1<<16 || 1<<20);  //pin4 & pin5
                }
            break;

        default:
        }

        RetVar=GPIO_OK;
        }else{RetVar = GPIO_NOK;}

    return RetVar;

}
/*A function to Select the interrupt event for a specific GPIO Group*/
GPIO_CheckType GPIO_SetInterruptEvent(uint8_t GroupId,GPIO_IntEventType IntEvent,GPIO_IntMaskStatus IntMaskStatus)
{
    GPIO_CheckType RetVar;
    const GPIO_CfgType * CfgINT;
    CfgINT = &GPIO_ConfigParam[GroupId];
    if ((GroupId <GPIO_GROUPS_NUMBER)){
        if(IntMaskStatus == MASK_DISABLED){
            //3e4 7yatak
        }else{
            GPIOIM_REG(CfgINT->PortId) &= ~(1<<(CfgINT->PortMask));         // disable
            if(IntEvent == EVENT_FALL_EDGE){
                GPIOIS_REG(CfgINT->PortId)  &= ~(CfgINT->PortMask); //edge.
                GPIOIBE_REG(CfgINT->PortId) &= ~(CfgINT->PortMask); //control
                GPIOIEV_REG(CfgINT->PortId) &= ~(CfgINT->PortMask); //falling
            }else if(IntEvent == EVENT_RISE_EDGE){
                GPIOIS_REG(CfgINT->PortId)  &= ~(CfgINT->PortMask); //edge.
                GPIOIBE_REG(CfgINT->PortId) &= ~(CfgINT->PortMask); //control
                GPIOIEV_REG(CfgINT->PortId) |=  (CfgINT->PortMask); //rising
            }else if(IntEvent == EVENT_BOTH_EDGE){
                GPIOIS_REG(CfgINT->PortId)  &= ~(CfgINT->PortMask); //edge.
                GPIOIBE_REG(CfgINT->PortId) |=  (CfgINT->PortMask); //control
            }else if(IntEvent == EVENT_LOW){
                GPIOIS_REG(CfgINT->PortId)  |=  (CfgINT->PortMask); //level
                GPIOIBE_REG(CfgINT->PortId) &= ~(CfgINT->PortMask); //control
                GPIOIEV_REG(CfgINT->PortId) &= ~(CfgINT->PortMask); //low level
            }else if(IntEvent == EVENT_HIGH){
                GPIOIS_REG(CfgINT->PortId)  |= (CfgINT->PortMask); //level
                GPIOIBE_REG(CfgINT->PortId) &=~(CfgINT->PortMask); //control
                GPIOIEV_REG(CfgINT->PortId) |= (CfgINT->PortMask); //high
            }

            GPIOICR_REG(CfgINT->PortId) |=  (CfgINT->PortMask); //clear
            GPIOIM_REG(CfgINT->PortId)  |=  (CfgINT->PortMask);// enable
            EN_INT(30);     //enable interrupts on port F.
        } RetVar = GPIO_OK;

    }else{RetVar = GPIO_NOK;}
    return RetVar;
}


/*A function to clear a specific pin interrupt flag*/
GPIO_CheckType GPIO_ClrInterruptFlag(uint8_t GroupId)
{
    const GPIO_CfgType * CfgINT;
    GPIO_CheckType RetVar;
    if ((GroupId <GPIO_GROUPS_NUMBER)){
        CfgINT = &GPIO_ConfigParam[GroupId];
        GPIOICR_REG(CfgINT->PortId) |= (CfgINT->PortMask); //clears RIS & MIS
        RetVar = GPIO_OK;
            }else{
                RetVar = GPIO_NOK;
            }
    return RetVar;

}
/*A function to Get a specific pin interrupt status*/
GPIO_CheckType GPIO_GetInterruptStatus(uint8_t GroupId,GPIO_IntStatus *IntStatusPtr)
{       GPIO_CheckType RetVar;

    const GPIO_CfgType * CfgINTMask;
    CfgINTMask = &GPIO_ConfigParam[GroupId];
    if ((GroupId <GPIO_GROUPS_NUMBER)){
    if(GPIORIS_REG(CfgINTMask->PortId) & (CfgINTMask->PortMask)){
        *IntStatusPtr = INT_PENDING;
    }else{*IntStatusPtr = INT_INACTIVE;}
    RetVar= GPIO_OK;
}else{
    RetVar= GPIO_NOK;
}
    return RetVar;
}

/*static uint8_t GetPinNumber(uint8_t PinMask)
{

}*/

/*void WhichTriggeredInterrupt_F(void){
    uint8_t i;
    for(i=0;i<8;i++){
        if(GPIORIS_REG(5) & 1<<i){
    switch (i){
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    case 4:

        break;
    case 8:

        break;
    case 16:

        break;
    case 32:

        break;
    case 64:

        break;
    }
        }
}
}*/
