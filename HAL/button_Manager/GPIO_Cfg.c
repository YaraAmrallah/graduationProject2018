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
                    0x04,1<<1,
                    0,0,0,
                    0x00,
                    0x00,0x00,0x00,
                    0x00,0x00,0x00,
                    0x00,
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



                 {           //led 3 (group 4)
                                0x05,1<<3,
                                0,0,0,
                                0xff,
                                0x00,0x00,0x00,
                                0xff,0x00,0x00,
                                0xff,
                                0
                                                },
                {           //Pin A0 (group 5) Uart0 RX
                                            0x00,1<<0,
                                            0xff,0,0,
                                            0x00,
                                            0x00,0x00,0x00,
                                            0x00,0x00,0x00,
                                            0xff,
                                            0
                                }

        };
