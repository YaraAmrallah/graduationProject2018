#ifndef I2C_H
#define I2C_H

#include <stdint.h>
//#include "GPIO.h"
#include "tm4c123gh6pm.h"
#include "I2C_CFG.h"



typedef volatile uint32_t* const I2C_RegAddType;
typedef volatile uint32_t* const GPIO_RegAddType;

/*Register GPIO memory map*/
#define PORTA_BASE_ADDRESS 0x40004000
#define PORTB_BASE_ADDRESS 0x40005000
#define PORTC_BASE_ADDRESS 0x40006000
#define PORTD_BASE_ADDRESS 0x40007000
#define PORTE_BASE_ADDRESS 0x40024000
#define PORTF_BASE_ADDRESS 0x40025000

#define RCGC_BASE_ADDRESS  0x400FE000
#define PORTS_NUMBER 6u
/*Base Address of Ports*/
static const uint32_t PortsBaseAddressLut[PORTS_NUMBER] =
{
    PORTA_BASE_ADDRESS,
    PORTB_BASE_ADDRESS,
    PORTC_BASE_ADDRESS,
    PORTD_BASE_ADDRESS,
    PORTE_BASE_ADDRESS,
    PORTF_BASE_ADDRESS
};

/*Needed GPIO Registers*/
#define GPIO_REG_ADDRESS(CHANNEL_ID,REG_OFFSET)\
(PortsBaseAddressLut[CHANNEL_ID] + REG_OFFSET)

#define GPIOAFSEL_REG(PORT_ID)          *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x420))
#define GPIOPCTL_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x52C))
#define GPIODR2R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x500))
#define GPIODR4R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x504))
#define GPIODR8R_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x508))
#define GPIOLOCK_REG(PORT_ID)           *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x520))
#define GPIOCR_REG(PORT_ID)             *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x524))
#define GPIODIR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x400))
#define GPIODEN_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x51C))
#define GPIOODR_REG(PORT_ID)            *((GPIO_RegAddType)GPIO_REG_ADDRESS(PORT_ID,0x50C))
#define RCGC_GPIO                       (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORT_UNLOCK_VALUE 0x4C4F434B



#define MAX_SCL_FREQUENCY_SM		(100000U)
#define MAX_SCL_FREQUENCY_FM		(400000U)
#define	I2C_DELAY_PARAMETER			(50U)
typedef volatile uint32_t* const I2C_RegisterAddressType;

#define I2C_MAXIMUM_NUMBER						(4U)
/*
 ******************************************************************************
 *                                                                            *
 *            						BASES						              *                                                                        *
 ******************************************************************************
 */

 #define PERIPHERAL_BASE    					    	(0x40020000U)//I2C0
 #define RCC_BASE1								        (0x400FE000U)
 #define RCC_I2C_offset                                 (0x620)
 #define I2C_BASE   				 		            PERIPHERAL_BASE


 /*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						OFFSETS						              *
 ******************************************************************************
 */
 /*************************   I2C MASTER    ****************************/
 #define I2C_MSA_OFFSET                         (0x000)
 #define I2C_MCS_OFFSET                         (0x004)
 #define I2C_MDR_OFFSET                         (0x008)
 #define I2C_MTPR_OFFSET                        (0x00C)
 #define I2C_MIMR_OFFSET                        (0x010)
 #define I2C_MRIS_OFFSET                        (0x014)
 #define I2C_MMIS_OFFSET                        (0x018)
 #define I2C_MICR_OFFSET                        (0x01C)
 #define I2C_MCR_OFFSET                         (0x020)
 #define I2C_MCLKOCNT_OFFSET                    (0x024)
 #define I2C_MBMON_OFFSET                       (0x02C)
 #define I2C_MCR2_OFFSET                        (0x038)


 /*************************  I2C SLAVE ****************************/
 #define I2C_SOAR_OFFSET                         (0x800)
 #define I2C_SCSR_OFFSET                         (0x804)
 #define I2C_SDR_OFFSET                          (0x808)
 #define I2C_SIMR_OFFSET                         (0x80C)
 #define I2C_SRIS_OFFSET                         (0x810)
 #define I2C_SMIS_OFFSET                         (0x814)
 #define I2C_SICR_OFFSET                         (0x818)
 #define I2C_SOAR2_OFFSET                        (0x81C)
 #define I2C_SACKCTL_OFFSET                      (0x820)
 /*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						ADDRESSES					              *
 ******************************************************************************
 */
 /************************************MASTER****************************/
  #define I2CRCGC                            *((I2C_RegisterAddressType) (RCC_BASE1 +RCC_I2C_offset ))

 #define I2CMSA(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MSA_OFFSET))
 #define I2CMCS(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MCS_OFFSET))
 #define I2CMDR(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MDR_OFFSET))
 #define I2CMTPR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MTPR_OFFSET))
 #define I2CMIMR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MIMR_OFFSET))
 #define I2CMRIS(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MRIS_OFFSET))
 #define I2CMMIS(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MMIS_OFFSET))
 #define I2CMICR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MICR_OFFSET))
 #define I2CMCR(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MCR_OFFSET))
 #define I2CMCLKOCNT(PERIPHERAL_ID)        *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MCLKOCNT_OFFSET))
 #define I2CMBON(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MBON_OFFSET))
 #define I2CMCR2(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_MCR2_OFFSET))

