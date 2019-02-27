#include "GPIO.h"
#include "GPIO_Cfg.h"
const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER] =
        {
                {       // PWM Group
                        0x01, 1<<6,
                        0xff,0,0,
                        0xff,
                        0x00,0x00,0x00,
                        0x00,0x00,0xff,
                        0xff,
                        0


                },
                      {
                          //button 1(group 1)
                    0x05,1<<0,
                    0,0,0,
                    0x00,
                    0xff,0x00,0x00,
                    0x00,0x00,0x00,
                    0xff,
                    0


                     },

                    {           //button 2(group 2)
                                0x05,1<<4,
                                0,0,0,
                                0x00,
                                0xff,0x00,0x00,
                                0x00,0x00,0x00,
                                0xff,
                                0
                    },
                    {

                      0x05,1<<2,
                      0,0,0,
                      0xff,
                      0x00,0x00,0x00,
                      0x00,0x00,0x00,
                      0xff,
                      0


                       }

        };
