
#ifndef I2C2_H_
#define I2C2_H_
#include <stdint.h>


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
/*Macros*/
#define READ 1
#define WRITE 0
 /*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *            						MACRROS					                  *
 ******************************************************************************/


#define I2C_SlaveADD_send(I2C_ID,Slave_add,ReadorWrite) 		  (I2CMSA(I2C_ID) = ReadorWrite << 0);\
																  (I2CMSA(I2C_ID) |= Slave_add << 1)
#define I2C_WRITE_DATA(I2C_ID,Data)       						  (I2CMDR(I2C_ID) = Data)
#define I2C_READ_DATA(I2C_ID,Data)        						  (*Data++ = I2CMDR(I2C_ID))
#define I2C_MasterCTL(I2C_ID,operation)  	  					  (I2CMCS(I2C_ID) = operation)


typedef enum 
{
	MASTER=0x10,
	SLAVE=0x20
}Type;
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
typedef struct
{
    uint8_t PortID;
    uint8_t SDA_Pin;
    uint8_t SCK_Pin;
    uint32_t Clock;
    uint8_t Enable;
}I2C_Mode;

void I2C_Init2(void);
uint8_t I2C_Send(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data);
uint8_t I2C_Receive(uint8_t I2CID, uint8_t Slave_address, uint8_t Datacount, uint8_t* Data);
uint8_t I2C_Quick_Receive(uint8_t I2CID, uint8_t Slave_address, uint8_t* Data);
uint8_t TPR_Calculator(uint32_t Clk);

#endif /* I2C_H_ */
