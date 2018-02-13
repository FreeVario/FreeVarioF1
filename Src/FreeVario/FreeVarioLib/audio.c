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
float step=0;
uint8_t dutycycle;
int8_t variomode=AUDIO_VARIO_SILENT;
float t_vario=0;

extern TIM_HandleTypeDef FV_TONETMR;
extern TIM_HandleTypeDef FV_TONEPERIODTMR;

void AUDIO_Setup_Tone() {
	 HAL_TIM_PWM_Start(&FV_TONETMR, FV_TONECHN);
	 HAL_TIM_Base_Start_IT(&FV_TONEPERIODTMR);
	 FV_TONEPERIODHALTMR->PSC  =  (int)SystemCoreClock / 1000;
	 FV_TONEPERIODHALTMR->ARR = (int) 4500/20;

}


void noTone() {
	AUDIO_ToneOff();
	AUDIO_TonePeriodOff();
}

void AUDIO_SetFreq(float freq) {

	fv_tone_t = 1/((1 + freq) * 400 + 1000) * 5000000;

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

//main function to call from the main loop
void AUDIO_Vario(float vario){
	if (vario > 0.1 ) {
		variomode = AUDIO_VARIO_UP;
		AUDIO_SetFreq(vario);
		AUDIO_SetTone();

		if (TIM3->CR1 != TIM_CR1_CEN) {
		AUDIO_TonePeriodOn();
		}
	} else {
		variomode = AUDIO_VARIO_SILENT;
	}
}

//call this function to test tone
void AUDIO_TestToneCall(){
	if(t_vario <= 0.2) step = 0.2;
	if(t_vario >= 8) step = -0.2;

	t_vario += step;

	AUDIO_Vario(t_vario);


}

void AUDIO_varioUP(){

	 if(dutycycle >= 5) dutycycle = 0;
		dutycycle++;


	if ((dutycycle > 3) & (dutycycle <= 5)) {
		FV_TONEHALTMR->CR1  = 0;

	}else{

		FV_TONEHALTMR->CR1  |= TIM_CR1_CEN;
	}

	FV_TONEPERIODHALTMR->ARR = (int) fv_tone_t/20;
}

//Callback from the timer
//Timer periods must be done within this function

void AUDIO_TimerCall() {


	switch(variomode) {

	case AUDIO_VARIO_SILENT :
		noTone();
		break;

	case AUDIO_VARIO_UP:
		AUDIO_varioUP();
		break;

	}





}

