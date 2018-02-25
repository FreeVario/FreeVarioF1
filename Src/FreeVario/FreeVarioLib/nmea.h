/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef NMEA_H_
#define NMEA_H_
#include "config.h"

void NMEA_setGforce(float gforce);
void NMEA_setPTAS1(double varioFt, double varioAvFt, double altitudeFt);
void NMEA_setnmeaShortLXWP0(float varioAlt, float a);
void NMEA_setnmeaVarioLXWP0(double varioAlt, float a, float b, float c, float d, float e, float f);
void NMEA_setNmeaVarioSentence(long rawPressure, double varioAlt, float climbRate, float temperature, float voltage);
void NMEA_setNmeaHumidSentence(int temperatureH11Kelvin, int humidity);
void NMEA_setNmeaPcProbeSentence(float aax, float aay, float aaz, int temperature, int humidity, int batVolt);
void getCRC(char *buff);
char *dtostrf (float val, signed char width, unsigned char prec, char *sout);

char nmeaGforce[15];
char nmeaMag[15];
char nmeaAtt[30];
char nmeaVarioLXWP0[60];
char nmeaVario[44];
char nmeaHumid[22];
char nmeaPTAS1[44];
char nmeaPcProbe[60];

#endif /* NMEA_H_ */
