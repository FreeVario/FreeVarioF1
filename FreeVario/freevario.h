/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include <vector>
#include <numeric>
#include "BufferedSerial.h"

void sendSensorData();
void setupSerial();
void readSensor();
void queueData();
extern BufferedSerial pc;
extern std::vector<char> sendBuffer;
extern std::vector<float> vario;
extern std::vector<float> altitude;