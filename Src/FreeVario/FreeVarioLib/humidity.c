/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "humidity.h"
#include "am2322.h"


extern float humidity;
extern float humidtemp;

void HUMID_Setup(){

}



void HUMID_Read() {
#ifdef FV_AMI2C
	AM2322_poll(&FV_AMI2C);
	humidity=AM2322_getHumidity();
	humidtemp=AM2322_getTemperature();
#endif
}
