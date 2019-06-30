#include "GPIO.h"
#include "GPIO_Cfg.h"
const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER] =
{
 {
  //digital button 1 (group 0)
  0x05,1<<0,
                     0,0,0,
                     0x00,
                     0xff,0x00,0x00,
                     0x00,0x00,0x00,
                     0xff,
                     0


 },
 {
  //digital button 2 (group 1)
  0x05,1<<4,
                     0,0,0,
                     0x00,
                     0xff,0x00,0x00,
                     0x00,0x00,0x00,
                     0xff,
                     0


 },

 {           //led 1(group 2)
             0x05,1<<2,
             0,0,0,
             0xff,
             0x00,0x00,0x00,
             0xff,0x00,0x00,
             0xff,
             0
 },
 {                       // led 2(group 3)
                         0x05,1<<1,
                         0,0,0,
                         0xff,
                         0x00,0x00,0x00,
                         0xff,0x00,0x00,
                         0xff,
                         0
 },



 {           //can rx (group 4)
             0x01,1<<4,
             0xff,0,0,
             0x00,
             0x00,0x00,0x00,
             0x00,0x00,0x00,
             0xff,
             0
 },
 {           //can tx (group 5)
             0x01,1<<5,
             0xff,0,0,
             0xff,
             0x00,0x00,0x00,
             0xff,0x00,0x00,
             0xff,
             0},

             {
                 // analog button 1  Group 6 PE3
                 0x04,1<<3,
                         0,0,0,
                         0xFF,
                         0x00,0x00,0x00,
                         0x00,0x00,0x00,
                         0x00,
                         0


             }
             ,
             {// analog button 2   Group 7 PE2
              0x04,1<<2,
              0,0,0,
              0xFF,
              0x00,0x00,0x00,
              0x00,0x00,0x00,
              0x00,
              0
             }
             ,
             {// analog button 3  Group 8 PE1
              0x04,1<<1,
              0,0,0,
              0xFF,
              0x00,0x00,0x00,
              0x00,0x00,0x00,
              0x00,
              0
             },

             {
              // analog button 4  Group 9 PE0
              0x04,1<<0,
              0,0,0,
              0xFF,
              0x00,0x00,0x00,
              0x00,0x00,0x00,
              0x00,
              0
             },

             {       // PWM 1 Group 10 (left)
                     0x01, 1<<6,
                     0xff,0,0,
                     0xff,
                     0x00,0x00,0x00,
                     0x00,0x00,0xff,
                     0xff,
                     0


             },
             {       // PWM  2 Group 11 (right)
                     0x02, 1<<4,
                     0xff,0,0,
                     0xff,
                     0x00,0x00,0x00,
                     0x00,0x00,0xff,
                     0xff,
                     0

             }



 };
