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
#include "stdio.h"
#include "string.h"

#define GPSBUFFER 120
#define BTBUFFER 1024 //lower will crash when USB connected
char sendBuffer[GPSBUFFER];
char  BtBuffer[BTBUFFER];
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

//Protection against buffer overflow
void strBTCat(char * stradd) {

	if (strlen(BtBuffer) + strlen(stradd) < BTBUFFER ) { //then safe to cat
		strcat(BtBuffer,stradd);
	} else { //clear it, data is old
		memset(BtBuffer, 0, strlen(BtBuffer));

	}

}

//send all data
void sendSensorData(){

	NMEA_setnmeaShortLXWP0(currentAltitudeMtr, currentVarioMPS);
	strBTCat(nmeaVarioLXWP0);

	NMEA_setNmeaLK8EX1(realPressureAv, currentAltitudeMtr,  currentVarioMPS, humidtemp, 0);
	strBTCat(nmeaVarioLK8EX1);

	NMEA_setPTAS1(currentVarioMPS, cuttentVarioAvMPS, currentAltitudeMtr);
	strBTCat(nmeaPTAS1);

#if defined(ACCL) && defined(HUMID)
	NMEA_setNmeaPcProbe(accel_x,accel_y,accel_z,humidtemp,humidity,0);
	strBTCat(nmeaPcProbe);

#endif

	sendsData(BtBuffer);

}

void sendsData(char* txbuffer) {
	CDC_Transmit_FS(txbuffer, (uint16_t)strlen(txbuffer));

	HAL_UART_Transmit(&FV_UARTBT,(uint8_t *)txbuffer, strlen(txbuffer),0xff);
	memset(txbuffer, 0, strlen(txbuffer));
}

void SendDataGPSbuid(char * c) {                                         // GPSbuffer[] is global

  static char q;
  static bool flag = false;

  q = c;

  if ( q == 0x24 )                                              // '$'
  {
    y = 0;                                                      // brute force sync on '$' to GPSbuffer[0]
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

  // sendsData(sendBuffer);
    strBTCat(sendBuffer);
    memset(sendBuffer, 0, strlen(sendBuffer));
  }
}
