#include "hts221.h"



uint8_t errorString=0;

HTS221_Error_et HTS221_ReadReg(uint8_t MemAddress, uint8_t numByteToRead, uint8_t *data ){
    if(HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, MemAddress, sizeof(uint8_t), (uint8_t*) &data, numByteToRead, 10)== HAL_OK){
        return (HTS221_OK);
    }
    else return (HTS221_ERROR);
}

HTS221_Error_et HTS221_WriteReg(uint8_t MemAddress, uint8_t NumByteToWrite, uint8_t *data ){
  if(HAL_I2C_Mem_Write(&hi2c2, HTS221_ADDRESS_WRITE, MemAddress, sizeof(uint8_t), (uint8_t*) &data, NumByteToWrite, 10)== HAL_OK){
      return (HTS221_OK);
  }
  else return (HTS221_ERROR);
}

HTS221_Error_et HTS221_SET_AVG(uint8_t AVGT, uint8_t AVGH){
    uint8_t data =0;
    data = AVGT<<3;
    data += AVGH;
    return (HTS221_WriteReg(AV_CONF, 1, &data));
}

HTS221_Error_et HTS221_SET_CTRL_REG1(uint8_t pd, uint8_t rate, uint8_t bdu){
    uint8_t data =0;
    data = data + pd + rate + bdu;
    return (HTS221_WriteReg(CTRL_REG1, 1, &data));
}

HTS221_Error_et HTS221_SET_CTRL_REG2(uint8_t boot, uint8_t heater, uint8_t one_shot){
    uint8_t data =0;
    data = data + boot + heater + one_shot;
    return (HTS221_WriteReg(CTRL_REG2, 1, &data));
}

HTS221_Error_et HTS221_SET_CTRL_REG3(uint8_t drdy_hl, uint8_t pp_od, uint8_t drdy){
    uint8_t data =0;
    data = data + drdy_hl + pp_od + drdy;
    return (HTS221_WriteReg(CTRL_REG3, 1, &data));
}

uint8_t* whoAm;
uint8_t* tOut;

HTS221_Error_et HTS221_INIT(HTS221InitData *HTS221){
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, WHO_AM_I, sizeof(uint8_t), (uint8_t*) &whoAm, 1, 10);
    printf("I am %x\n\r", whoAm);
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, AV_CONF, sizeof(uint8_t), (uint8_t*) &tOut, 1, 10);
    printf("AVG %x\n\r", tOut);
    if(HTS221_SET_AVG(HTS221->avgt,HTS221->avgh)){
        return HTS221_ERROR;
    }
    HTS221_ReadReg(AV_CONF, 1, &tOut );
    printf("AVG %x\n\r", tOut);
    HTS221_ReadReg(CTRL_REG1, 1, &tOut );
    printf("CTRL_REG1 %x\n\r", tOut);
    if(HTS221_SET_CTRL_REG1(HTS221->pd, HTS221->rate, HTS221->bdu)){
        return HTS221_ERROR;
    }
    HTS221_ReadReg(CTRL_REG1, 1, &tOut );
    printf("CTRL_REG1 %x\n\r", tOut);
    HTS221_ReadReg(CTRL_REG2, 1, &tOut );
    printf("CTRL_REG2 %x\n\r", tOut);
    if(HTS221_SET_CTRL_REG2(HTS221->boot, HTS221->heater, HTS221->one_shot)){
        return HTS221_ERROR;
    }
    HTS221_ReadReg(CTRL_REG2, 1, &tOut );
    printf("CTRL_REG2 %x\n\r", tOut);
    HTS221_ReadReg(CTRL_REG3, 1, &tOut );
    printf("CTRL_REG3 %x\n\r", tOut);
    if(HTS221_SET_CTRL_REG3(HTS221->drdy_hl, HTS221->pp_od, HTS221->drdy)){
        return HTS221_ERROR;
    }
    else{
        HTS221_ReadReg(CTRL_REG3, 1, &tOut );
        printf("CTRL_REG3 %x\n\r", tOut);
        return HTS221_OK;
    }
}

