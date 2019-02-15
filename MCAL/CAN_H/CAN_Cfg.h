#ifndef CAN_CFG_H_
#define CAN_CFG_H_

#define CAN_GROUPS_NUMBER       1u
#define Message_Groups_Number   2u


typedef enum {Normal_Mode = 1, Test_Mode = 2} CAN_MODETYP;
typedef enum {Basic_Mode = 0x04, Silent_Mode = 0x08,Loopback_Mode=0x10} CAN_MODESELEC;
#define Mode_Type                   Test_Mode

#define Mode_Selector               Loopback_Mode

#endif
