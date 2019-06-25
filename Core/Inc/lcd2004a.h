#ifndef LCD_H_
#define LCD_H_
//------------------------------------------------
#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
//------------------------------------------------
//portIOLCDAdapter - output of PCF8574.
#define e_set() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter|=0x04)  //Set line  Е in 1
#define e_reset() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter&=~0x04) //Set line Е in 0
#define rs_set() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter|=0x01) //Set line RS in 1
#define rs_reset() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter&=~0x01) //Set line RS in 0
#define setled() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter|=0x08) //Set line RS in 1
#define setwrite() LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter&=~0x02) //Set line RS in 0
//------------------------------------------------

//It is necessary in case the display is not one, but several at different addresses and ports.
struct communicationParameters{
	I2C_HandleTypeDef* i2cPortMaster;
	uint8_t adressSlave;
};

//Sending a byte to the display if IsData == 1 then a symbol will be printed if IsData == 0 then the command is a display.
void SendByte(struct communicationParameters CommParam, uint8_t ByteToSend, uint8_t IsData);
//Sending a byty of i2c.
void LCD_WriteByteI2CLCD(struct communicationParameters CommParam, uint8_t bt);
//Send a string to the screen.
void LCD_String(struct communicationParameters CommParam, char* st);
//Send a char to the screen.
void LCD_SendChar(struct communicationParameters CommParam, char ch);
//Cleaning the display.
void LCD_Clear(struct communicationParameters CommParam);
//Return home - the cursor to the begining.
void LCD_ReturnHome(struct communicationParameters CommParam);
//Turn the display on, the cursor is off, the cursor does not blink.
void LCD_On(struct communicationParameters CommParam);
//Shift the cursor to the left.
void LCD_ShiftCursorToLeft(struct communicationParameters CommParam);
//Shift the cursor to the right.
void LCD_ShiftCursorToRight(struct communicationParameters CommParam);
//Shift the display to the left.
void LCD_ShiftLCDToLeft(struct communicationParameters CommParam);
//Shift the display to the right.
void LCD_ShiftLCDToRight(struct communicationParameters CommParam);
//4-bit, 2 lines, font small/
void LCD_FunctionSet(struct communicationParameters CommParam);
//The cursor moves each time one character forward.
void LCD_EntyModeSet(struct communicationParameters CommParam);
//Initialization of the LCD module. 4 bits, 2 string, symbols 5*8.
void LCD_ini(struct communicationParameters CommParam);
//Set the cursor (Line number, string number).
void LCD_SetCursor(struct communicationParameters CommParam, uint8_t str, uint8_t num);
//The square root sign.
void LCD_PrintSQRT(struct communicationParameters CommParam);
//------------------------------------------------
#endif /* LCD_H_ */
