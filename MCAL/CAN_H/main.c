#include "GPIO.h"
#include "GPIO_Cfg.h"
#include "CAN.h"
#include "CAN_Cfg.h"
#include "CANManager.h"
#include "CANManager_Cfg.h"
#include <stdint.h>


int main(void)

{
  //  uint64_t Data;
  /*int64_t* p;*/
    CANMgr_CheckType x;
  //  p = &Data;

    GPIO_Init();
    CAN_Init();
    CAN_Write(0, 0x0a);
            CAN_Write(1, 0x0a);
            do
            {
           x =  CAN_Buffer(0);
            }while(x == CANMgr_BUSY);
    while(1)
    {
        CAN_checkstatus();



    }
	return 0;
}


void Callbkfn1(void)
{
    uint8_t x = 0;
    x++;
}

void Callbkfn2(void)
{
    uint8_t y = 0;
    y++;
}
