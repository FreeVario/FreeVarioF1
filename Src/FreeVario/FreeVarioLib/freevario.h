/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef FREEVARIO_H_
#define FREEVARIO_H_
#include "config.h"


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,uint8_t transferdirection,uint16_t AddrMatchCode);
void FV_Run();
void run10();
void run100();
void run200();
void run1000();


//private functions
static void setup();
static void loop();

#endif /* FREEVARIO_H_ */
