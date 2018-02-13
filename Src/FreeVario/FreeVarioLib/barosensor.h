/*
 * barosensor.h
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#ifndef BAROSENSOR_H_
#define BAROSENSOR_H_
#include "config.h"

extern int32_t realPressureAv;

void BARO_Setup();
void Baro_GetSensorData();
float getAltitudeFt();
double getAltitudeMt();
void calcVario();



#endif /* BAROSENSOR_H_ */
