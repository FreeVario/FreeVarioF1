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

#define AUDIO_VARIO_SILENT 0
#define AUDIO_VARIO_UP 1
#define AUDIO_VARIO_PRETHERMAL 2
#define AUDIO_VARIO_SINK_ALERT 3
#define AUDIO_VARIO_SINKALARM 4

void AUDIO_SetFreq(float freq);
void AUDIO_Setup_Tone();
void noTone();
void AUDIO_SetTone();
void AUDIO_ToneOff();
void AUDIO_ToneON();
void AUDIO_TonePeriodOn();
void AUDIO_TonePeriodOff();
void AUDIO_TestToneCall();
void AUDIO_TimerCall();
void AUDIO_Vario(float vario);
void AUDIO_varioUP();

#endif /* AUDIO_H_ */
