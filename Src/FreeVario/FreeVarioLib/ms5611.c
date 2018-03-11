/*
 FreeVario http://FreeVario.org

 Copyright (c), PrimalCode (http://www.primalcode.org)
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */

#define OSR 					2		// 0-3
#define CMD_RESET 				0x1E
#define CMD_ADC_READ			0x00
#define CMD_CONV_D1_BASE 		0x40
#define CMD_CONV_D2_BASE 		0x50
#define CONV_REG_SIZE 			0x02
#define CMD_PROM_READ_BASE		0xA2
#define PROM_REG_SIZE			0x02
#define NBYTES_CONV 			3
#define NBYTES_PROM 			2

// Temperature sampling period threshold [milliseconds]
// Kindly read the comment bellow in getPressure() method
#define T_THR 1000

#include "ms5611.h"
#include "math.h"


void MS5611_Setup(SD_MS5611* MS_Datastruct, uint8_t add) {
	MS_Datastruct->_T = 0;
	MS_Datastruct->uP = 0;
	MS_Datastruct->_lastTime = T_THR;
	for (uint8_t k = 0; k < N_PROM_PARAMS; k++)
		MS_Datastruct->uC[k] = 69;
	MS_Datastruct->adress = add;

	MS5611_sendCommand(CMD_RESET, MS_Datastruct);

	HAL_Delay(100);
	MS5611_readCalibration( MS_Datastruct);


}

void MS5611_Reset(SD_MS5611* MS_Datastruct) {
	MS5611_sendCommand(CMD_RESET, MS_Datastruct);
	HAL_Delay(10);
	MS5611_readCalibration( MS_Datastruct);
}

void MS5611_readPressure( SD_MS5611* MS_Datastruct) {

	MS5611_readTemperature( MS_Datastruct); //updates temperature _dT and _T
	MS5611_sendCommand(CMD_CONV_D1_BASE + OSR * CONV_REG_SIZE,MS_Datastruct);	//read sensor, prepare a data
	HAL_Delay(1 + 2 * OSR);
	//if the second send command is to soon, the I2C bus will lockup in BUSY state

	MS5611_sendCommand(CMD_ADC_READ, MS_Datastruct);

	uint32_t D1 = MS5611_readnBytes(NBYTES_CONV, MS_Datastruct);

	int64_t OFF = (int64_t) MS_Datastruct->uC[2 - 1] * 65536
			+ (int64_t) MS_Datastruct->uC[4 - 1] * MS_Datastruct->_dT / 128;

	int64_t SENS = (int64_t) MS_Datastruct->uC[1 - 1] * 32768
			+ (int64_t) MS_Datastruct->uC[3 - 1] * MS_Datastruct->_dT / 256;

	if(D1 != '\0' && D1 != 0){

			MS_Datastruct->uP = (D1 * SENS / 2097152 - OFF) / 32768;

	}

}

void MS5611_readTemperature( SD_MS5611* MS_Datastruct) {
	// Code below can be uncommented for slight speedup:
	// NOTE: Be sure what you do! Notice that Delta 1C ~= Delta 2hPa
	//****************

	if (HAL_GetTick() - MS_Datastruct->_lastTime < T_THR)
		return;

	MS_Datastruct->_lastTime = HAL_GetTick();
	//****************
	MS5611_sendCommand(CMD_CONV_D2_BASE + OSR * CONV_REG_SIZE,MS_Datastruct);		//read sensor, prepare a data
	HAL_Delay(1 + 2 * OSR); 							//wait at least 8.33us
	MS5611_sendCommand(CMD_ADC_READ, MS_Datastruct); //get ready for reading the data

	uint32_t D2;
	D2 = MS5611_readnBytes(NBYTES_CONV, MS_Datastruct); //reading the data
	MS_Datastruct->_dT = D2 - ((uint32_t) MS_Datastruct->uC[5 - 1] * 256); //update '_dT'
	// Below, 'dT' and 'uC[6-1]'' must be casted in order to prevent overflow
	// A bitwise division can not be dobe since it is unpredictible for signed integers
	MS_Datastruct->_T = 2000
			+ ((int64_t) MS_Datastruct->_dT * MS_Datastruct->uC[6 - 1])
					/ 8388608;

}

void MS5611_readCalibration(SD_MS5611* MS_Datastruct) {
	for (uint8_t k = 0; k < 6; k++) {
		MS5611_sendCommand(CMD_PROM_READ_BASE + k * 2, MS_Datastruct);
		MS_Datastruct->uC[k] = (uint16_t) (MS5611_readnBytes(NBYTES_PROM, MS_Datastruct) & 0xFFFF); //masking out two LSB
	}
}

void MS5611_getCalibration(uint16_t *C, SD_MS5611* MS_Datastruct) {
	for (uint8_t k = 0; k < N_PROM_PARAMS; k++)
		C[k] = MS_Datastruct->uC[k];
	return;
}

// Calculate altitude from Pressure & Sea level pressure
double MS5611_getAltitude(double pressure, double seaLevelPressure) {
	return (44330.0f
			* (1.0f
					- pow((double) pressure / (double) seaLevelPressure,
							0.1902949f)));
}

void MS5611_sendCommand(uint8_t cmd, SD_MS5611* MS_Datastruct) {
	uint8_t Buffer_Tx1[1];
	Buffer_Tx1[0] = cmd;


	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(MS_Datastruct->i2chelper.instance, (uint16_t) MS_Datastruct->adress, (uint8_t *) Buffer_Tx1,	1, 1000);

	if (result == HAL_BUSY) { //use i2c helper to reset bus

		I2C_ClearBusyFlagErratum( &MS_Datastruct->i2chelper, 1000);
	}
}


uint32_t MS5611_readnBytes(uint8_t nBytes, SD_MS5611* MS_Datastruct) {

	uint32_t data = 0;
	uint8_t Buffer_Rx[nBytes];


	for (uint8_t i = 0; i < nBytes; i++) { //init array in case of null values
		Buffer_Rx[i] = 0;
	}

	if (HAL_I2C_GetState(MS_Datastruct->i2chelper.instance)	== HAL_I2C_STATE_READY) {

		HAL_StatusTypeDef result = HAL_I2C_Master_Receive(
				MS_Datastruct->i2chelper.instance,
				(uint16_t) (MS_Datastruct->adress + 1), (uint8_t *) Buffer_Rx,
				nBytes, 1000);

		if (result == HAL_OK) { //this saved crashes

			for (uint8_t i = 0; i < nBytes; i++) {
				data = (data << 8) + Buffer_Rx[i];
			}


		}else{
			I2C_ClearBusyFlagErratum(&MS_Datastruct->i2chelper, 1000);

		}

	}else{
		I2C_ClearBusyFlagErratum(&MS_Datastruct->i2chelper, 1000);

	}

	if(data == '\0' || data==0 ) {
		MS5611_Reset(MS_Datastruct);

	}

	return data;
	// no byte required
}


