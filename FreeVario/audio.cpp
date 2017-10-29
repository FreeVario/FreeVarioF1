/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "mbed.h"
#include "config.h"
#include "freevario.h"
#include "audio.h"



bool climbing = false;
int tm;
int rm;
int stime;
bool toneOn = false;
bool muted = false;
float variof;
int pause=0;
int tcount=0;
int notonetimer=0;

PwmOut speaker(BUZZPIN);

#if defined(TESTBUZZER)
unsigned long btime = 0;
float testvario = -6;
Timer tst_timer;
#endif


#if defined(BUZZER)

#define BASEPULSE 200
#define TOPPULSE  1000


void tone(float freq, int period) {

 notonetimer = period + millis();
 speaker.period(1.0/freq);
 speaker = BUZZVOLUME; //0.5 = max volume 
 
 
   
}


void tone(float freq) {

 speaker.period(1.0/freq);
 speaker = BUZZVOLUME; //0.5 = max volume 
 
   
}

void noToneTimer() {
   if(millis() >= notonetimer && notonetimer > 0) {
     noTone();
     notonetimer = 0;
   }
 
}


void noTone(){
 speaker = 0.0;
 notonetimer = 0;
}

int millis() {
  return int(us_ticker_read() / 1000);
}




// Non-Blocking beep blob beep
void playTwoToneInterval(int freq,int freq2, int period, int intervald) {
 
  
  if (toneOn) {
    int wait = period + tm;
    
    
    if ( wait < millis()) {
      toneOn = false;
      noTone();
      tone(freq2, intervald);
      rm = millis();
    }

  } else {
    int ndwait = intervald + rm;
     
    if(ndwait < millis()) {
    
    tone( freq, period);
    toneOn = true;
    tm = millis();
    }
  }

}




// Non-Blocking beep beep beep
void playToneInterval(int freq, int period, int tinterval) {

  if (toneOn) {
    int wait = period + tinterval + tm;

    if ( wait < millis()) {
      toneOn = false;
      noTone();
      tcount++; // count the amount of beeps for playTonePause
      if (tcount > 1000) { // prevent overflow
        tcount = 0;
      }
    }

  } else {
    tone( freq, period);
    toneOn = true;
    tm = millis();
  }

}


// nth beeps then pauze x

void playTonePause(int freq, int nbeeps, int tpause) {

   if (pause < millis()) {
      
      if (tcount < nbeeps) {
        playToneInterval(freq, 500, 200);
        
      }else{
        pause = millis() + tpause;
        tcount=0;

      }
      
    
   }

  
}

void makeVarioAudio(float vario) {
  int pulse;
  float varioorg = vario;
   noToneTimer();
#if defined(TESTBUZZER)
  vario = testvario;
  int tpassed = millis() - btime;
  if (tpassed > 2000) {
    testvario = testvario + 0.2;
    btime = millis();
  }

  if (testvario > 9 ) {
    testvario = -6;
  }

  
#endif

  if (vario > 9) {
    vario = 9;

  }

#if defined(SOARDETECTION) && !defined(TESTBUZZER)

  if (varioorg > -0.2 && varioorg < 0.2) {
    int diffe = millis() - stime;
    if (diffe >  int(conf.SoarDeadBandTime)) {
      muted = true;
    }
  } else {
    stime = millis();
    muted = false;
  }

#endif

    if (vario <= 0 && vario >= BUZZERZEROCLIMB) {
      
      float variofr = (1 + vario) * 200  + 900;
      
      if (!muted) {
         playToneInterval(variofr, 50, 400);
      }

    }
    
   if (vario <= double(conf.sinkAlarmLevel)/1000 ) { //sink alarm
      if (!muted) {
         playTwoToneInterval(1400, 1800, 100, 100);
      }

   }

#if defined(BUZZSINKALERT) 
    if (vario <=  BUZZSINKALERT && vario >= double(conf.sinkAlarmLevel)/1000 ) {
       playTonePause(300, fabs(vario), BUZZSINKALERTPAUSE);
    }
  
#endif
   

  
  variof = (float(fabs(vario)) * 200 ) + 900;
  //variof = (10 * variof + variofa) / 11 ;

  if (vario > 0) {
    pulse = TOPPULSE / (vario * 10) + 100;
    if (!muted) {
      playToneInterval(variof, pulse, pulse / 2);
    }
    climbing = true;
  } else {
    if (climbing ) { //dropped out of the thermal
      tone( 100, OUTOFTHERMALBUZZT);
      climbing = false;
    }
  }

}


#endif