void HTS221_ReadSetsRegister(){
    uint8_t result;
//    HTS221_ReadReg(AV_CONF, 1, &result);
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, AV_CONF, sizeof(uint8_t), (uint8_t*) &result, 1, 10);
    printf("AV_CONF = %d\n\r",result);
    result = 0;
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, CTRL_REG1, sizeof(uint8_t), (uint8_t*) &result, 1, 10);
//    HTS221_ReadReg(CTRL_REG1, 1, &result);
    printf("CTRL_REG1 = %d\n\r",result);
    result = 0;
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, CTRL_REG2, sizeof(uint8_t), (uint8_t*) &result, 1, 10);
//    HTS221_ReadReg(CTRL_REG2, 1, &result);
    printf("CTRL_REG2 = %d\n\r",result);
    result = 0;
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, CTRL_REG3, sizeof(uint8_t), (uint8_t*) &result, 1, 10);
//    HTS221_ReadReg(CTRL_REG3, 1, &result);
    printf("CTRL_REG3 = %d\n\r",result);
}


/**
* @brief Read HTS221 temperature output registers, and calculate temperature.
* @param Pointer to the returned temperature value that must be divided by 10 to get the value in ['C].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Temperature(int16_t *value)
{
 int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
 int16_t T0_degC, T1_degC;
 uint8_t buffer[4], tmp;
 int32_t tmp32;
/*1. Read from 0x32 & 0x33 registers the value of coefficients T0_degC_x8 and T1_degC_x8*/
 if(HTS221_ReadReg(HTS221_T0_DEGC_X8, 2, buffer)){
     errorString =1;
     return HTS221_ERROR;
 }
 printf("Buffer1 %x\n\r", *buffer);
/*2. Read from 0x35 register the value of the MSB bits of T1_degC and T0_degC */
 if(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2, 1, &tmp))
     return HTS221_ERROR;
/*Calculate the T0_degC and T1_degC values*/
 T0_degC_x8_u16 = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);
 T1_degC_x8_u16 = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
 T0_degC = T0_degC_x8_u16>>3;
 T1_degC = T1_degC_x8_u16>>3;
/*3. Read from 0x3C & 0x3D registers the value of T0_OUT*/
/*4. Read from 0x3E & 0x3F registers the value of T1_OUT*/
 if(HTS221_ReadReg(HTS221_T0_OUT_L, 4, buffer)){
     errorString =4;
     return HTS221_ERROR;
 }
 printf("Buffer4 %x\n\r", *buffer);
 T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
 T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];
/* 5.Read from 0x2A & 0x2B registers the value T_OUT (ADC_OUT).*/
 if(HTS221_ReadReg(HTS221_TEMP_OUT_L_REG, 2, buffer)){
     errorString =5;
     return HTS221_ERROR;
 }
 printf("Buffer5 %x\n\r", *buffer);
 T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
/* 6. Compute the Temperature value by linear interpolation*/
 tmp32 = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC)*10);
 *value = (tmp32 /(T1_out - T0_out) + T0_degC*10)/10;
 return HTS221_OK;
}


/*Start test*/
/*Start variables*/
int16_t temperature=0;
HTS221InitData testData;
/*End variables*/

void HTS221Test(){
    uint8_t errorPrint;
    uint8_t tempData[4];
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

    HTS221_ReadSetsRegister();

    if(HTS221_INIT(&testData)){
        printf("no init\n\r");
        Error_Handler();
    }

    HTS221_ReadSetsRegister();

    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, STATUS_REG, sizeof(uint8_t), (uint8_t*) &errorPrint, 1, 10);
    printf("status register = %x\n\r", errorPrint);
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, HTS221_T0_OUT_L, sizeof(uint8_t), (uint8_t*) &tempData, 4, 10);
    printf("OUT = %x\n\r", tempData);
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, STATUS_REG, sizeof(uint8_t), (uint8_t*) &errorPrint, 1, 10);
    printf("status register = %x\n\r", errorPrint);
    HAL_I2C_Mem_Read (&hi2c2, HTS221_ADDRESS_READ, HTS221_T0_OUT_L, sizeof(uint8_t), (uint8_t*) &tempData, 4, 10);
    printf("OUT = %x\n\r", tempData);
    if(HTS221_Get_Temperature(&temperature)){
        printf("Error string %d\n\r",errorString);
        Error_Handler();
    }
    else{
        printf("Temperature = %d\n\r",temperature);
    }
}

/*End test*/
