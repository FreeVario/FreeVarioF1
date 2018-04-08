/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef AUDIO_H_
#define AUDIO_H_
#include "config.h"


void makeVarioAudio(float vario);
void AUDIO_Setup_Tone();
void StartupTone();
void tone(float freq, int period);
void resetTone();
void noTone();
int millis();
void playTonePause(int freq, int nbeeps, int tpause);
void playToneInterval(int freq, int period, int tinterval);
void playTwoToneInterval(int freq,int freq2, int period, int intervald);
void noToneTimer();
void dynaTone(float freq);

void AUDIO_TestToneCall();


#endif /* AUDIO_H_ */