/************************************SLAVE****************************/
 #define I2CSOAR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SOAR_OFFSET))
 #define I2CSCSR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SCSR_OFFSET))
 #define I2CSDR(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SDR_OFFSET))
 #define I2CSIMR(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SIMR_OFFSET))
 #define I2CSRIS(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SRIS_OFFSET))
 #define I2CSMIS(PERIPHERAL_ID)            *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SMIS_OFFSET))
 #define I2CICR(PERIPHERAL_ID)             *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SICR_OFFSET))
 #define I2CSOAR2(PERIPHERAL_ID)           *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SOAR2_OFFSET))
 #define I2CSACKCTL(PERIPHERAL_ID)         *((I2C_RegisterAddressType) (I2C_BASE+(0x1000*PERIPHERAL_ID)+I2C_SACKCTL_OFFSET))

 /*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						BIT POSITION				              *
 *                                                                            *
 *                                                                            *
 ******************************************************************************
*/
#define  I2C_Slave_Addr_POS                         (1U)
#define  I2C_R_W_POS                                (0)
#define  I2C_HS_POS                                 (4U)
#define  I2C_HS_SCL_POS                             (7u)
/*
******************************************************************************
*                                                                            *
*                                                                            *
*                                  MACRROS                                   *
******************************************************************************/


#define I2C_SlaveADD_send(I2C_ID,Slave_add,ReadorWrite)           (I2CMSA(I2C_ID) = ReadorWrite << 0);\
                                                                 (I2CMSA(I2C_ID) |= Slave_add << 1)
#define I2C_WRITE_DATA(I2C_ID,Data)                               (I2CMDR(I2C_ID) = Data)
#define I2C_READ_DATA(I2C_ID,Data)                                (*Data++ = I2CMDR(I2C_ID))
#define I2C_MasterCTL(I2C_ID,operation)                           (I2CMCS(I2C_ID) = operation)

typedef enum
{
			I2C_NOK=0,
			I2C_OK=1,
			I2C_SINGLENOK=2,
			I2C_SINGLEOK=3,
			I2C_BURSTOK=4,
			I2C_BURSTNOK=5,
I2C_READDATAOK=6,
I2C_BURST_READOK=7,
I2C_SREAD_NOK=8,
I2C_SREAD_OK=9,
I2C_NOBUSY=10,
I2C_BUSY=11
}I2C_CheckType;
typedef enum
{
    SINGLE_OP=0x00000007,
    BURST_SEND_START=0x00000003,
    BURST_SEND_CONT =0x00000001,
    BURST_DONE=0x00000005,
    BURST_RECEIVE_START=0x0000000b,
    BURST_RECEIVE_CONT=0x00000009,
    BURST_RECEIVE_ERROR_STOP=0x00000004
}Ops;
typedef enum
{
	  I2C_SINGLESEND=0,
    I2C_BURSTSEND_STAT=1,
    MASTER_BUSY=2,
    I2C_BUSBUSY=3,
    I2C_MBUSY=5

	 
}I2C_Checkstatus;

