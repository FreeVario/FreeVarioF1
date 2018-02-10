/*
 * ms5611.h
 *
 *  Created on: Jan 6, 2018
 *      Author: marco
 */

#ifndef MS5611_H_
#define MS5611_H_

#include "config.h"
#define N_PROM_PARAMS 6


/**
 * @brief  Main MS5611 structure
 */
typedef struct  {
	int32_t 	uP; //changed from _P, conflicts with teensy internal vars
	int32_t  	_T;
	int32_t 	_dT;
	uint16_t 	uC[N_PROM_PARAMS]; //also as above
	uint32_t 	_lastTime;
	uint8_t adress;
} SD_MS5611;




void MS5611_Setup(I2C_HandleTypeDef *hi2c, SD_MS5611* MS_Datastruct, uint8_t add);
void MS5611_sendCommand(uint8_t cmd,uint8_t adress, I2C_HandleTypeDef *hi2c);
void MS5611_readPressure(I2C_HandleTypeDef *hi2c,SD_MS5611* MS_Datastruct);
void MS5611_readTemperature(I2C_HandleTypeDef *hi2c, SD_MS5611* MS_Datastruct);
void MS5611_readCalibration(I2C_HandleTypeDef *hi2c, SD_MS5611* MS_Datastruct);
void MS5611_getCalibration(uint16_t *C, SD_MS5611* MS_Datastruct);
double MS5611_getAltitude(double pressure, double seaLevelPressure);
uint32_t MS5611_readnBytes(uint8_t nBytes, uint8_t adress, I2C_HandleTypeDef *hi2c);




#endif /* MS5611_H_ */