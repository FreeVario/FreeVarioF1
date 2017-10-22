/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef BAROSENSOR_H
#define BAROSENSOR_H

#include "mbed.h"
#include <vector>
#include <numeric>

extern double realPressureAv;
extern double realTemp;
extern int samplecount;
extern float currentVarioFts; // ft/s
extern float cuttentVarioAvFts;
extern float currentAltitudeFt;

extern std::vector<float> varioFt;
extern std::vector<float> altitudeFt;

void setupBaro();
void readBaro();
void checkAdaptiveVario(double vario);
void readTemp();
float getAltitudeFt();
void calcVario();

#endif