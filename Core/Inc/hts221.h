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
#include "lcd2004a.h"
#include "rtc.h"
#define I2CNUMBER hi2c2
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
void viewLCD2004a();
void HTS221_ReadSetsRegister();
HTS221_Error_et HTS221_Get_Temperature(float *value);
HTS221_Error_et HTS221_Get_Humidity(float* value);

#endif /* INC_HTS221_H_ */
