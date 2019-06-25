#include "lcd2004a.h"

uint8_t buf[1]={0};

uint8_t portIOLCDAdapter; //The cell for storing the data of the port of the expansion chip.
void LCD_WriteByteI2CLCD(struct communicationParameters CommParam, uint8_t bt)
{
	buf[0]=bt;
	HAL_I2C_Master_Transmit(CommParam.i2cPortMaster,(uint16_t) (CommParam.adressSlave)<<1, buf, 1, 1000);
}

void SendHalfByte(struct communicationParameters CommParam, uint8_t c)
{
	c<<=4;
	e_set();
	LCD_WriteByteI2CLCD(CommParam, portIOLCDAdapter|c);
	e_reset();
}

void SendByte(struct communicationParameters CommParam, uint8_t ByteToSend, uint8_t IsData)
{
	if(IsData==0) rs_reset();
	else rs_set();
	uint8_t hc=0;
	hc=ByteToSend>>4;
	SendHalfByte(CommParam, hc);SendHalfByte(CommParam, ByteToSend);
}

void LCD_String(struct communicationParameters CommParam, char* st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		SendByte(CommParam, st[i], 1);
		i++;
	}
}

void LCD_SendChar(struct communicationParameters CommParam, char ch)
{
	SendByte(CommParam, ch, 1);
}

void LCD_Clear(struct communicationParameters CommParam){//Clear display and input first
	SendByte(CommParam, 0x1,0);
    HAL_Delay(1);
}

void LCD_ReturnHome(struct communicationParameters CommParam){
	SendByte(CommParam, 0x2,0);
}

void LCD_On(struct communicationParameters CommParam){
	SendByte(CommParam, 0xC,0);
}

void LCD_ShiftCursorToLeft(struct communicationParameters CommParam){
	SendByte(CommParam, 0x10,0);
}

void LCD_ShiftCursorToRight(struct communicationParameters CommParam){
	SendByte(CommParam, 0x14,0);
}

void LCD_ShiftLCDToLeft(struct communicationParameters CommParam){
	SendByte(CommParam, 0x18, 0);
}

void LCD_ShiftLCDToRight(struct communicationParameters CommParam){
	SendByte(CommParam, 0x1C, 0);
}

void LCD_FunctionSet(struct communicationParameters CommParam){
	SendByte(CommParam, 0x28, 0);
}

void LCD_EntyModeSet(struct communicationParameters CommParam){
	SendByte(CommParam, 0x6, 0);
}

void LCD_ini(struct communicationParameters CommParam)
{
	HAL_Delay(15);
	SendHalfByte(CommParam, 0x03);
	HAL_Delay(4);
	SendHalfByte(CommParam, 0x03);
	HAL_Delay(1);
	SendHalfByte(CommParam, 0x03);
	HAL_Delay(1);
	SendHalfByte(CommParam, 0x02);
	HAL_Delay(1);
	LCD_FunctionSet(CommParam);
	HAL_Delay(1);
	LCD_On(CommParam);
	HAL_Delay(1);
    LCD_Clear(CommParam);
	LCD_EntyModeSet(CommParam);
	HAL_Delay(1);
	setled();//Led on.
	setwrite();//Write on.
}

void LCD_SetCursor(struct communicationParameters CommParam, uint8_t str, uint8_t num){
	switch (str){
		case 0:
			SendByte(CommParam, num | 0x80, 0);
			HAL_Delay(1);
			break;
		case 2:
			SendByte(CommParam, (num + 0x14) | 0x80, 0);
			HAL_Delay(1);
			break;
		case 1:
			SendByte(CommParam, (num + 0x40) | 0x80, 0);
			HAL_Delay(1);
			break;
		case 3:
			SendByte(CommParam, (num + 0x54) | 0x80, 0);
			HAL_Delay(1);
			break;
		default:
			SendByte(CommParam, num | 0x80, 0);
			HAL_Delay(1);
	}	
}

void LCD_PrintSQRT(struct communicationParameters CommParam){
	SendByte(CommParam, 0xE8, 1);
}
