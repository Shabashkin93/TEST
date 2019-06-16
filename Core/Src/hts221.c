#include "hts221.h"



uint8_t errorString=0;

static uint8_t HTS221_ReadReg(uint8_t MemAddress){
    uint8_t res=0;
    if(HAL_I2C_Mem_Read(&hi2c2, HTS221_ADDRESS_READ, MemAddress, 1, &res, 1, 1000) == HAL_OK){
        return (res);
    }
}

HAL_StatusTypeDef HTS221_WriteReg(uint8_t MemAddress, uint8_t data){
  return(HAL_I2C_Mem_Write(&hi2c2, HTS221_ADDRESS_WRITE, MemAddress, 1, &data, 1, 1000));
}

HAL_StatusTypeDef HTS221_SET_AVG(uint8_t AVGT, uint8_t AVGH){
    HAL_StatusTypeDef status;
    uint8_t data =0;
    data = AVGT<<3;
    data += AVGH;
    status = HTS221_WriteReg(AV_CONF, &data);
    return status;
}

HAL_StatusTypeDef HTS221_SET_CTRL_REG1(uint8_t pd, uint8_t rate, uint8_t bdu){
    uint8_t data =0;
    data = data + pd + rate + bdu;
    return (HTS221_WriteReg(CTRL_REG1, &data));
}

HAL_StatusTypeDef HTS221_SET_CTRL_REG2(uint8_t boot, uint8_t heater, uint8_t one_shot){
    uint8_t data =0;
    data = data + boot + heater + one_shot;
    return (HTS221_WriteReg(CTRL_REG2, &data));
}

HAL_StatusTypeDef HTS221_SET_CTRL_REG3(uint8_t drdy_hl, uint8_t pp_od, uint8_t drdy){
    uint8_t data =0;
    data = data + drdy_hl + pp_od + drdy;
    return (HTS221_WriteReg(CTRL_REG3, &data));
}

HTS221_Error_et HTS221_INIT(HTS221InitData *HTS221){
    printf("I am %x\n\r", HTS221_ReadReg(WHO_AM_I));
//    printf("AVG %x\n\r", HTS221_ReadReg(AV_CONF));
//    HTS221_ReadSetsRegister();
    if(HTS221_SET_AVG(HTS221->avgt,HTS221->avgh)!=HAL_OK){
        return HTS221_ERROR;
    }
    if(HTS221_SET_CTRL_REG1(HTS221->pd, HTS221->rate, HTS221->bdu)!=HAL_OK){
        return HTS221_ERROR;
    }
    if(HTS221_SET_CTRL_REG2(HTS221->boot, HTS221->heater, HTS221->one_shot)!=HAL_OK){
        return HTS221_ERROR;
    }
    if(HTS221_SET_CTRL_REG3(HTS221->drdy_hl, HTS221->pp_od, HTS221->drdy)!=HAL_OK){
        return HTS221_ERROR;
    }
    else{
//        HTS221_ReadSetsRegister();
        return HTS221_OK;
    }
}

void HTS221_ReadSetsRegister(){
    uint8_t result;
    printf("AV_CONF = %x\n\r",HTS221_ReadReg(AV_CONF));
    printf("CTRL_REG1 = %x\n\r",HTS221_ReadReg(CTRL_REG1));
    printf("CTRL_REG2 = %x\n\r",HTS221_ReadReg(CTRL_REG2));
    printf("CTRL_REG3 = %x\n\r",HTS221_ReadReg(CTRL_REG3));
}


/**
* @brief Read HTS221 temperature output registers, and calculate temperature.
* @param Pointer to the returned temperature value that must be divided by 10 to get the value in ['C].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Temperature(float *value)
{
 uint16_t T0_degC_x8_u16, T1_degC_x8_u16;
 int16_t T0_degC, T1_degC, T0_out, T1_out, T_out;
 uint8_t buffer[4], tmp;
 float tmp32;
 uint8_t buffer1[4];
 uint8_t buffer2[2];
/*1. Read from 0x32 & 0x33 registers the value of coefficients T0_degC_x8 and T1_degC_x8*/
// if(HTS221_ReadReg(HTS221_T0_DEGC_X8, 2, buffer))
// return HTS221_ERROR;
// buffer[0]=HTS221_ReadReg(HTS221_T0_DEGC_X8);
// buffer[1]=HTS221_ReadReg(HTS221_T0_DEGC_X8+1);
// printf("res1 = %x\n\r",buffer);
/*2. Read from 0x35 register the value of the MSB bits of T1_degC and T0_degC */
//tmp = HTS221_ReadReg(HTS221_T0_T1_DEGC_H2);
//printf("res2 = %x\n\r",tmp);
/*Calculate the T0_degC and T1_degC values*/
 T0_degC_x8_u16 = (((uint16_t)(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2) & 0x03)) << 8) | ((uint16_t)HTS221_ReadReg(HTS221_T0_DEGC_X8));
 T1_degC_x8_u16 = (((uint16_t)(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2) & 0x0C)) << 6) | ((uint16_t)HTS221_ReadReg(HTS221_T0_DEGC_X8+1));
 T0_degC = T0_degC_x8_u16>>3;
 T1_degC = T1_degC_x8_u16>>3;
 printf("res3 = %d %d\n\r",T0_degC, T1_degC);
