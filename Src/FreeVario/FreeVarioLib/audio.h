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

void AUDIO_SetFreq(uint16_t freq);
void AUDIO_Setup_Tone();
void AUDIO_SetTone();
void AUDIO_ToneOff();
void AUDIO_ToneON();
void AUDIO_TonePeriodOn();
void AUDIO_TonePeriodOff();
void AUDIO_TestToneCall();
void AUDIO_TimerCall();

#endif /* AUDIO_H_ */
