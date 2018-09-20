#include "LCD_int.h"
#include "LCD_cnfg.h"
#define GET_BIT(data,bit_num) ((data >> bit_num)&1)
const uint8_t Data_uint8_tPins[]={LCD_D0,LCD_D1,LCD_D2,LCD_D3,LCD_D4,LCD_D5,LCD_D6,LCD_D7};


static void _delay_ms(unsigned long halfsecs){
  unsigned long count;
  
  while(halfsecs > 0 ) { // repeat while there are still halfsecs to delay
    //count = 1538460; // 400000*0.5/0.13 that it takes 0.13 sec to count down to zero (.5 sec)
		count = 10000; // 3ms (80MHz)
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    halfsecs--;
  }
}


LCD_CheckType LCD_Init8DataMode(void)
{

	// Indicator function finished successfully
	LCD_CheckType FuncErrValidation=LCD_OK;
	_delay_ms(50);
	 LCD_WriteCmd(60);
	_delay_ms(2);

	 LCD_WriteCmd(15);
	_delay_ms(2);

	 LCD_WriteCmd(1);
	_delay_ms(2);


	return FuncErrValidation;
}


void LCD_PrivateSetDataPinVal(uint8_t Copy_Data)
{
	uint8_t Bit_Number,Data_Val,LCD_Data;
	for(Bit_Number=0;Bit_Number<8;Bit_Number++)
	{
		Data_Val=GET_BIT(Copy_Data,Bit_Number);
		if(Data_Val == 1) LCD_Data=LCD_HIGH;
		else LCD_Data=LCD_LOW;
		GPIO_SetGroupValue(Data_uint8_tPins[Bit_Number],LCD_Data);
	}
}


/**
  * This function used in write commands to LCD
  * Input: The Command Supposed to be written
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_WriteCmd(uint8_t Copy_Cmd)
{

	LCD_CheckType FuncErrValidation=LCD_OK;
	GPIO_SetGroupValue(LCD_RS,LCD_LOW);

	// Dividing the Cmd To the Pins
	LCD_PrivateSetDataPinVal(Copy_Cmd);

	// Trigger the enable pins to read the command
	GPIO_SetGroupValue(LCD_EN,LCD_HIGH);
	_delay_ms(2);
	GPIO_SetGroupValue(LCD_EN,LCD_LOW);

	return FuncErrValidation;
}


/**
  * This Function used in write one byte to LCD
  * Input: The Byte supposed to be written
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_WriteByte(uint8_t Copy_Data)
{

	LCD_CheckType FuncErrValidation=LCD_OK;
	GPIO_SetGroupValue(LCD_RS,LCD_HIGH);

	// Dividing the Cmd To the Pins
	LCD_PrivateSetDataPinVal(Copy_Data);

	// Trigger the enable pins to read the command
	GPIO_SetGroupValue(LCD_EN,LCD_HIGH);
	_delay_ms(2);
	GPIO_SetGroupValue(LCD_EN,LCD_LOW);

	return FuncErrValidation;

}





/**
  * This Function used in moving between rows and columns
  * Input: Rows {0,1}.., Column [0:15]
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_GoToX_Y(uint8_t LCD_Row,uint8_t LCD_Column)
{
	uint8_t FuncErrorVaidation;
	if(LCD_Row < 0 || LCD_Row > 1) FuncErrorVaidation = LCD_NOK;
	else if(LCD_Column < 0 || LCD_Column > 15) FuncErrorVaidation = LCD_OK;
	else
	{
		FuncErrorVaidation = LCD_OK;
		switch(LCD_Row)
		{
			case 0:
				LCD_WriteCmd(128 + LCD_Column);
				break;
			case 1:
				LCD_WriteCmd(192 + LCD_Column);
				break;
		}
	}
	return FuncErrorVaidation;
}


/**
 * This Function Writes string to the LCD
 * Input: Array of chars
 * maximum size of array of chars is 2^64-1
 * Return 0 Completed successfully
 * Return 1 unsuccessful
 */
LCD_CheckType LCD_WriteString(uint8_t LCD_Chars[])
{

	uint8_t LCD_CharsIdx=0;
	LCD_CheckType FuncErrorValid=LCD_OK;
	while(LCD_Chars[LCD_CharsIdx] != '\0')
	{
		LCD_WriteByte(LCD_Chars[LCD_CharsIdx++]);
	}

	return FuncErrorValid;
}


