/*
 * settings.h
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "string.h"
#include "stdbool.h"

void getDefaultConfig();
void setupConfig();

typedef struct {
  int eepromSaveVersion;
  float qnePressure;
  int sinkAlarmLevel;
  bool SerialOut;
  bool SerialOutBT;
  bool SerialOutESP;
  bool SerialOutUSB;
  bool ptas1;
  bool lxnav;
  bool pcprobe; //
  bool xcs; //
  bool variosmooth;
  bool buzzer;
  int SoarDeadBandTime;
  int advTriggerLevel;
  int advTriggerTime;
  int advRelaxTime;
  bool advMinSmooth;
  bool advMaxSmooth;
  int gliderSinkRate;

} settings_t;

extern settings_t conf;






#endif /* SETTINGS_H_ */
