/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "audio.h"

uint16_t fv_tone_t=4500;
int8_t step=0;
uint8_t dutycycle;

extern TIM_HandleTypeDef FV_TONETMR;
extern TIM_HandleTypeDef FV_TONEPERIODTMR;

void AUDIO_Setup_Tone() {
	 HAL_TIM_PWM_Start(&FV_TONETMR, FV_TONECHN);
	 HAL_TIM_Base_Start_IT(&FV_TONEPERIODTMR);
	 FV_TONEPERIODHALTMR->PSC  =  (int)SystemCoreClock / 1000;


}


void AUDIO_SetFreq(uint16_t freq) {
	fv_tone_t = 1/freq;
}

void AUDIO_ToneOff() {
	FV_TONEHALTMR->CR1  = 0;

}


void AUDIO_ToneON() {
	FV_TONEHALTMR->CR1 |= TIM_CR1_CEN;
}

void AUDIO_TonePeriodOn() {
	FV_TONEPERIODHALTMR->CR1 |= TIM_CR1_CEN;
}

void AUDIO_TonePeriodOff() {
	FV_TONEPERIODHALTMR->CR1  = 0;
}

void AUDIO_SetTone()
{

	FV_TONEHALTMR->PSC  = SystemCoreClock/2057142.857;
	FV_TONEHALTMR->ARR  = fv_tone_t;
	FV_TONEHALTMR->FV_TONECCR = fv_tone_t/2;

}

void AUDIO_TestToneCall(){
	if(fv_tone_t <= 200) step = 10;
	if(fv_tone_t >= 4500) step = -10;

	fv_tone_t += step;

	AUDIO_SetTone();


}

//Callback from the timer
//Timer periods must be done within this function

void AUDIO_TimerCall() {


	 if(dutycycle >= 5) dutycycle = 0;
		dutycycle++;


	if ((dutycycle > 3) & (dutycycle <= 5)) {
		FV_TONEHALTMR->CR1  = 0;

	}else{

		FV_TONEHALTMR->CR1  |= TIM_CR1_CEN;
	}

	FV_TONEPERIODHALTMR->ARR = (int) fv_tone_t/20;


}

