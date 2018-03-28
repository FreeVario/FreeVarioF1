/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "audio.h"


float step=0;
float vario_s=0;
float t_vario=-5;


bool climbing = false;
int tm;
int rm;
int stime;
bool toneOn = false;
bool muted = false;
float variof;
float variofr;
int pause=0;
int tcount=0;
int notonetimer=0;


const uint16_t sine_wave_array[32] = {2047, 1648, 1264, 910, 600,  345,
        156, 39,  0,  39,  156,  345,
        600, 910, 1264, 1648, 2048, 2447,
        2831, 3185, 3495, 3750, 3939, 4056,
        4095, 4056, 3939, 3750, 3495, 3185,
        2831, 2447};


extern TIM_HandleTypeDef FV_TONETMR;


#ifdef BUZZERDAC
extern DAC_HandleTypeDef FV_DAC;
extern TIM_HandleTypeDef FV_DACTMR;
#endif


void AUDIO_Setup_Tone() {
	 HAL_TIM_PWM_Start(&FV_TONETMR, FV_TONECHN);

	 FV_TONEHALTMR->PSC  = SystemCoreClock/10000000;

	 //DAC output
#ifdef BUZZERDAC
	 HAL_TIM_Base_Start(&FV_DACTMR);
	 HAL_DAC_Start(&FV_DAC,FV_DAC_CHANNEL);
	 HAL_DAC_Start_DMA(&FV_DAC, FV_DAC_CHANNEL, (uint32_t*)sine_wave_array, 32, DAC_ALIGN_12B_R);
	 FV_DACHALTMR->PSC = SystemCoreClock/100000000;
#endif


}


#define BASEPULSE 200
#define TOPPULSE  1000
#define PWMTMRMULTIPLIER 10000000
#define DACTMRMULTIPLIER 1000000

void tone(float freq, int period) {

    notonetimer = period + millis();
    uint16_t fv_tone_t = 1/(float)freq * PWMTMRMULTIPLIER;
    FV_TONEHALTMR->CR1 |= TIM_CR1_CEN;
    FV_TONEHALTMR->ARR  = fv_tone_t;
	FV_TONEHALTMR->FV_TONECCR = fv_tone_t/2;

#ifdef BUZZERDAC
	FV_DACHALTMR->ARR = 1/(float)freq * DACTMRMULTIPLIER;
	FV_DACHALTMR->CR1 |= TIM_CR1_CEN;
#endif
}

//changes tone while beeping
void dynaTone(float freq) {
	uint16_t fv_tone_t = 1/(float)freq * PWMTMRMULTIPLIER;
	FV_TONEHALTMR->ARR  = fv_tone_t;
	FV_TONEHALTMR->FV_TONECCR = fv_tone_t/2;
#ifdef BUZZERDAC
	FV_DACHALTMR->ARR = 1/(float)freq * DACTMRMULTIPLIER;
#endif

}

int millis() {

	return HAL_GetTick();
}

void noTone() {
	FV_TONEHALTMR->CR1 = 0;
#ifdef BUZZERDAC
	FV_DACHALTMR->CR1=0;
#endif
}



void noToneTimer() {
   if(millis() >= notonetimer && notonetimer > 0) {
     noTone();
     notonetimer = 0;
   }

}


// Non-Blocking beep blob beep
void playTwoToneInterval(int freq,int freq2, int period, int intervald) {


  if (toneOn) {
    int wait = period + tm;


    if ( wait < millis()) {
      toneOn = false;
      //noTone();
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


// Plays nbeeps then pause

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
  vario = t_vario;

#endif

  if (vario > 9) {
    vario = 9;

  }

  if (vario < -9) {
    vario = -9;

  }

#if defined(SOARDETECTION) && !defined(TESTBUZZER)

  if (varioorg > -0.2 && varioorg < 0.2) {
    int diffe = millis() - stime;
    if (diffe >  (int)(conf.SoarDeadBandTime)) {
      muted = true;
    }
  } else {
    stime = millis();
    muted = false;
  }

#endif

    variofr = ((float)(fabs(vario + 1)) * 200 ) + FV_TONEBASE;

    variof = (AUDIOSMOOTH * variof + variofr )/(AUDIOSMOOTH + 1);

    if (vario <= BUZZERCLIMBING && vario >= BUZZERZEROCLIMB) { // prethermal audio bip bip bip



      if (!muted) {
         playToneInterval(variof, 50, 400);
      }

    }

   if (vario <= (double)(conf.sinkAlarmLevel)/1000 ) { //sink alarm
      if (!muted) {
         playTwoToneInterval(1400, 1800, 50, 50);
      }

   }

#if defined(BUZZSINKALERT) //sink alert beh beh beh (-3)
    if (vario <=  BUZZSINKALERT && vario > (double)(conf.sinkAlarmLevel)/1000 ) {
       playTonePause(300, fabs(vario), BUZZSINKALERTPAUSE);
    }

#endif






  if (vario > BUZZERCLIMBING) {
    pulse = TOPPULSE / (vario * 10) + 100;

    if (!muted) {
      dynaTone(variof);
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


//call this function to test tone
void AUDIO_TestToneCall(){
	if(t_vario <= -5) step = 0.1;
	if(t_vario >= 9) step = -0.1;

	t_vario += step;



}


