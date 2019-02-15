#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "CAN_Cfg.h"
typedef volatile uint32_t* const CAN_RegAddType;

#define CAN_NUMBER 2u
#define CAN_0BaseAddress 0x40040000
#define CAN_1BaseAddress 0x40041000

static const uint32_t CAN_BaseAddressLut[CAN_NUMBER] =
{
     CAN_0BaseAddress,
     CAN_1BaseAddress
};

/* Defining Clock Gating Control register 0 */
#define RCGC0_CAN                           *((CAN_RegAddType)(0x400FE100))

/* Defining Control Registers */
#define CAN_CTL(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x000))
#define CAN_STS(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x004))
#define CAN_ERR(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x008))
#define CAN_BIT(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x00C))
#define CAN_INT(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x010))
#define CAN_TST(CAN_ID)                  *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x014))
#define CAN_BRPE(CAN_ID)                 *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x018))

/* Defining CAN_ TX Interface Registers */
#define CAN_IF1CRQ(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x020))
#define CAN_IF1CMSK(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x024))
#define CAN_IF1CMSK1(CAN_ID)             *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x028))
#define CAN_IF1CMSK2(CAN_ID)             *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x02C))
#define CAN_IF1ARB1(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x030))
#define CAN_IF1ARB2(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x034))
#define CAN_IF1MCTL(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x038))
#define CAN_IF1DA1(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x03C))
#define CAN_IF1DA2(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x040))
#define CAN_IF1DB1(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x044))
#define CAN_IF1DB2(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x048))

/* Defining CAN_ RX Interface Registers */
#define CAN_IF2CRQ(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x080))
#define CAN_IF2CMSK(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x084))
#define CAN_IF2MSK1(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x088))
#define CAN_IF2MSK2(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x08C))
#define CAN_IF2ARB1(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x090))
#define CAN_IF2ARB2(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x094))
#define CAN_IF2MCTL(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x098))
#define CAN_IF2DA1(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x09C))
#define CAN_IF2DA2(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x0A0))
#define CAN_IF2DB1(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x0A4))
#define CAN_IF2DB2(CAN_ID)               *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x0A8))

/* Defining CAN_ TX Message Object Registers */
#define CAN_TXRQ1(CAN_ID)                *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x100))
#define CAN_NWDA1(CAN_ID)                *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x120))
#define CAN_MSG1INT(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x140))
#define CAN_MSG1VAL(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x160))

/* Defining CAN_ RX Message Object Registers */
#define CAN_TXRQ2(CAN_ID)                *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x104))
#define CAN_NWDA2(CAN_ID)                *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x124))
#define CAN_MSG2INT(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x144))
#define CAN_MSG2VAL(CAN_ID)              *((CAN_RegAddType)(CAN_BaseAddressLut[CAN_ID] + 0x164))


/* Registers Initializations */
#define CAN_0_EN                         (1<<24)

#define CAN_CTL_INIT                     (0x01)
#define CAN_CTL_TEST                     (0x80)
#define CAN_CTL_CCE                      (0x40)
#define CAN_CTL_DAR                      (0x20)
#define CAN_CTL_IE                       (0x02)

#define CAN_BIT_TSEG2_MASK               (12)
#define CAN_BIT_TSEG2_EN                 (0x07)
#define CAN_BIT_TSEG1_MASK               (8)
#define CAN_BIT_TSEG1_EN                 (0x0F)
#define CAN_BIT_SJW_MASK                 (6)
#define CAN_BIT_SJW_EN                   (0x03)
#define CAN_BIT_BRP_EN                   (0x03F)

#define CAN_BRPE_EN                      (0x3C0)


#define CAN_IFnCRQ_Busy                  (0x8000)

#define CAN_IFnCMSK_WRNRD                (0x80)
#define CAN_IFnCMSK_MASK                 (0x40)
#define CAN_IFnCMSK_ARB                  (0x20)
#define CAN_IFnCMSK_CONTROL              (0x10)
#define CAN_IFnCMSK_DATA_EN              (0x03)

#define CAN_IFnARB2_IDE_MASK             (14)
#define CAN_IFnARB2_DIR                  (0x2000)
#define CAN_IFnARB2_MSGVAL               (0x8000)
#define CAN_IFnARB2_ID_STANDARD_MASK     (2)
#define CAN_IFnARB2_ID_STANDARD_EN       (0x7FF)
#define CAN_IFnARB2_ID_EXTENDED_MASK     (16)
#define CAN_IFnARB2_ID_EXTENDED_EN       (0x1FFF0000)
#define CAN_IFnARB1_ID_EXTENDED_EN       (0xFFFF)

#define CAN_IFnMCTL_TXRQST               (0x100)
#define CAN_IFnMCTL_UMASK                (0x1000)
#define CAN_IFnMCTL_EOB                  (0x80)


#define CAN_STS_BOFF_MASK                (7)
#define CAN_STS_BOFF_EN                  (0x80)
#define CAN_STS_TXOK_MASK                (3)
#define CAN_STS_TXOK_EN                  (0x08)
#define CAN_STS_RXOK_MASK                (4)
#define CAN_STS_RXOK_EN                  (0x10)

typedef struct
{
    uint8_t CAN_Id;  /* To choose CAN_0 or CAN_1 */
    uint8_t TSEG2;  /* Phase 2 */
    uint8_t TSEG1;  /* Propagation + Phase 1 */
    uint8_t SJW;    /* Synchronization Jump Width */
    uint16_t BRP;   /* Baud Rate Pre-scalar */
    uint8_t InterruptEnable;

}CAN_CfgType;

typedef struct
{
    uint32_t ID;
    uint8_t RTR;
    uint8_t IDE;
    uint8_t DLC;
    uint8_t ModuleType;
}MessageParameters;



typedef enum {CAN_NOK = 0, CAN_OK = 1,  CAN_Busy = 2} CAN_CheckType;
typedef enum {BusOff = 0, TxDone = 1, TxInprogress = 2, RxDone = 3, RxInprogress = 4,InitialState = 5 } Status_CheckType;

extern const CAN_CfgType CAN_ConfigParam [CAN_GROUPS_NUMBER];
extern const MessageParameters MSG_ConfigParam [Message_Groups_Number];

CAN_CheckType CAN_Init(void);
CAN_CheckType CAN_TX(uint8_t ChannelID, uint8_t* DataPtr, uint8_t Module);
CAN_CheckType CAN_RX(uint8_t ChannelID, uint16_t* DataPtr, uint8_t Module);
Status_CheckType CAN_Status(uint8_t ChannelID, uint8_t MessagePriority);
static CAN_CheckType CAN_TxInit(uint8_t Module);
static CAN_CheckType CAN_RxInit(uint8_t Module);

static uint8_t TxStart = 0;

void CAN_StopCurrentObject(uint8_t ChannelID);

#endif
