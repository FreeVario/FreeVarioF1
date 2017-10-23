/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/



#ifndef HUMIDITY_H
#define HUMIDITY_H


#include "mbed.h"
#include "config.h"
#include "freevario.h"


extern int dhttemperature;
extern int dhthumidity;

void readHumid();

#endif