///*3. Read from 0x3C & 0x3D registers the value of T0_OUT*/
///*4. Read from 0x3E & 0x3F registers the value of T1_OUT*/
// buffer1[0]=HTS221_ReadReg(HTS221_T0_OUT_L);
// buffer1[1]=HTS221_ReadReg(HTS221_T0_OUT_L+1);
// buffer1[2]=HTS221_ReadReg(HTS221_T0_OUT_L+2);
// buffer1[3]=HTS221_ReadReg(HTS221_T0_OUT_L+3);
//printf("res4 = %x\n\r",buffer1);
 T0_out = (((uint16_t)HTS221_ReadReg(HTS221_T0_OUT_L+1))<<8) | (uint16_t)HTS221_ReadReg(HTS221_T0_OUT_L);
 T1_out = ((HTS221_ReadReg(HTS221_T0_OUT_L+3))<<8) | HTS221_ReadReg(HTS221_T0_OUT_L+2);
 printf("res5 = %d %d\n\r",T0_out, T1_out);
///* 5.Read from 0x2A & 0x2B registers the value T_OUT (ADC_OUT).*/
// buffer2[0]=HTS221_ReadReg(HTS221_TEMP_OUT_L_REG);
// buffer2[1]=HTS221_ReadReg(HTS221_TEMP_OUT_L_REG+1);
T_out = (((uint16_t)HTS221_ReadReg(HTS221_TEMP_OUT_L_REG+1))<<8) | (uint16_t)HTS221_ReadReg(HTS221_TEMP_OUT_L_REG);
/* 6. Compute the Temperature value by linear interpolation*/
 tmp32 = ((float)(T_out - T0_out)) * ((float)(T1_degC - T0_degC)*10);
 *value = tmp32 /(T1_out - T0_out) + T0_degC*10;
 return HTS221_OK;
}


/*Start test*/
/*Start variables*/
HTS221InitData testData;
/*End variables*/

void HTS221Test(){
    uint8_t *errorPrint;
    uint8_t tempData[2];
    uint8_t out;
    uint8_t in;
    float temperature;
//    float temperature1 = 1.326545;
    testData.avgt = AVGT_256;
    testData.avgh = AVGH_512;
    testData.pd = CTRL_REG1_PD;
    testData.rate = CTRL_REG1_12_5HZ;
    testData.bdu = CTRL_REG1_BDU;
    testData.boot = CTRL_REG2_BOOT;
    testData.heater = 0;
    testData.one_shot = CTRL_REG2_ONE_SHOT;
    testData.drdy_hl = 0;
    testData.pp_od = 0;
    testData.drdy = 0;

    if(HAL_I2C_IsDeviceReady (&hi2c2, HTS221_ADDRESS_READ, 1, 100) == HAL_OK){
        printf("hts221 OK\n\r");
      }
    else{
        Error_Handler();
    }

//    HTS221_ReadSetsRegister();
//
    if(HTS221_INIT(&testData)){
        printf("no init\n\r");
        Error_Handler();
    }

    if(HTS221_Get_Temperature(&temperature)){
        printf("Error string %d\n\r",errorString);
        Error_Handler();
    }
    else{
        printf("Temperature = %06f\n\r",temperature);
    }
    HTS221_ReadSetsRegister();
    HAL_I2C_Mem_Read(&hi2c2, HTS221_ADDRESS_READ, TEMP_OUT_L, 1, &tempData, 2, 1000);
    printf("TEMP_OUT_L = %x\n\r", *tempData);
}

/*End test*/
