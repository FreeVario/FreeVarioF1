/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#ifndef BOARDGPIO_H_
#define BOARDGPIO_H_
#include "config.h"

void BGPIO_Setup();
void BGPIO_Read();
void doIntervalCharge();
void readBatVoltage();
void readCharger();
void setPowerBankFunction(uint8_t state);
void togglePowerBankFunction();

#endif /* BOARDGPIO_H_ */
