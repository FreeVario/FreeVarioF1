/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#ifndef BAROSENSOR_H_
#define BAROSENSOR_H_
#include "config.h"

extern int32_t realPressureAv;

void BARO_Setup();
void BARO_Reset();
void Baro_Read();
float getAltitudeFt();
float getAltitudeMt();
void calcVario();
void checkAdaptiveVario(float vario);


#endif /* BAROSENSOR_H_ */
