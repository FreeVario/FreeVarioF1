/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#ifndef SENDDATA_H_
#define SENDDATA_H_
#include "config.h"



void sendSensorData(); //manager
void SendDataGPSbuid(char c); //team lead
void sendsData(char* txbuffer); //actually does the work

#endif /* SENDDATA_H_ */