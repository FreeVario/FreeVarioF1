/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef FREEVARIO_H
#define FREEVARIO_H

#include <vector>
#include <numeric>
#include "BufferedSerial.h"
#include "conf.h"

void sendSensorData();
void setupSerial();
void readSensor();
void queueData();
void sendSerial(char *message);
extern bool takeoff;
extern Serial pc;



#endif