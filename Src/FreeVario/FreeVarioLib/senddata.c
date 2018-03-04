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

#define GPSBUFFER 120
char sendBuffer[GPSBUFFER];
char  BtBuffer[256];
static int y;

extern float currentVarioMPS;
extern float cuttentVarioAvMPS;
extern float currentAltitudeMtr;
extern int32_t realPressureAv; // pressure hpa * 100

extern float accel_x;
extern float accel_y;
extern float accel_z;

extern float humidity;
extern float humidtemp; //TODO: temp set from whats available



//send of data

void sendSensorData(){

	NMEA_setnmeaShortLXWP0(currentAltitudeMtr, currentVarioMPS);
	//CDC_Transmit_FS(nmeaVarioLXWP0, sizeof(nmeaVarioLXWP0));

	strcat(BtBuffer,nmeaVarioLXWP0);


	NMEA_setNmeaLK8EX1(realPressureAv, currentAltitudeMtr,  currentVarioMPS, humidtemp, 0);
	//CDC_Transmit_FS(nmeaVarioLK8EX1, sizeof(nmeaVarioLK8EX1));
	strcat(BtBuffer,nmeaVarioLK8EX1);

	NMEA_setPTAS1(currentAltitudeMtr, cuttentVarioAvMPS, currentAltitudeMtr);
	//CDC_Transmit_FS(nmeaPTAS1, sizeof(nmeaPTAS1));
	strcat(BtBuffer,nmeaPTAS1);

#if defined(ACCL) && defined(HUMID)
	NMEA_setNmeaPcProbe(accel_x,accel_y,accel_z,humidtemp,humidity,0);
	strcat(BtBuffer,nmeaPcProbe);

#endif


	sendsData(BtBuffer);

}


void sendsData(char* txbuffer) {
	CDC_Transmit_FS(txbuffer, sizeof(txbuffer));
	HAL_UART_Transmit(&FV_UARTBT,txbuffer, strlen(txbuffer),0xff);
	memset(txbuffer, 0, sizeof(txbuffer));
}

void SendDataGPSbuid(char c) {                                         // GPSbuffer[] is global

  static char q;
  static bool flag = false;


  q = c;

  if ( q == 0x24 )                                              // '$'
  {
    y = 0;                                                      // brute force sync on '$' to GPSbuffer[0]
    // Serial << "Found $" << endl;
  }

  if ( y < GPSBUFFER) sendBuffer[y++] = q;

  if (q == 0x0d) {
    flag = true;                                                // is the character a CR for eol
    sendBuffer[y++] = '\r';
    sendBuffer[y++] = '\n';
    sendBuffer[y++] = '\0';
    y = 0;
  }

  if (flag) {                                                   // test for end of line and if the right GPSbuffer
    flag = false;                                               // reset for next time

   // CDC_Transmit_FS(sendBuffer, sizeof(sendBuffer));
    //strcat(BtBuffer,sendBuffer);
   sendsData(sendBuffer);
 //  HAL_UART_Transmit(&FV_UARTBT,sendBuffer, strlen(sendBuffer),0xff);

    memset(sendBuffer, 0, sizeof(sendBuffer));
  }
}
