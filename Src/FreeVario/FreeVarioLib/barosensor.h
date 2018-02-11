/*
 * barosensor.h
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#ifndef BAROSENSOR_H_
#define BAROSENSOR_H_
#include "config.h"


void BARO_Setup();
void Baro_GetSensorData();
float getAltitudeFt();
float getAltitudeMt();
void calcVarioFt();



#endif /* BAROSENSOR_H_ */
