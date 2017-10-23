/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "mbed.h"
#include "config.h"
#include "freevario.h"
#include "DHT.h"
#include "humidity.h"

int dhttemperature = 0;
int dhthumidity = 0;

#if defined(DHTH)

//modified lib to disable delay check
DHT dht(DHT_PIN, DHTTYPE);

void readHumid() {
    
    dht.readData();
    dhthumidity = dht.ReadHumidity();
    dhttemperature = dht.ReadTemperature(CELCIUS);
    
}

#endif