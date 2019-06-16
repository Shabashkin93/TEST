/*
 * hts221.h
 *
 *  Created on: Apr 28, 2019
 *      Author: Shabashkin Oleg
 */

#ifndef INC_HTS221_H_
#define INC_HTS221_H_

#include "i2c.h"
#include <string.h>

#define HTS221_ADDRESS_WRITE    0xBE
#define HTS221_ADDRESS_READ     0xBF

//Device identification
#define WHO_AM_I          0x0F

//Humidity and temperature resolution mode
#define AV_CONF     0x10
//Temperature
#define AVGT_2      0x0
#define AVGT_4      0x1
#define AVGT_8      0x2
#define AVGT_16     0x3
#define AVGT_32     0x4
#define AVGT_64     0x5
#define AVGT_128    0x6
#define AVGT_256    0x7
//Humidity
#define AVGH_4      0x0
#define AVGH_8      0x1
#define AVGH_16     0x2
#define AVGH_32     0x3
#define AVGH_64     0x4
#define AVGH_128    0x5
#define AVGH_256    0x6
#define AVGH_512    0x7

//Control register 1
#define CTRL_REG1         0x20
//PD: power-down control (0: power-down mode; 1: active mode)
#define CTRL_REG1_PD       0x80
//ODR1, ODR0: output data rate selection
#define CTRL_REG1_ONE_SHOT 0x0
#define CTRL_REG1_1HZ      0x1
#define CTRL_REG1_7HZ      0x2
#define CTRL_REG1_12_5HZ   0x3
// BDU: block data update (0: continuous update; 1: output registers not updated until MSB and LSB reading)
#define CTRL_REG1_BDU      0x4

//Control register 2
#define CTRL_REG2         0x21
//BOOT: Reboot memory content (0: normal mode; 1: reboot memory content)
#define CTRL_REG2_BOOT         0x80
//Heater (0: heater disable; 1: heater enable)
#define CTRL_REG2_HEATER         0x2
//One-shot enable (0: waiting for start of conversion; 1: start for a new dataset)
#define CTRL_REG2_ONE_SHOT       0x1

//Control register 3
#define CTRL_REG3         0x22
//DRDY_H_L: Data Ready output signal active high, low (0: active high - default;1: active low)
#define CTRL_REG3_DRDY_H_L 0x80
//PP_OD: Push-pull / Open Drain selection on pin 3 (DRDY) (0: push-pull - default; 1: open drain)
#define CTRL_REG3_PP_OD   0x40
//DRDY_EN: Data Ready enable (0: Data Ready disabled - default;1: Data Ready signal available on pin 3)
#define CTRL_REG3_DRDY   0x2

//Status register
#define STATUS_REG        0x27
//H_DA: Humidity data available. (0: new data for humidity is not yet available; 1: new data for humidity is available)
#define STATUS_REG_H_DA 0x2
#define STATUS_REG_T_DA 0x1

#define HUMIDITY_OUT_L    0x28
#define HUMIDITY_OUT_H    0x29
#define TEMP_OUT_L        0x2A
#define TEMP_OUT_H        0x2B

/* defines for HTS221_Get_Temperature API */
#define HTS221_T0_DEGC_X8       0x32
#define HTS221_T0_T1_DEGC_H2    0x35
#define HTS221_T0_OUT_L         0x3C
#define HTS221_TEMP_OUT_L_REG   0x2A

#define HTS221_H0_RH_X2 0x30
#define HTS221_H1_RH_X2 0x31
#define HTS221_H0_T0_OUT_L 0x36
#define HTS221_H1_T0_OUT_L 0x3A
#define HTS221_HR_OUT_L_REG 0x28

extern uint8_t errorString;

typedef struct {
    uint8_t avgt;
    uint8_t avgh;
    uint8_t pd;
    uint8_t rate;
    uint8_t bdu;
    uint8_t boot;
    uint8_t heater;
    uint8_t one_shot;
    uint8_t drdy_hl;
    uint8_t pp_od;
    uint8_t drdy;
}HTS221InitData;

