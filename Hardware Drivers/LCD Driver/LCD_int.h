#ifndef LCD_INT_H
#define LCD_INT_H
#include <stdint.h>

#include "GPIO.h"
#define LCD_HIGH 0xFF
#define LCD_LOW 0x00
#define CLEAR_DISPLAY 0b00000001
#define CURSOR_LEFT 0b00000100
#define CURSOR_RIGHT 0b00000110
#define SHIFT_DISPLAY_R 0b00000101
#define SHIFT_DISPLAY_L 0b00000111


typedef enum{LCD_OK=0,LCD_NOK} LCD_CheckType;


/**
  * This Function used in initialize the LCD for eight data mode
  */
LCD_CheckType LCD_Init8DataMode(void);

/**
  * This function used in write commands to LCD
  * Input: The Command Supposed to be written
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_WriteCmd(uint8_t Copy_Cmd);


/**
  * This Function used in write one byte to LCD
  * Input: The Byte supposed to be written
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_WriteByte(uint8_t Copy_Data);

/**
  * This Function used in moving between rows and columns
  * Input: Rows {0,1}.., Column [0:15]
  * Return 0 Complete Successfully
  * Return 1 Completed Unsuccessfully
  */
LCD_CheckType LCD_GoToX_Y(uint8_t LCD_Row,uint8_t LCD_Column);


/**
 * This Function Writes string to the LCD
 * Input: Array of chars
 * Return 0 Completed successfully
 * Return 1 unsuccessful
 */
LCD_CheckType LCD_WriteString(uint8_t LCD_Chars[]);



#endif
