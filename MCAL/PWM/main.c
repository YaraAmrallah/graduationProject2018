



/**
 * main.c
 */
#include "GPIO.h"
#include "GPIO_Cfg.h"
#include "PWM.h"
#include "PWM_Cfg.h"
#include "M4MemMap.h"
#include "servo.h"
#include <stdint.h>

int main(void)
{
	// btn_1 angle up.
	// btn_2 angle down.
	uint8_t btn_1=1,btn_2=1;
	uint8_t pre_ang_1=90;
	uint16_t i =0 ;
    GPIO_Init();
   GPIO_SetAlternFuntion(0,4);
    PWM_Init ();
  // Initiate_Servo(0);

 //  Rotate_Servo(0,170);
   while (1)
  {
	   GPIO_Read(1,&btn_1);
	   GPIO_Read(2,&btn_2);

	   if(btn_1 == 0)
	   {
		   AngleUp_Servo(0, pre_ang_1, &pre_ang_1);
		   GPIO_Write(3,0xff);
	   }
	   else if(btn_2 == 0)
	   {
		   AngleDown_Servo(0, pre_ang_1, &pre_ang_1);
		   GPIO_Write(3,0x00);
	   }
	   else
	   {
		   // do nothing
	   }
	   for(i=0 ; i<20000 ;i++);
   }

}

