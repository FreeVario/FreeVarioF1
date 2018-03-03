/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#ifndef CONFIG_H_
#define CONFIG_H_
#include "../Src/config_hal.h"

#include <math.h>
#include "stdbool.h"
#include "settings.h"
#include "stackops.h"
#include "globals.h"

#define SENDBUFFER 1024

#define VARIO
//#define VARIO2

//#define ACCL
#define ACCLSMOOTH 10 //Lowpass filter level
//#define HUMID //enable humid sensor

//#define BUZZERDAC //use the DAC output
#define STARTDELAY 6000 //the time delay before the process starts
#define TAKEOFFVARIO 0.8 //0.4 //abs vario level to detect takeoff
#define BUZZERZEROCLIMB -0.3 // Normal sink rate for glider is -0.9 m/s. At this value up to 0 m/s a "blip" sound is made
#define BUZZSINKALERT -2 //Alert if sinking harder than normal (not sink alarm)
#define BUZZSINKALERTPAUSE 8000 //pause length in between alerts
#define BUZZERVARIOSTOP 10000 //time vario STOP making noise when climbrate 0 m/s
#define OUTOFTHERMALBUZZT 3000 //time buzzer goes buuhhhhh
#define SOARDETECTION 30000 // if climbrate is constant for set milliseconds at 0 m/s the buzzer is muted
#define BUZZERCLIMBING 0.2 // vario level to start giving climbing signal
#define AUDIOSMOOTH 10 //smooth out audio changes
#define ADAPTIVEVARIO //Adapts the vario low pass filter

//#define TESTBUZZER



#endif /* CONFIG_H_ */
