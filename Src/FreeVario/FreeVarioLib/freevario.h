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


void FV_Run();
void run10();
void run100();
void run1000();

void FV_GpsCallback();
void  FV_DHTCallback();

//private functions
static void setup();
static void loop();

#endif /* FREEVARIO_H_ */
