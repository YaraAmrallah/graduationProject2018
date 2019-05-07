/**Done**/
#ifndef I2C2_CFG_H_
#define I2C2_CFG_H_


#include "I2C2.h"
#define I2CMODS_NUM 4u
#define I2C_PCTL_VAL 3u
#define Clk 400000
#define I2Cs_NUM 1u /*Number of I2Cs you need*/
const I2C_Mode I2CMOD_Data[I2CMODS_NUM]=
{
  {(1),(3),(2),Clk,1} /*****I2C 0*****/
 ,{(0),(7),(6),Clk,0} /*****I2C 1*****/
 ,{(4),(5),(4),Clk,0} /*****I2C 2*****/
 ,{(3),(1),(0),Clk,0} /*****I2C 3*****/
};



#endif /* I2C_CFG_H_ */
