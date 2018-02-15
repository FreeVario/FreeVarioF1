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
uint8_t toneisOn=1;
uint8_t peroidisOn=0;
uint8_t g_period=0;
int8_t sinkwarncounter=0;
int8_t variomode=AUDIO_VARIO_SILENT;
float t_vario=-5;

extern TIM_HandleTypeDef FV_TONETMR;
extern TIM_HandleTypeDef FV_TONEPERIODTMR;

void AUDIO_Setup_Tone() {
	 HAL_TIM_PWM_Start(&FV_TONETMR, FV_TONECHN);
	 HAL_TIM_Base_Start_IT(&FV_TONEPERIODTMR);
	 FV_TONEHALTMR->PSC  = SystemCoreClock/2057142.857;
	 FV_TONEPERIODHALTMR->PSC  =  (int)SystemCoreClock / 1000;
	 FV_TONEPERIODHALTMR->ARR = (int) 4500/30;


}


void noTone() {
	AUDIO_ToneOff();
	AUDIO_TonePeriodOff();
	dutycycle = 0;
}

void AUDIO_SetFreq(float freq) {

	fv_tone_t = 1/((1 + freq) * 400 + 1000) * 5000000;

}

void AUDIO_ToneOffOnce() {
	 FV_TONEHALTMR->CR1  = 0;
	toneisOn =0;

}
void AUDIO_ToneOff() {
	 FV_TONEHALTMR->CR1  = 0;


}

void AUDIO_ToneONonce() {
	if (!toneisOn)FV_TONEHALTMR->CR1 |= TIM_CR1_CEN;
	toneisOn =1;

}


void AUDIO_ToneON() {
	FV_TONEHALTMR->CR1 |= TIM_CR1_CEN;

}

void AUDIO_TonePeriodOn() {

	if(!peroidisOn)	FV_TONEPERIODHALTMR->CR1 |= TIM_CR1_CEN;
	peroidisOn=1;

}

void AUDIO_SinglePeriod(int8_t period) {

	if (!(g_period && period)) {
		g_period = period;
		FV_TONEPERIODHALTMR->ARR = period;

	}
}

void AUDIO_TonePeriodOff() {
	if( peroidisOn) FV_TONEPERIODHALTMR->CR1  = 0;
	peroidisOn=0;
}

void AUDIO_SetTone()
{

	//FV_TONEHALTMR->PSC  = SystemCoreClock/2057142.857;
	FV_TONEHALTMR->ARR  = fv_tone_t;
	FV_TONEHALTMR->FV_TONECCR = fv_tone_t/2;

}

//main function to call from the main loop
void AUDIO_Vario(float vario){
	vario = t_vario;

	if (vario > 0.2 ) {
		variomode = AUDIO_VARIO_UP;
		AUDIO_SetFreq(vario);
		AUDIO_SetTone();
		AUDIO_TonePeriodOn();

	}else if ((vario <= 0.1) && (vario >= -0.3)){
		variomode = AUDIO_VARIO_PRETHERMAL;
		AUDIO_SetFreq(vario);
		AUDIO_SetTone();
		AUDIO_TonePeriodOn();
		AUDIO_SinglePeriod(100);

	}else if (vario <= -4){
		variomode = AUDIO_VARIO_SINKALARM;
		AUDIO_TonePeriodOn();
		AUDIO_ToneON();

	}else if ((vario <= -1.8) && (vario >= -3.9)){
		variomode = AUDIO_VARIO_SINK_ALERT;
		AUDIO_TonePeriodOn();
		sinkwarncounter = 2*vario;
	} else {
		variomode = AUDIO_VARIO_SILENT;
		AUDIO_ToneOffOnce();
	}
}

//call this function to test tone
void AUDIO_TestToneCall(){
	if(t_vario <= -2) step = 0.2;
	if(t_vario >= 9) step = -0.2;

	t_vario += step;

	//AUDIO_Vario(t_vario);

}

void AUDIO_varioUP(){

	 if(dutycycle >= 5) dutycycle = 0;
		dutycycle++;


	if ((dutycycle > 3) && (dutycycle <= 5)) {
		FV_TONEHALTMR->CR1  = 0;

	}else{

		FV_TONEHALTMR->CR1  |= TIM_CR1_CEN;
	}

	FV_TONEPERIODHALTMR->ARR = (int) fv_tone_t/20;
}


void AUDIO_varioPrethermal(){

	 if(dutycycle >= 9) dutycycle = 0;
		dutycycle++;


	if ((dutycycle == 1) || (dutycycle == 4) || (dutycycle == 7)) {


		AUDIO_ToneON();
	}else{


		AUDIO_ToneOff();
	}


}

void AUDIO_varioSinkAlarm() {

	 if(dutycycle >= 1) {
		 dutycycle = 0;
		 fv_tone_t=1000;
		 AUDIO_SetTone();

	 }else {
		 dutycycle = 1;
		 fv_tone_t=2000;
		 AUDIO_SetTone();


	 }

	FV_TONEPERIODHALTMR->ARR = (int) 150;

}

void AUDIO_varioSinkWarning() {

	 if(dutycycle >= 10 + -sinkwarncounter ) dutycycle = 0;
	    dutycycle++;

	    if(dutycycle <= -sinkwarncounter){
	    	if(dutycycle%2 == 0) {
	    		fv_tone_t=8500;
	    		AUDIO_SetTone();
	    		AUDIO_ToneON();
	    	}else{
	    		AUDIO_ToneOff();
	    	}

	    } else {
	    	AUDIO_ToneOff();
	    }

	FV_TONEPERIODHALTMR->ARR = (int) 350;

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

	case AUDIO_VARIO_PRETHERMAL:
		AUDIO_varioPrethermal();
			break;

	case AUDIO_VARIO_SINKALARM:
		AUDIO_varioSinkAlarm();
		break;

	case AUDIO_VARIO_SINK_ALERT:
		AUDIO_varioSinkWarning();
		break;

	}





}

