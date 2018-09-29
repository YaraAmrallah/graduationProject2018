#include "GPIO.h"
#include "GPIO_Cfg.h"
const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER] =
		{
				{       //red led(group 0)
						0x05,1<<1,
						0,0,0,
						0xff,
						0x00,0xff,0x00,
						0xff,0x00,0x00,
						0xff,
						0


				},
				      {
				          //button 1(group 1)
					0x05,1<<0,
					0,0,0,
					0x00,
					0x00,0x00,0x00,
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
				{                       //blue led(group 3)
				                        0x05,1<<2,
				                        0,0,0,
				                        0xff,
				                        0x00,0xff,0x00,
				                        0xff,0x00,0x00,
				                        0xff,
				                        0
				                },

			    {           //Pin A0 (group 4) Uart0 RX
			                                0x00,1<<0,
			                                1,0,0,
			                                0x00,
			                                0xff,0x00,0x00,
			                                0x00,0x00,0x00,
			                                0xff,
			                                0
			                    },

			     {           //Pin A1 (group 5) UART0 TX
			                    0x00,1<<1,
			                    1,0,0,
			                    0x00,
			                    0xff,0x00,0x00,
			                    0x00,0x00,0x00,
			                    0xff,
			                    0
			                                    }

		};
