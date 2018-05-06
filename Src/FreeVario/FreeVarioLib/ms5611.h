/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef MS5611_H_
#define MS5611_H_

#include "i2chelper.h"
#include "../Src/config.h"

#define N_PROM_PARAMS 6
#define MS5611_ADD1 0xEE
#define MS5611_ADD2 0xEC


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
	I2C_HandleTypeDef *hi2c;
	I2C_Helper_t  i2chelper;
} SD_MS5611;




void MS5611_Setup(SD_MS5611* MS_Datastruct, uint8_t add);
void MS5611_Reset(SD_MS5611* MS_Datastruct);
void MS5611_sendCommand(uint8_t cmd,SD_MS5611* MS_Datastruct);
void MS5611_readPressure(SD_MS5611* MS_Datastruct);
void MS5611_readTemperature(SD_MS5611* MS_Datastruct);
void MS5611_readCalibration( SD_MS5611* MS_Datastruct);
void MS5611_getCalibration(uint16_t *C, SD_MS5611* MS_Datastruct);
double MS5611_getAltitude(double pressure, double seaLevelPressure);
uint32_t MS5611_readnBytes(uint8_t nBytes, SD_MS5611* MS_Datastruct);




#endif /* MS5611_H_ */
