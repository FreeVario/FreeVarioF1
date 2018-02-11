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

uint8_t receiveBuffer[120];



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


}


static void loop() {


	//AUDIO_TestToneCall();

	HAL_Delay(100);

}
