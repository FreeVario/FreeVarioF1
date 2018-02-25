/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "senddata.h"
#include "nmea.h"
#include <string.h>
#include <stdio.h>

extern float currentVarioMPS;
extern float cuttentVarioAvMPS;
extern float currentAltitudeMtr;

extern float accel_x;
extern float accel_y;
extern float accel_z;

extern float humidity;
extern float humidtemp;

//send of data

void sendSensorData(){

	NMEA_setnmeaShortLXWP0(currentAltitudeMtr, currentVarioMPS);
	CDC_Transmit_FS(nmeaVarioLXWP0, sizeof(nmeaVarioLXWP0));

#if defined(ACCL) && defined(HUMID)
	//NMEA_setNmeaPcProbeSentence(accel_x,accel_y,accel_z,humidtemp,humidity,0);
	//CDC_Transmit_FS(nmeaPcProbe, sizeof(nmeaPcProbe));
#endif

}


//does not work that well
void sendSplitGPSData(char *buffer) {

	char * p , z;
	char bleff[2048];
	p = strpbrk(buffer,"\r\n");
	NMEA_setnmeaShortLXWP0(currentAltitudeMtr, currentVarioMPS);

	if (p != NULL) {
		sprintf(bleff,"%s\r\n",p);
		strcpy(&z, p + 1);
		//strncpy(bleff,&buff, sizeof(buff));
		strncat(bleff,nmeaVarioLXWP0,sizeof(nmeaVarioLXWP0));
		strncat(bleff,&z,sizeof(z));


	}else{
		//TODO:check if there enough space left in buffer

		sprintf(bleff,"%s\n",buffer);
		strncat(bleff,nmeaVarioLXWP0,sizeof(nmeaVarioLXWP0));

	}
	CDC_Transmit_FS(bleff, sizeof(bleff));


}
