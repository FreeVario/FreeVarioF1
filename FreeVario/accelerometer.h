/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


void setupAccl();
void readAccl();

extern float accl[3];
extern float gyro[3];

#endif