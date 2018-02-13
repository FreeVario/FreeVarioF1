/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "freevario.h"
#include "audio.h"
#include "barosensor.h"

uint8_t receiveBuffer[120];
uint8_t sendBuffer[120];
uint32_t sc_timer=0;
uint32_t sc_timer100=0;
uint32_t sc_timer1000=0;

extern TIM_HandleTypeDef FV_10HZTMR;


//GPS data RX interrupt callback
void FV_GpsCallback() {


	  memcpy(sendBuffer,receiveBuffer,sizeof(receiveBuffer));
	  CDC_Transmit_FS(sendBuffer, sizeof(sendBuffer));
	  HAL_GPIO_TogglePin(FV_LED_GPIO, FV_LED);


}

//pass through to audio.h
void FV_TonePeriodCall() {

	AUDIO_TimerCall();

}


//Called from main.c
void FV_Run(){
	setup();

	while(1){
		loop();
	}
}

static void setup() {
	 setupConfig();
	 HAL_UART_Receive_DMA(&FV_UART2, receiveBuffer, sizeof(receiveBuffer));
	 AUDIO_Setup_Tone();
	 AUDIO_SetTone();
	 AUDIO_ToneOff();
	 AUDIO_TonePeriodOff();

	 BARO_Setup();



}


static void loop() {


	//AUDIO_TestToneCall();


	if (HAL_GetTick() >= (sc_timer+20)) {
		sc_timer=HAL_GetTick();
		Baro_GetSensorData();

	}


	if (HAL_GetTick() >= (sc_timer100+100)) {
		sc_timer100=HAL_GetTick();
		calcVario();
		//AUDIO_Vario(currentVarioMPS);
	}


	if (HAL_GetTick() >= (sc_timer1000+1000)) {
		sc_timer1000=HAL_GetTick();
		AUDIO_TestToneCall();
	}



	HAL_Delay(1);

}