typedef enum {HTS221_OK = 0, HTS221_ERROR = 1} HTS221_Error_et;
static uint8_t HTS221_ReadReg(uint8_t MemAddress);
HAL_StatusTypeDef HTS221_WriteReg(uint8_t MemAddress, uint8_t data );
HAL_StatusTypeDef HTS221_SET_AVG(uint8_t AVGT, uint8_t AVGH);
HAL_StatusTypeDef HTS221_SET_CTRL_REG1(uint8_t pd, uint8_t rate, uint8_t bdu);
HAL_StatusTypeDef HTS221_SET_CTRL_REG2(uint8_t boot, uint8_t heater, uint8_t one_shot);
HAL_StatusTypeDef HTS221_SET_CTRL_REG3(uint8_t drdy_hl, uint8_t pp_od, uint8_t drdy);
HTS221_Error_et HTS221_INIT(HTS221InitData *HTS221);
void HTS221Test();
void HTS221_ReadSetsRegister();
HTS221_Error_et HTS221_Get_Temperature(float *value);

#endif /* INC_HTS221_H_ */


///**
//* @brief Read HTS221 temperature output registers, and calculate temperature.
//* @param Pointer to the returned temperature value that must be divided by 10 to get the value in ['C].
//* @retval Error code [HTS221_OK, HTS221_ERROR].
//*/
//HTS221_Error_et HTS221_Get_Temperature(float *value)
//{
// uint16_t T0_degC_x8_u16, T1_degC_x8_u16;
// int16_t T0_degC, T1_degC, T0_out, T1_out, T_out;
// uint8_t buffer[4], tmp;
// float tmp32;
// uint8_t buffer1[4];
// uint8_t buffer2[2];
///*1. Read from 0x32 & 0x33 registers the value of coefficients T0_degC_x8 and T1_degC_x8*/
//// if(HTS221_ReadReg(HTS221_T0_DEGC_X8, 2, buffer))
//// return HTS221_ERROR;
//// buffer[0]=HTS221_ReadReg(HTS221_T0_DEGC_X8);
//// buffer[1]=HTS221_ReadReg(HTS221_T0_DEGC_X8+1);
//// printf("res1 = %x\n\r",buffer);
///*2. Read from 0x35 register the value of the MSB bits of T1_degC and T0_degC */
////tmp = HTS221_ReadReg(HTS221_T0_T1_DEGC_H2);
////printf("res2 = %x\n\r",tmp);
///*Calculate the T0_degC and T1_degC values*/
// T0_degC_x8_u16 = (((uint16_t)(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2) & 0x03)) << 8) | ((uint16_t)HTS221_ReadReg(HTS221_T0_DEGC_X8));
// T1_degC_x8_u16 = (((uint16_t)(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2) & 0x0C)) << 6) | ((uint16_t)HTS221_ReadReg(HTS221_T0_DEGC_X8+1));
// T0_degC = T0_degC_x8_u16>>3;
// T1_degC = T1_degC_x8_u16>>3;
// printf("res3 = %d %d\n\r",T0_degC, T1_degC);
/////*3. Read from 0x3C & 0x3D registers the value of T0_OUT*/
/////*4. Read from 0x3E & 0x3F registers the value of T1_OUT*/
//// buffer1[0]=HTS221_ReadReg(HTS221_T0_OUT_L);
//// buffer1[1]=HTS221_ReadReg(HTS221_T0_OUT_L+1);
//// buffer1[2]=HTS221_ReadReg(HTS221_T0_OUT_L+2);
//// buffer1[3]=HTS221_ReadReg(HTS221_T0_OUT_L+3);
////printf("res4 = %x\n\r",buffer1);
// T0_out = (((uint16_t)HTS221_ReadReg(HTS221_T0_OUT_L+1))<<8) | (uint16_t)HTS221_ReadReg(HTS221_T0_OUT_L);
// T1_out = ((HTS221_ReadReg(HTS221_T0_OUT_L+3))<<8) | HTS221_ReadReg(HTS221_T0_OUT_L+2);
// printf("res5 = %d %d\n\r",T0_out, T1_out);
/////* 5.Read from 0x2A & 0x2B registers the value T_OUT (ADC_OUT).*/
//// buffer2[0]=HTS221_ReadReg(HTS221_TEMP_OUT_L_REG);
//// buffer2[1]=HTS221_ReadReg(HTS221_TEMP_OUT_L_REG+1);
//T_out = (((uint16_t)HTS221_ReadReg(HTS221_TEMP_OUT_L_REG+1))<<8) | (uint16_t)HTS221_ReadReg(HTS221_TEMP_OUT_L_REG);
///* 6. Compute the Temperature value by linear interpolation*/
// tmp32 = ((float)(T_out - T0_out)) * ((float)(T1_degC - T0_degC)*10);
// *value = tmp32 /(T1_out - T0_out) + T0_degC*10;
// return HTS221_OK;
//}