typedef struct
{
    uint8_t PortID;
    uint8_t SDA_Pin;
    uint8_t SCK_Pin;
    uint32_t Clock;
    uint8_t Enable;
}I2C_Mode;
typedef enum {

	I2C_Transmit=0,
	I2C_Receive=1

}I2C_Receive_transmit;
typedef enum
{
	I2C_Busy=0,//WHEN SET ALL OTHER STATUS BITS ARE NOT VALID
	I2C_ERROR=1,
	I2C_ADDR_ACK=2,
	I2C_DATA_ACK=3,
	I2C_ARBIR_LST=4,
	I2C_IDEL=5,
	I2C_BUS_BUSY=6,//CHANGE BASED ON START TOP BIT
	I2C_CLK_TO=7
}I2C_STATUS;

typedef enum {

	I2C_START_GEN=0x00000007,
	I2C_STOP_GEN=0x00000005
}I2C_Control;
typedef enum {
	I2C_RUN_EN=0,
	I2C_START=1,
	I2C_STOP=2,
	I2C_ACK=3,
	I2C_HS_EN=4

}I2C_position;
typedef enum
{
	I2C_0 = 0,
	I2C_1 = 1,
	I2C_2 = 2,
	I2C_3 = 3
}Peripheral_ID;
typedef enum
{
	PERIPHERAL_DISABLE=0,
	PERIPHERAL_ENABLE=1
}PeripheralEnable;
typedef enum 
{
	MASTER=0x10,
	SLAVE=0x20
}Type;

typedef enum
{
	STANDARD_MODE=0,
	FAST_MODE=1
}SpeedMode;

typedef enum
{
		I2C_RIS_NSET=0,
	    I2C_RIS_SET=1
}MASK;
typedef enum
{
	DISABLE_GENERAL_CALL=0,
	ENABLE_GENERAL_CALL=1
}GeneralCall;

typedef void (*I2C_CallBackFunctionType)(void);

typedef enum
{
	
	PERIPHERAL_CLOCK_16MHz=0x07,
	
	PERIPHERAL_CLOCK_20MHz=0x09
	
}PeripheralFrequency;//for standardmode


typedef struct
{
	uint8_t							   I2C_GPIO_Structure_ID;
	/*used to choose from I2Cx,  x=1,2,3*/
	Peripheral_ID					   I2C_Peripheral_ID;
	
	uint32_t						   I2C_FrequencyOf_SCL;
		
	PeripheralFrequency			       I2C_PeripheralFrequency;
	/**/
	Type                               I2C_TYPE;
	
	SpeedMode					       I2C_SpeedMode;
	/**/
	GeneralCall				           I2C_GeneralCall;
	/**/
	I2C_CallBackFunctionType			I2C_TransmissionDoneCallBack;

	I2C_CallBackFunctionType			I2C_ReceptionDoneCallBack;

}I2C_ConfigType;

extern const I2C_ConfigType I2C_ConfigParam[I2C_PERIPHERAL_NUMBER];
extern uint8_t I2C_InitFlag;
/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            				Functions Prototypes				              *
 ******************************************************************************
 */

I2C_CheckType I2C_Init(void);
extern uint8_t I2C_TX(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data,uint8_t datacount);

extern void I2C_GenerateStart(uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_StartStatus(uint8_t Peripheral_ID);

extern void I2C_SendSlaveAddress(uint8_t SlaveAddress,uint8_t WriteOrRead, uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_SendSlaveAddressStatus(uint8_t Peripheral_ID);
extern void I2C_Clear_ADDR(uint8_t Peripheral_ID);

extern void I2C_SendData(uint8_t* DataPtr, uint8_t Peripheral_ID);
//extern I2C_CheckType I2CSendDataStatus(uint8_t Peripheral_ID);

extern void I2C_GetData(uint8_t *Data, uint8_t Peripheral_ID);
//extern I2C_CheckType I2C_GetDataStatus(uint8_t Peripheral_ID);
extern I2C_CheckType I2C_CHECKSTATUS(uint8_t Peripheral_ID,I2C_Checkstatus Status);
extern void I2C_GenerateStop(uint8_t Peripheral_ID);



extern void I2C_TurnOffAcknowledge(uint8_t Peripheral_ID);
extern void I2C_TurnOnAcknowledge(uint8_t Peripheral_ID);

#endif
