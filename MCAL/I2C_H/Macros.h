/*
 * Macros.h
 *
 *      Author: hussienmohamed
 */

#ifndef MACROS_H_
#define MACROS_H_


/***********************************************************************************************************************/

typedef volatile uint32_t* const I2C_RegAddType;

#define MAX_SCL_FREQUENCY_SM        (100000U)
#define MAX_SCL_FREQUENCY_FM        (400000U)
#define I2C_DELAY_PARAMETER         (50U)
typedef volatile uint32_t* const I2C_RegisterAddressType;

#define I2C_MAXIMUM_NUMBER                      (4U)
/*
 ******************************************************************************
 *                                                                            *
 *                                  BASES                                     *                                                                        *
 ******************************************************************************
 */

 #define PERIPHERAL_BASE                                (0x40020000U)//I2C0
 #define RCC_BASE1                                      (0x400FE000U)
 #define RCC_I2C_offset                                 (0x620)
 #define I2C_BASE                                       PERIPHERAL_BASE


 /*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                                  OFFSETS                                   *
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
 *                                  ADDRESSES                                 *
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
 *                                  BIT POSITION                              *
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
#define I2C_READ_DATA(I2C_ID,Data)                                (*(Data++) = I2CMDR(I2C_ID))
#define I2C_MasterCTL(I2C_ID,operation)                           (I2CMCS(I2C_ID) = operation)


#endif /* MACROS_H_ */
