

#include "PLL.h"

#define RCC2      			    (*((volatile unsigned long *)0x400FE070))
#define RCC            			(*((volatile unsigned long *)0x400FE060))
#define MOSCCTL            		(*((volatile unsigned long *)0x400FE07C))
#define PLLFREQ0            	(*((volatile unsigned long *)0x400FE032))
#define PLLFREQ1            	(*((volatile unsigned long *)0x400FE164))
#define PLLSTAT            		(*((volatile unsigned long *)0x400FE168))
#define RIS         	   		(*((volatile unsigned long *)0x400FE050))

void PLL_Set80(void)
{
	  RCC2 |= 0x80000000;
	  RCC2 |= 0x00000800;
	  RCC &= ~0x000007C0;
	  RCC += 0x00000540;
	  RCC2 &= ~0x00000070;
	  RCC2 += 0x00000000;
	  RCC2 &= ~0x00002000;
	  RCC2 |= 0x40000000;
	  RCC2 = (RCC2&~0x1FC00000)
	                  + (4<<22);
	  while((RIS&0x00000040)==0){};
	  RCC2 &= ~0x00000800;
}
void PLL_Set16(void)
{

RCC2 |= 0x80000000;
	  RCC2 |= 0x00000800;
	  RCC &= ~0x000007C0;
	  RCC += 0x00000540;
	  RCC2 &= ~0x00000070;
	  RCC2 += 0x00000000;
	  RCC2 &= ~0x00002000;
	  RCC2 |= 0x40000000;
	  RCC2 = (RCC2&~0x1FC00000)
	                  + (24<<22);
	  while((RIS&0x00000040)==0){};
	  RCC2 &= ~0x00000800;
}