/*
 * config.h
 * Program configuration
 *  Created on: Feb 8, 2018
 *      Author: marco
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "../Src/config_hal.h"

#include "stdbool.h"
#include "settings.h"
#include "stackops.h"
#include "globals.h"



#define VARIO
//#define VARIO2


#define TAKEOFFVARIO 0.8 //0.4 //abs vario level to detect takeoff
#define BUZZERZEROCLIMB -0.3 // Normal sink rate for glider is -0.9 m/s. At this value up to 0 m/s a "blip" sound is made
#define BUZZSINKALERT -2 //Alert if sinking harder than normal (not sink alarm)
#define BUZZSINKALERTPAUSE 8000 //pause length in between alerts
#define BUZZERVARIOSTOP 10000 //time vario STOP making noise when climbrate 0 m/s
#define OUTOFTHERMALBUZZT 3000 //time buzzer goes buuhhhhh
#define SOARDETECTION 30000 // if climbrate is constant for set milliseconds at 0 m/s the buzzer is muted
//#define TESTBUZZER

#endif /* CONFIG_H_ */
